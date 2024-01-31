// FScreens.cpp: implementation of the CFScreens class.
//////////////////////////////////////////////////////////////////////
#include "FScreens.h"
#include "FVRena.h"
#include "ppt_protocol.h"
#include "resource.h"

//--------------------------------------------------------------------------------
CFScreens::CFScreens(CFPPTPanel **ppPanelsPPT) : CFPanel()
{
  m_bCreated= FALSE;
  m_iActiveScreen= -1;
  m_aPanelsPPT= ppPanelsPPT;
  for (int i=0; i<3; i++) {
    m_iStates[i]= VRENA_INACTIVE;
  }

  // default number of used screens
  m_iNrOfScreensUsed= 3;
  
  // set according screens active
  m_bActive[0]= FALSE;
  m_bActive[1]= FALSE;
  m_bActive[2]= FALSE;
  for (i=0; i<m_iNrOfScreensUsed; i++) {
    m_bActive[i]= TRUE;
  }
}

//--------------------------------------------------------------------------------
CFScreens::~CFScreens()
{
  delete(m_bmpPlay);
  delete(m_bmpPause);
  delete(m_bmpBack);
  for (int i=0; i<3; i++) {
    delete(m_bmpScreens[i][0]);
    delete(m_bmpScreens[i][1]);
    delete(m_bmpScreens[i][2]);
  }
}

//--------------------------------------------------------------------------------
void CFScreens::Draw()
{
  m_bmpBack->Blit();

  // draw GUI depending on number of screens in use
  switch(m_iNrOfScreensUsed) {
    case 1:
      m_bmpScreens[1][m_iStates[1]]->TransBlit(0);
      break;

    case 2:
      m_bmpScreens[0][m_iStates[0]]->TransBlit(0);
      m_bmpScreens[2][m_iStates[2]]->TransBlit(0);
      break;

    case 3:
      m_bmpScreens[0][m_iStates[0]]->TransBlit(0);
      m_bmpScreens[1][m_iStates[1]]->TransBlit(0);
      m_bmpScreens[2][m_iStates[2]]->TransBlit(0);
      break;
  }

  for (int i=0; i<3; i++) {
    // show "Play" or "Pause" buttons for MOVIE content
    if ((m_iMedia[i] == 1)||(m_iMedia[i] == 5)) {
      if (m_bVideoPaused[i]) {
        m_bmpPlay->SetX(m_bmpScreens[i][0]->GetX() + 30);
        m_bmpPlay->SetY(m_bmpBack->GetY() + 35);
        m_bmpPlay->Blit();
      } else {
        m_bmpPause->SetX(m_bmpScreens[i][0]->GetX() + 30);
        m_bmpPause->SetY(m_bmpBack->GetY() + 35);
        m_bmpPause->Blit();
      } 
    }
  }
}

