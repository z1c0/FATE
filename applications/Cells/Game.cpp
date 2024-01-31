#include "Game.h"
#include "Cell.h"
#include "Chaser.h"
#include "ids.h"

//--------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CGame());
}



//--------------------------------------------------------------------------------
CGame::CGame() : CFateApp(DM_LANDSCAPE)
{
  m_btnQuit= NULL;
  m_bmpPitch= NULL;
  m_dwFrames= 0;
  m_iChase= 1;
  
  for (int i=0; i<MAX_NR_OF_CELLS; i++) {
    m_pCells[i]= NULL;
  }
}

//--------------------------------------------------------------------------------
CGame::~CGame()
{
  SAFE_DELETE(m_btnQuit);
  SAFE_DELETE(m_bmpPitch);
  for (int i=0; i<MAX_NR_OF_CELLS; i++) {
    SAFE_DELETE(m_pCells[i]);
  }
}

//--------------------------------------------------------------------------------
bool CGame::InitFateApp()
{
  INIT_REMOTE_CONSOLE(TEXT("COM"), 1);
  
  // create quit button
  m_btnQuit= new CFButton(TEXT("X"));
  m_btnQuit->SetId(ID_BTN_QUIT);
  Add(*m_btnQuit);

  // create pitch bitmap
  m_bmpPitch = new CFBitmap(*m_pSystem->GetDoubleBuffer());
  m_bmpPitch->Create(BOUND_R - BOUND_L, BOUND_D - BOUND_U);
  m_bmpPitch->SetX(BOUND_L);
  m_bmpPitch->SetY(BOUND_U);
  m_bmpPitch->SolidFill(RGB(0, 170, 40));

  // create the life-forms
  if (!CreateCells()) return(FALSE);

  // initialize random seed
  m_dwOld= GetTickCount();
  srand(m_dwOld);
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CGame::Idle()
{
  int i;
  
  DWORD dwTicks= GetTickCount();
  if (dwTicks - m_dwOld >= 1000) {
    REMOTE_CONSOLE(TEXT("%d"), m_dwFrames);
    m_dwOld= dwTicks;
    m_dwFrames= 0;
  }
  m_dwFrames++;

  m_bmpPitch->Blit();
  // iterate through all cells
  for (i=0; i<MAX_NR_OF_CELLS; i++) {        
    // cell "i" lives?
    if (m_pCells[i]) {
      // cell performs individual movement
      m_pCells[i]->Move(m_pCells[m_iChase]->GetX(), m_pCells[m_iChase]->GetY());
      // cell performs individual drawing operation
      m_pCells[i]->Draw();
    }
  }  

#ifndef _WIN32_WCE
  Sleep(50);
#endif
}


//--------------------------------------------------------------------------------
void CGame::Draw()
{
  ClearScreen(RGB(200, 200, 200));
  m_bmpPitch->Blit();
}

//--------------------------------------------------------------------------------
bool CGame::ButtonReleased(DWORD dwBtnID)
{
  if (dwBtnID == ID_BTN_QUIT) {
    Exit();
    return(TRUE);
  }

  return(FALSE);
}

//--------------------------------------------------------------------------------
bool CGame::StylusDown(int iPosX, int iPosY)
{
  // iterate through all cells
  for (int i=0; i<MAX_NR_OF_CELLS; i++) {        
    // cell "i" lives?
    if (m_pCells[i]) {
      if (m_pCells[i]->PointInside(iPosX, iPosY)) m_iChase= i;
    }
  }
  
  return(FALSE);
}

//--------------------------------------------------------------------------------
bool CGame::CreateCells()
{
  m_pCells[0]= new CChaser();
  if (!m_pCells[0]->Create(*m_pSystem->GetDoubleBuffer())) return(FALSE);
  
  for (int i=1; i<6; i++) {
    m_pCells[i]= new CCell();
    if (!m_pCells[i]->Create(*m_pSystem->GetDoubleBuffer())) return(FALSE);
  }
  
  return(TRUE);
}
