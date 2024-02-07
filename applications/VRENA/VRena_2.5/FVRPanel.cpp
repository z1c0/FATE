// FExtPanel.cpp: implementation of the CFExtPanel class.
//////////////////////////////////////////////////////////////////////
#include "FVRPanel.h"
#include "FVRena.h"
#include "ids.h"
#include "resource.h"

//--------------------------------------------------------------------------------
CFVRPanel::CFVRPanel()
{
  m_iTab= 0;
  m_iLeftApp= 0;  
  m_iIconY= 233;
}

//--------------------------------------------------------------------------------
CFVRPanel::~CFVRPanel()
{
  delete(m_btnClose);
  delete(m_btnLeft);
  delete(m_btnRight);
  delete(m_bmpTitle);
  delete(m_bmpTab[0]);
  delete(m_bmpTab[1]);
  delete(m_bmpTab[2]);
  delete(m_bmpReset);
  // load images
  for (int i=0; i<m_iNrOfApps; i++) {
    delete(m_bmpIcons[i]);
    delete(m_bmpPics[i]);
  }
  delete(m_bmpIcons);
  delete(m_bmpPics);
}

//--------------------------------------------------------------------------------
void CFVRPanel::Draw()
{
  m_bmpTitle->Blit();
  m_bmpReset->Blit();
  m_btnClose->Draw();
  m_bmpTab[m_iTab]->Blit();
  m_bmpPics[m_iLeftApp + m_iTab]->Blit();
  m_btnLeft->Draw();
  m_btnRight->Draw();

  DrawIcons();
}

//--------------------------------------------------------------------------------
void CFVRPanel::DrawIcons()
{
  m_bmpIcons[m_iLeftApp]->SetX(39);
  m_bmpIcons[m_iLeftApp]->SetY(m_iIconY);
  m_bmpIcons[m_iLeftApp]->TransBlit(0);  
  if (m_iNrOfApps > 1) {
    m_bmpIcons[m_iLeftApp + 1]->SetX(100);
    m_bmpIcons[m_iLeftApp + 1]->SetY(m_iIconY);
    m_bmpIcons[m_iLeftApp + 1]->TransBlit(0);  
    if (m_iNrOfApps > 2) {
      m_bmpIcons[m_iLeftApp + 2]->SetX(160);
      m_bmpIcons[m_iLeftApp + 2]->SetY(m_iIconY);
      m_bmpIcons[m_iLeftApp + 2]->TransBlit(0);  
    }
  }
}

