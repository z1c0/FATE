#include "Cell.h"
#include "Game.h"


//--------------------------------------------------------------------------------
CCell::CCell()
{
  m_iPosX  = BOUND_L + (rand() % (BOUND_R - BOUND_L));
  m_iPosY  = BOUND_U + (rand() % (BOUND_D - BOUND_U));
  m_iWidth = 8;
  m_iHeight= 8;
  m_bmp= NULL;
  
  // movement vector
  m_iVec[0]= 2;
  m_iVec[1]= 2;
}

//--------------------------------------------------------------------------------
CCell::~CCell()
{
  SAFE_DELETE(m_bmp);
}

//--------------------------------------------------------------------------------
bool CCell::Create(const CFBitmap& destbmp)
{
  m_bmp= new CFBitmap(destbmp);
  m_bmp->Create(m_iWidth, m_iHeight);
  m_bmp->SolidFill(RGB(255, 0, 0));

  return(TRUE);
}

//--------------------------------------------------------------------------------
void CCell::Move(int x, int y)
{
  // change movement-vector by random
  if (rand() % 5000 < 100) {
    m_iVec[0]= rand() % 3 - 1; 
  }
  if (rand() % 5000 < 100) {
    m_iVec[1]= rand() % 3 - 1; 
  }
  
  if ((m_iPosX <= BOUND_L)||(m_iPosX >= BOUND_R)) m_iVec[0]*= -1;
  if ((m_iPosY <= BOUND_U)||(m_iPosY >= BOUND_D)) m_iVec[1]*= -1;
  
  m_iPosX+=  m_iVec[0];
  m_iPosY+=  m_iVec[1];
}

//--------------------------------------------------------------------------------
void CCell::Draw()
{
  m_bmp->SetX(m_iPosX);
  m_bmp->SetY(m_iPosY);
  m_bmp->Blit();
}

//--------------------------------------------------------------------------------
bool CCell::Touches(const CCell &cell)
{
  return(TRUE);
}