//--------------------------------------------------------------------------------
BOOL CFScreens::Create()
{
  // cast pointer to application object
  m_theApp= (CFVRena*)m_app;
  m_app->SetTransparency(0);

  // load background bitmap
  m_bmpBack= new CFBitmap(m_hdc);
  if (!m_bmpBack->Load(IDB_SCREENS_BACK)) return(FALSE);
  
  // load bitmaps for screens
  m_bmpScreens[0][0]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[0][0]->Load(IDB_BLANC_LEFT)) return(FALSE);

  m_bmpScreens[0][1]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[0][1]->Load(IDB_ACTIVE_LEFT)) return(FALSE);

  m_bmpScreens[0][2]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[0][2]->Load(IDB_SET_LEFT)) return(FALSE);

  m_bmpScreens[1][0]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[1][0]->Load(IDB_BLANC_MID)) return(FALSE);

  m_bmpScreens[1][1]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[1][1]->Load(IDB_ACTIVE_MID)) return(FALSE);

  m_bmpScreens[1][2]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[1][2]->Load(IDB_SET_MID)) return(FALSE);

  m_bmpScreens[2][0]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[2][0]->Load(IDB_BLANC_RIGHT)) return(FALSE);

  m_bmpScreens[2][1]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[2][1]->Load(IDB_ACTIVE_RIGHT)) return(FALSE);

  m_bmpScreens[2][2]= new CFBitmap(m_hdc);
  if (!m_bmpScreens[2][2]->Load(IDB_SET_RIGHT)) return(FALSE);
  
  // play button for videos
  m_bmpPlay= new CFBitmap(m_hdc);
  if (!m_bmpPlay->Load(IDB_PLAY)) return(FALSE);
  
  // pause button for videos
  m_bmpPause= new CFBitmap(m_hdc);
  if (!m_bmpPause->Load(IDB_PAUSE)) return(FALSE);

  m_bCreated= TRUE;
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::StylusDown(int xPos, int yPos)
{
  LPDRAGGINGINFO pDI;

  // drag into one of the screens
  if ((m_bmpScreens[0][0]->PointInside(xPos, yPos))&&(m_iStates[0] != VRENA_INACTIVE)) {
    m_app->StartDragMode(m_bmpScreens[0][2], xPos, yPos);
    pDI= m_app->GetDragInfo();
    pDI->dwReserved1= 0xFF;
    pDI->dwReserved2= 0;
    return(TRUE);  
    
  } else if ((m_bmpScreens[1][0]->PointInside(xPos, yPos))&&(m_iStates[1] != VRENA_INACTIVE)) {
    m_app->StartDragMode(m_bmpScreens[1][2], xPos, yPos);
    pDI= m_app->GetDragInfo();
    pDI->dwReserved1= 0xFF;
    pDI->dwReserved2= 1;
    return(TRUE);  
    
  } else if ((m_bmpScreens[2][0]->PointInside(xPos, yPos))&&(m_iStates[2] != VRENA_INACTIVE)) {
    m_app->StartDragMode(m_bmpScreens[2][2], xPos, yPos);
    pDI= m_app->GetDragInfo();
    pDI->dwReserved1= 0xFF;
    pDI->dwReserved2= 2;
    return(TRUE);  
  }     
    
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFScreens::StylusMove(int xPos, int yPos)
{
  if (m_app->IsDragMode()) {
    LPDRAGGINGINFO pDI= m_app->GetDragInfo();
    if ((abs(pDI->iOrigX + pDI->iOffsX - xPos) > 5)||(abs(pDI->iOrigY + pDI->iOffsY - yPos) > 5))
      m_app->DoDrag(xPos, yPos);
    return(TRUE);
  }

  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFScreens::StylusUp(int xPos, int yPos)
{
  if (m_app->IsDragMode()) {
    LPDRAGGINGINFO pDI= m_app->GetDragInfo();
    
    // quit drag anyway
    m_app->StopDragMode();
   
    if (pDI->dwReserved1 == 0xFF) {
      if (!m_bmpScreens[pDI->dwReserved2][0]->PointInside(xPos, yPos)) {
        // content was dragged out from screen
        ScreenUnload(pDI->dwReserved2);
      
      } else if (m_iStates[pDI->dwReserved2] != VRENA_INACTIVE) {
        // it was just a "click" on the screen
        ScreenTapped(pDI->dwReserved2);
      }

      pDI->dwReserved1= 0;      
      pDI->cbDrag->SetX(pDI->iOrigX);
      pDI->cbDrag->SetY(pDI->iOrigY);
      Draw();
      pDI->cbDrag->SaveUnder();
    
    } else {
      char szFile[MAX_PATH];
      // convert textual information to ASCII string
#ifdef _WIN32_WCE
      wcstombs(szFile, pDI->strDesc, _tcsclen(pDI->strDesc) + 1);
#else
      strcpy(szFile, pDI->strDesc);
#endif

      // drag into one of the screens
      if (m_bmpScreens[0][0]->PointInside(xPos, yPos)) {
        ScreenActivated(0, m_theApp->GetMediaType(), szFile);
    
      } else if (m_bmpScreens[1][0]->PointInside(xPos, yPos)) {
        ScreenActivated(1, m_theApp->GetMediaType(), szFile);
    
      } else if (m_bmpScreens[2][0]->PointInside(xPos, yPos)) {
        ScreenActivated(2, m_theApp->GetMediaType(), szFile);    
      } 
    }
    
    return(TRUE);  
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFScreens::UpdatePos()
{
  if (m_bCreated) {
    int i;

    m_bmpBack->SetX(m_iPosX);
    m_bmpBack->SetY(m_iPosY);

    // create GUI depending on number of screens in use
    switch(m_iNrOfScreensUsed) {
      case 1:
        for (i=0; i<3; i++) {
          m_bmpScreens[1][i]->SetX(m_iPosX + 85);
          m_bmpScreens[1][i]->SetY(m_iPosY + 20);
        }
        break;

      case 2:
        for (i=0; i<3; i++) {
          m_bmpScreens[0][i]->SetX(m_iPosX + 55);
          m_bmpScreens[0][i]->SetY(m_iPosY + 5);
          m_bmpScreens[2][i]->SetX(m_bmpScreens[0][i]->GetRight() - 6);
          m_bmpScreens[2][i]->SetY(m_bmpScreens[0][0]->GetY());
        }
       break;

      case 3:
        for (i=0; i<3; i++) {
          m_bmpScreens[0][i]->SetX(m_iPosX + 16);
          m_bmpScreens[0][i]->SetY(m_iPosY + 5);
          m_bmpScreens[1][i]->SetX(m_bmpScreens[0][i]->GetRight() - 3);
          m_bmpScreens[1][i]->SetY(m_iPosY + 22);
          m_bmpScreens[2][i]->SetX(m_bmpScreens[1][i]->GetRight() - 5);
          m_bmpScreens[2][i]->SetY(m_bmpScreens[0][0]->GetY());
        }
        break;

      default:
        // SHOULD NEVER HAPPEN
        break;
    }
  }
}

//--------------------------------------------------------------------------------
BOOL CFScreens::ScreenActivated(int iScreen, int iMediaType, char *pszFileName)
{
  // check if screen is active
  if ((iMediaType != 3)&&(!m_bActive[iScreen])) return(FALSE);

  // unload previously selected content
  if (m_iStates[iScreen] != VRENA_INACTIVE) ScreenUnload(iScreen, FALSE);
  
  // NB: SPECIAL TREATMENT FOR STEREO VIDEO TO CAUSE A CORRECT MATRIX SWITCH!!
  if (strstr(pszFileName, "stereo_")) iMediaType= 5;

  // NB: GRABBER MODE IN PLAYER IS TREATED LIKE STEREO-VIDEO REGARDING MATRIX SETTINGS
  if (strstr(pszFileName, "GRABBER")) iMediaType= 5;

  switch(iMediaType) {
    case 0:  // PPT
      if (!StartPPT(iScreen, pszFileName)) return(FALSE);
      break;

    case 1:  // MONO VIDEO
      if (!OpenMovie(iScreen, pszFileName, TRUE)) return(FALSE);
      break;
    
    case 2:  // PICS
      if (!OpenMovie(iScreen, pszFileName, TRUE)) return(FALSE);
      break;
    
    case 3:  // 3D
      Start3D();      
      m_theApp->ModelLoad(pszFileName);
      break;

    case 4:  // VR
      break;

    case 5:  // STEREO VIDEO
      if (!OpenMovie(iScreen, pszFileName, FALSE)) return(FALSE);
      break;

    default:
      // should never happen!
      return(FALSE);
  }  

  for (int i=0; i<3; i++) {
    if (m_iStates[i] == VRENA_SELECTED) 
      m_iStates[i]= VRENA_RUNNING;  // formerly selected is now running
  }

  m_iActiveScreen   = iScreen;
  m_iMedia[iScreen] = iMediaType;
  m_iStates[iScreen]= VRENA_SELECTED;
  Draw();    
  
  // send message to linux host, that display settings have changed
  return(ScreenChange(iMediaType, iScreen));
}

//--------------------------------------------------------------------------------
// Screen already has media content and is reactivated
BOOL CFScreens::ScreenTapped(int iScreen)
{
  switch(m_iMedia[iScreen]) {
    case 0:  // PPT
      ReturnToPPT(iScreen);
      m_theApp->SetMediaType(0);
      break;

    case 1:  // MONO-VIDEO
      // PLAY or PAUSE video if currently selected
      if (PlayPauseMovie(iScreen))
        m_bVideoPaused[iScreen]= !m_bVideoPaused[iScreen];
      break;

    case 2:  // PICS
      break;

    case 3:  // 3D
      ReturnTo3D();
      m_theApp->SetMediaType(3);
      return(TRUE);
      break;

    case 4:  // VR
      ReturnToVR();
      m_theApp->SetMediaType(4);
      return(TRUE);
      break;
      
    case 5:  // STEREO-VIDEO
      // PLAY or PAUSE video if currently selected
      if (PlayPauseMovie(iScreen))
        m_bVideoPaused[iScreen]= !m_bVideoPaused[iScreen];
      break;

    default:
      break;
  }
  
  for (int i=0; i<3; i++) {
    if (m_iStates[i] == VRENA_SELECTED) 
      m_iStates[i]= VRENA_RUNNING;  // formerly selected is now running
  }
  m_iStates[iScreen]= VRENA_SELECTED;
  m_iActiveScreen= iScreen;
  Draw();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::ScreenUnload(int iScreen, BOOL bSendToMatrix /* = TRUE */)
{

  switch(m_iMedia[iScreen]) {
    case 0:  // PPT
      if (!m_aPanelsPPT[iScreen]->ClosePPT()) return(FALSE);
      break;

    case 1:  // MONO-VIDEO
      if (!CloseMovie(iScreen)) return(FALSE);
      break;
    
    case 2:  // PICS
      if (!CloseMovie(iScreen)) return(FALSE);
      break;
    
    case 3:  // 3D
      break;

    case 4:  // VR
      break;

    case 5:  // STEREO-VIDEO
      if (!CloseMovie(iScreen)) return(FALSE);
      break;

    default:
      // should never happen!
      return(FALSE);
  }  
  
  m_iStates[iScreen]= VRENA_INACTIVE;
  m_iMedia[iScreen]= 4;  
  m_bVideoPaused[iScreen]= FALSE;
  
  if (bSendToMatrix) {
    // send message to MATRIX host, that display settings have changed
    return(ScreenChange(m_iMedia[iScreen], iScreen));
  
  } else {
    return(TRUE);
  }
}

//--------------------------------------------------------------------------------
BOOL CFScreens::StartPPT(int iScreen, char *pszFileName)
{
  for (int i=0; i<3; i++) m_aPanelsPPT[i]->SetVisible(FALSE);
  if (m_aPanelsPPT[iScreen]->StartPPT(pszFileName)) {
    m_theApp->ActivateControls(FALSE);
    m_theApp->EnableController(FALSE);
    m_aPanelsPPT[iScreen]->SetVisible(TRUE);
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::ReturnToPPT(int iScreen)
{
  for (int i=0; i<3; i++) m_aPanelsPPT[i]->SetVisible(FALSE);
  m_theApp->ActivateControls(FALSE);
  m_theApp->EnableController(FALSE);
  m_aPanelsPPT[iScreen]->SetVisible(TRUE);
  return(ScreenChange(1, iScreen));  // force matrix switch
}

//--------------------------------------------------------------------------------
BOOL CFScreens::OpenMovie(int iScreen, char *pszFileName, BOOL bMono)
{
  char szMsg[MAX_PATH + 1];

  // prepare the message (MONO or STEREO?)
  if (bMono) sprintf(szMsg, "M%s", pszFileName);
  else  sprintf(szMsg, "S%s", pszFileName);

  if (!m_theApp->SendToMovieHost(iScreen, szMsg)) return(FALSE);
  
  m_bVideoPaused[iScreen]= FALSE;  
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::PlayPauseMovie(int iScreen)
{
  char *pMsg= "X";
  return(m_theApp->SendToMovieHost(iScreen, pMsg));
}

//--------------------------------------------------------------------------------
BOOL CFScreens::CloseMovie(int iScreen)
{
  char *pMsg= "C";
  return(m_theApp->SendToMovieHost(iScreen, pMsg));
}

//--------------------------------------------------------------------------------
BOOL CFScreens::Start3D()
{
  // reset the tracker
  m_theApp->SendTrackerReset();

  for (int i=0; i<3; i++) {
    // unload previously selected content if not VR
    if ((m_iStates[i] != VRENA_INACTIVE)&&(m_iMedia[i] != 4)) ScreenUnload(i);
    m_iStates[i]= VRENA_SELECTED;
    m_iMedia[i]= 3;
    ScreenChange(4, i);
  }
  Draw();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::StartVR()
{
  // reset the tracker
  m_theApp->SendTrackerReset();
  ScreenChange(4, 3);  // TARGET 3 => ALL SCREENS!!!
  Draw();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::ReturnTo3D()
{
  for (int i=0; i<3; i++) {
    // reactivate all screens loaded with VR
    if (m_iStates[i] != VRENA_INACTIVE) {
      if (m_iMedia[i] == 3)
        m_iStates[i]= VRENA_SELECTED;
      else
        m_iStates[i]= VRENA_RUNNING;
    }
  }
  Draw();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFScreens::ReturnToVR()
{
  for (int i=0; i<3; i++) {
    // reactivate all screens loaded with VR
    if (m_iStates[i] != VRENA_INACTIVE) {
      if (m_iMedia[i] == 4)
        m_iStates[i]= VRENA_SELECTED;
      else
        m_iStates[i]= VRENA_RUNNING;
    }
  }
  m_theApp->ShowVR();

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Notifies the linux host about changes of media.
/// "iSource" specifies the medium (VIDEO, STEREO-VIDEO, VR)
///  0 ... PPT
///  1 ... Video
///  2 ... Picture
///  3 ... 3D (Modelview)
///  4 ... VR
///  5 ... Stereo-Video
/// "iTarget" specifies the screen on which the medium is displayed
BOOL CFScreens::ScreenChange(int iSource, int iTarget)
{
  char szMsg[16]= {0};
  char szTarget[2];
  
  // encode target
  _itoa(iTarget + 1, szTarget, 10);
  
  if (iSource < 3) {
    // PPT, Video & Pics can be treated equally    
    strcpy(szMsg, "MV");  // Mono-Video
    strcat(szMsg, szTarget);

  } else if ((iSource == 3)||(iSource == 4)) {
    // 3D and VR
    strcpy(szMsg, "VR");  // Virtual Reality
    strcat(szMsg, szTarget);
  
  } else if (iSource == 5) {  
    // Stereo-Video
    strcpy(szMsg, "SV");  // Stereo-Video
    strcat(szMsg, szTarget);    
  }

  return(m_theApp->SendToMatrixHost(szMsg));
}