//--------------------------------------------------------------------------------
bool CFVRPanel::Create()
{
  TCHAR szPath[1024];
  TCHAR szFullPath[2048];  
  TCHAR szBuff[4];  
 
  // cast pointer to application object
  m_theApp= (CFVRena*)m_app;

  // set path for bitmaps
  _tcscpy(szPath, m_app->GetAppPath());
  _tcscat(szPath, TEXT("menupics\\"));
  
  // title
  m_bmpTitle= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpTitle->Load(IDB_VR_TITLE)) return(FALSE);
  
  // tabs
  m_bmpTab[0]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpTab[0]->Load(IDB_LI_UP)) return(FALSE);
  
  m_bmpTab[1]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpTab[1]->Load(IDB_MI_UP)) return(FALSE);
  
  m_bmpTab[2]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpTab[2]->Load(IDB_RE_UP)) return(FALSE);  
  
  // close button
  CFBitmap *bmpClose= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpClose->Load(IDB_SKIP_SMALL)) return(FALSE);
  m_btnClose= new CFButton(bmpClose);
  m_btnClose->SetVisible(TRUE);
  m_btnClose->SetId(ID_BTN_CLOSEVR);
  Add(*m_btnClose);

  // left arrow button
  CFBitmap *bmpLeft= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpLeft->Load(IDB_LINKS)) return(FALSE);
  CFBitmap *bmpLeftPr= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpLeftPr->Load(IDB_LINKS_PR)) return(FALSE);
  CFBitmap *bmpLeftDis= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpLeftDis->Load(IDB_LINKS_END)) return(FALSE);  
  m_btnLeft= new CFButton(bmpLeft, bmpLeftPr, bmpLeftDis);
  m_btnLeft->SetVisible(TRUE);
  m_btnLeft->SetEnabled(FALSE);
  m_btnLeft->SetId(ID_BTN_LEFT);
  m_btnLeft->SetTransparency(RGB(51, 102, 0));
  Add(*m_btnLeft);

  // right arrow button
  CFBitmap *bmpRight= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpRight->Load(IDB_RECHTS)) return(FALSE);
  CFBitmap *bmpRightPr= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpRightPr->Load(IDB_RECHTS_PR)) return(FALSE);
  CFBitmap *bmpRightDis= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpRightDis->Load(IDB_RECHTS_END)) return(FALSE);  
  m_btnRight= new CFButton(bmpRight, bmpRightPr, bmpRightDis);
  m_btnRight->SetVisible(TRUE);
  m_btnRight->SetId(ID_BTN_RIGHT);
  m_btnRight->SetTransparency(RGB(51, 102, 0));
  Add(*m_btnRight);

  // reset
  m_bmpReset= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpReset->Load(IDB_RESET)) return(FALSE);

  // retrieve number of applications to display
  m_iNrOfApps= GetNumberOfApps();
  // number of applications is increased by one because additional "clear-application" (ARS_BOX) is added
  m_iNrOfApps++;
  m_btnRight->SetEnabled(m_iNrOfApps > 3);
  
  m_bmpIcons= new CFBitmap*[m_iNrOfApps];    
  m_bmpPics = new CFBitmap*[m_iNrOfApps];
  
  // add icon and image for ARS-BOX app
  m_bmpIcons[m_iNrOfApps - 1]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpIcons[m_iNrOfApps - 1]->Load(IDB_ARSBOX_ICON)) return(FALSE);
	m_bmpPics[m_iNrOfApps - 1]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpPics[m_iNrOfApps - 1]->Load(IDB_ARSBOX_PIC)) return(FALSE);

  // load images
  for (int i=0; i<m_iNrOfApps - 1; i++) {
		// load icons
    _tcscpy(szFullPath, szPath);
		_tcscat(szFullPath, TEXT("icon_"));
		_itot(i, szBuff, 10);
		_tcscat(szFullPath, szBuff);
		_tcscat(szFullPath, TEXT(".bmp"));
		m_bmpIcons[i]= new CFBitmap(*m_pSystem->GetDoubleBuffer());    
  	if (!m_bmpIcons[i]->Load(szFullPath)) return(FALSE);

		// load pictures
    _tcscpy(szFullPath, szPath);
		_tcscat(szFullPath, TEXT("pic_"));
		_itot(i, szBuff, 10);
		_tcscat(szFullPath, szBuff);
		_tcscat(szFullPath, TEXT(".bmp"));
		m_bmpPics[i]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
    if (!m_bmpPics[i]->Load(szFullPath)) return(FALSE);
	}  
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
bool CFVRPanel::StylusDown(int xPos, int yPos)
{
  if (PointInRect(&m_rectTab[0], xPos, yPos)) {
    m_iTab= 0;
    Draw();
    return(TRUE);
  
  } else if (PointInRect(&m_rectTab[1], xPos, yPos)) {
    m_iTab= 1;
    Draw();
    return(TRUE);
  
  } else if (PointInRect(&m_rectTab[2], xPos, yPos)) {
    m_iTab= 2;
    Draw();
    return(TRUE);
  
  } else if (m_bmpReset->PointInside(xPos, yPos)) {
    // reset signal for tracker
    m_theApp->SendTrackerReset();
    return(TRUE);
  
  } else if (m_bmpPics[0]->PointInside(xPos, yPos)) {
    // launch CAVE application
    int iNr= m_iLeftApp + m_iTab + 1;
    if (iNr == m_iNrOfApps) iNr= 0;
    m_theApp->SendCaveMenuChange(iNr);
    return(TRUE);
  }

  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFVRPanel::UpdatePos()
{
  if (m_pSystem) {
    int iPosY;

    m_bmpTitle->SetX(m_iPosX);
    m_bmpTitle->SetY(m_iPosY);
    m_btnClose->SetX(m_bmpTitle->GetRight() - m_btnClose->GetWidth());
    m_btnClose->SetY(m_iPosY + 3);
    m_bmpReset->SetX(m_btnClose->GetX() - m_bmpReset->GetWidth() - 9);
    m_bmpReset->SetY(m_iPosY + 6);
    m_btnLeft->SetX(m_iPosX + 7);
    m_btnLeft->SetY(m_iPosY + 210);
    m_btnRight->SetX(m_iPosX + 218);
    m_btnRight->SetY(m_btnLeft->GetY());
    for (int i=0; i<3; i++) {
      m_bmpTab[i]->SetX(m_iPosX);
      m_bmpTab[i]->SetY(m_bmpTitle->GetBottom());
    }
    iPosY= m_bmpTitle->GetBottom();
    for (i=0; i<m_iNrOfApps; i++) {
      m_bmpPics[i]->SetX(m_iPosX + 10);
      m_bmpPics[i]->SetY(iPosY + 10);
    }

    m_rectTab[0].left  = m_iPosX +  25;
    m_rectTab[0].top   =   iPosY + 180;
    m_rectTab[0].right = m_iPosX +  84;
    m_rectTab[0].bottom=   iPosY + 230;
    m_rectTab[1].left  = m_iPosX +  85;
    m_rectTab[1].top   =   iPosY + 180;
    m_rectTab[1].right = m_iPosX + 149;
    m_rectTab[1].bottom=   iPosY + 230;
    m_rectTab[2].left  = m_iPosX + 150;
    m_rectTab[2].top   =   iPosY + 180;
    m_rectTab[2].right = m_iPosX + 210;
    m_rectTab[2].bottom=   iPosY + 230;
  }
}

//--------------------------------------------------------------------------------
bool CFVRPanel::PointInRect(const RECT *rect, int iPosX, int iPosY)
{
  return((iPosX >= rect->left)&&(iPosX <= rect->right)&&
         (iPosY >= rect->top)&&(iPosY <= rect->bottom));
}

//--------------------------------------------------------------------------------
int CFVRPanel::GetNumberOfApps()
{
  TCHAR szFilePath[MAX_PATH];
  TCHAR szBuff[8];
  int iNrOfApps= 0;
  HANDLE hFile;

  // go through pics to find out about number of apps  
  while(TRUE) {
		// find pictures
    _tcscpy(szFilePath, m_app->GetAppPath());
    _tcscat(szFilePath, TEXT("menupics\\pic_"));
		_itot(iNrOfApps, szBuff, 10);
		_tcscat(szFilePath, szBuff);
		_tcscat(szFilePath, TEXT(".bmp"));

    hFile= CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL); 
    if (hFile == INVALID_HANDLE_VALUE) break;  // file was not found, we are done
    else {
      iNrOfApps++;
      CloseHandle(hFile);
    }
	}  
  return(iNrOfApps);
}

//--------------------------------------------------------------------------------
bool CFVRPanel::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_BTN_LEFT:
      if (m_iLeftApp > 0) m_iLeftApp--;
      // left end reached?
      if (m_iLeftApp == 0) m_btnLeft->SetEnabled(FALSE);
      m_btnRight->SetEnabled(TRUE);
      Draw();
      break;

    case ID_BTN_RIGHT:
      if (m_iLeftApp < m_iNrOfApps - 3) m_iLeftApp++;
      // rigth end reached?
      if (m_iLeftApp  == m_iNrOfApps - 3) m_btnRight->SetEnabled(FALSE);
      m_btnLeft->SetEnabled(TRUE);
      Draw();
      break;

    default:
      break;
  }

  return(FALSE);
}

