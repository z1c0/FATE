// FVRena.cpp: implementation of the CFVRena class.
//////////////////////////////////////////////////////////////////////
#include "FVRena.h"
#include "ids.h"
#include "ppt_protocol.h"
#include "protocol.h"
#include "resource.h"


//--------------------------------------------------------------------------------
// Global function for creating the one and only application object.
CFateApp* CreateFateApp()
{
  return(new CFVRena());
}


//--------------------------------------------------------------------------------
CFVRena::CFVRena() : CFateApp()
{
  // prepare socket connection to tracker host
  m_sockTrack= new CFUDPSocket();
  m_sockTrack->Create();
  m_iMediaType= 0;
  m_bIntro= FALSE;
  m_iListIndent= 10;
  m_iMaxItems= 9;
  m_bDropListEx= FALSE;
  m_iVrHostCount= 0;
  m_iModelHostCount= 0;
  m_bModelView= FALSE;
  m_bFirst= TRUE;
  m_labelInfo= NULL;
  m_labelTime= NULL;
  m_bmpDrag= NULL;
  m_iModelState= MODEL_STATE_ROT;
  
  // zero-initialize socket-addresses
  m_addrMatrix= NULL;
  m_addrTrack= NULL;
  memset(m_addrVR   , 0, MAX_RENDER_NODES * sizeof(CFInetAddr*));
  memset(m_addrModel, 0, MAX_RENDER_NODES * sizeof(CFInetAddr*));
  memset(m_addrVideo, 0, MAX_VIDEO_NODES  * sizeof(CFInetAddr*));
  
  // init states of the modelloader
  m_fModelSize= 1.0f;
  m_iModelRotX= 0;
  m_iModelRotZ= 0;
  m_bHave3DList= FALSE;
}

//--------------------------------------------------------------------------------
CFVRena::~CFVRena()
{
  delete(m_bmpTitle);
  delete(m_btnClose);
  for(int i=0; i<4; i++) delete(m_aItemLists[i]);
  for (i=0; i<MAX_VIDEO_NODES; i++) {
    SAFE_DELETE(m_addrVideo[i]);
    SAFE_DELETE(m_panelPPT[i]);
  }
  for (i=0; i<m_iVrHostCount; i++) SAFE_DELETE(m_addrVR[i]);
  for (i=0; i<m_iModelHostCount; i++) SAFE_DELETE(m_addrModel[i]);
  delete(m_addrMatrix);
  delete(m_addrTrack);
  delete(m_sockTrack);
  delete(m_screens);
  delete(m_panelVR);
  delete(m_panelCtls);
  delete(m_cont);
  delete(m_bmpPPT[0]);
  delete(m_bmpPPT[1]);
  delete(m_bmpVideo[0]);
  delete(m_bmpVideo[1]);
  delete(m_bmpPic[0]);
  delete(m_bmpPic[1]);
  delete(m_bmpVR[0]);
  delete(m_bmpVR[1]);
  delete(m_bmp3D[0]);
  delete(m_bmp3D[1]);
  SAFE_DELETE(m_labelInfo);
  SAFE_DELETE(m_labelTime);
  SAFE_DELETE(m_bmpDrag);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::InitFateApp()
{
  TCHAR szPath[1024];

  // disable automatic suspension of app
  EnableSuspend(FALSE);
  
  // set path for bitmaps
  _tcscpy(szPath, m_app->GetAppPath());

  // create title bar
  m_bmpTitle= new CFBitmap(m_hdc);
  if (!m_bmpTitle->Load(IDB_TITLE)) return(FALSE);
  
  // create bitmaps for switching tabs
  // PPT
  m_bmpPPT[0]= new CFBitmap(m_hdc);
  if (!m_bmpPPT[0]->Load(IDB_BUTTON_PPT)) return(FALSE);
  m_bmpPPT[0]->SetX(1);  
  m_bmpPPT[0]->SetY(m_app->GetHeight() - m_bmpPPT[0]->GetHeight());  
  m_bmpPPT[1]= new CFBitmap(m_hdc);
  if (!m_bmpPPT[1]->Load(IDB_BUTTON_PPT_P)) return(FALSE);
  m_bmpPPT[1]->SetX(m_bmpPPT[1]->GetX());  
  m_bmpPPT[1]->SetY(m_bmpPPT[0]->GetY());  
  
  // VIDEO
  m_bmpVideo[0]= new CFBitmap(m_hdc);
  if (!m_bmpVideo[0]->Load(IDB_BUTTON_VIDEO)) return(FALSE);
  m_bmpVideo[0]->SetX(m_bmpPPT[0]->GetRight() + 1);
  m_bmpVideo[0]->SetY(m_bmpPPT[0]->GetY());  
  m_bmpVideo[1]= new CFBitmap(m_hdc);
  if (!m_bmpVideo[1]->Load(IDB_BUTTON_VIDEO_P)) return(FALSE);
  m_bmpVideo[1]->SetX(m_bmpVideo[0]->GetX());
  m_bmpVideo[1]->SetY(m_bmpPPT[0]->GetY());  
  
  // PIC
  m_bmpPic[0]= new CFBitmap(m_hdc);  
  if (!m_bmpPic[0]->Load(IDB_BUTTON_PICS)) return(FALSE);
  m_bmpPic[0]->SetX(m_bmpVideo[1]->GetRight() + 1);
  m_bmpPic[0]->SetY(m_bmpPPT[0]->GetY());  
  m_bmpPic[1]= new CFBitmap(m_hdc);
  if (!m_bmpPic[1]->Load(IDB_BUTTON_PICS_P)) return(FALSE);
  m_bmpPic[1]->SetX(m_bmpPic[0]->GetX());
  m_bmpPic[1]->SetY(m_bmpPPT[0]->GetY());  
  
  // 3D
  m_bmp3D[0]= new CFBitmap(m_hdc);  
  if (!m_bmp3D[0]->Load(IDB_BUTTON_3D)) return(FALSE);
  m_bmp3D[0]->SetX(m_bmpPic[1]->GetRight() + 1);
  m_bmp3D[0]->SetY(m_bmpPPT[0]->GetY());  
  m_bmp3D[1]= new CFBitmap(m_hdc);
  if (!m_bmp3D[1]->Load(IDB_BUTTON_3D_P)) return(FALSE);
  m_bmp3D[1]->SetX(m_bmp3D[0]->GetX());
  m_bmp3D[1]->SetY(m_bmpPPT[0]->GetY());  
 
  // VR
  m_bmpVR[0]= new CFBitmap(m_hdc);
  if (!m_bmpVR[0]->Load(IDB_BUTTON_VR)) return(FALSE);
  m_bmpVR[0]->SetX(m_bmp3D[1]->GetRight() + 1);
  m_bmpVR[0]->SetY(m_bmpPPT[0]->GetY());  
  m_bmpVR[1]= new CFBitmap(m_hdc);
  if (!m_bmpVR[1]->Load(IDB_BUTTON_VR_P)) return(FALSE);
  m_bmpVR[1]->SetX(m_bmpVR[0]->GetX());
  m_bmpVR[1]->SetY(m_bmpPPT[0]->GetY());  

  // create a panel for logical aggregation of controls
  m_panelCtls= new CFPanel();
  m_panelCtls->SetVisible(TRUE);
  Add(m_panelCtls);

  // create close button
  CFBitmap *bmpClose= new CFBitmap(m_hdc);
  if (!bmpClose->Load(IDB_SKIP)) return(FALSE);
  CFBitmap *bmpCloseDis= new CFBitmap(m_hdc);
  if (!bmpCloseDis->Load(IDB_SKIP_DIS)) return(FALSE);
  m_btnClose= new CFButton(bmpClose, NULL, bmpCloseDis);
  m_btnClose->SetX(m_bmpTitle->GetRight() - m_btnClose->GetWidth() - 3);
  m_btnClose->SetY(3);
  m_btnClose->SetVisible(TRUE);
  m_btnClose->SetID(ID_BTN_QUITAPP);
  m_panelCtls->Add(m_btnClose);

  // create the item lists
  CFBitmap *aBmpUp[4];
  CFBitmap *aBmpDown[4];
  for (int i=0; i<4; i++) {
    aBmpUp[i]= new CFBitmap(m_hdc);
    aBmpDown[i]= new CFBitmap(m_hdc);
    if (!aBmpDown[i]->Load(IDB_DOWN_SCROLL)) return(FALSE);
    if (!aBmpUp[i]->Load(IDB_UP_SCROLL)) return(FALSE);
    m_aItemLists[i]= new CFItemList(m_iMaxItems, GetWidth() - m_iListIndent * 2 - aBmpDown[i]->GetWidth(), 
                                    aBmpUp[i], aBmpDown[i]);
    m_aItemLists[i]->SetVisible(FALSE);
    m_aItemLists[i]->SetX(m_iListIndent);
    m_aItemLists[i]->SetY(m_bmpTitle->GetBottom() + 10);
    m_panelCtls->Add(m_aItemLists[i]);
  }
  
  // assign IDs
  m_aItemLists[0]->SetVisible(TRUE);
  m_aItemLists[0]->SetID(ID_LIST_PPT);   
  m_aItemLists[1]->SetID(ID_LIST_VIDEO);
  m_aItemLists[2]->SetID(ID_LIST_PICS);
  m_aItemLists[3]->SetID(ID_LIST_EXT);
  // start with PPT list
  m_aItemLists[0]->SetVisible(TRUE);
  
  // create drag'n' drop bitmap
  m_bmpDrag= new CFBitmap(m_hdc);

  // create VR panel
  m_panelVR= new CFVRPanel();
  Add(m_panelVR);
  if (!m_panelVR->Create()) return(FALSE);
  m_panelVR->SetY(m_bmpTitle->GetBottom());
  m_panelVR->SetVisible(FALSE);

  // create PPT panels
  for (i=0; i<3; i++) {
    m_panelPPT[i]= new CFPPTPanel();
    Add(m_panelPPT[i]);
    if (!m_panelPPT[i]->Create()) return(FALSE);
    m_panelPPT[i]->SetY(m_bmpTitle->GetBottom());
    m_panelPPT[i]->SetVisible(FALSE);
    m_panelPPT[i]->SetScreenID(i);
  }
  
  // add the screens control
  m_screens= new CFScreens(m_panelPPT);
  m_panelCtls->Add(m_screens);
  if (!m_screens->Create()) return(FALSE);
  m_screens->SetY(214);
  m_screens->SetVisible(TRUE);

  // create a dummy controller
  m_cont= new CFController(NULL);
  
  // parse config file for IP configuration
  _tcscpy(szPath, m_app->GetAppPath());
  _tcscat(szPath, TEXT("IPs.txt"));
  if (!ReadConfigFile(szPath)) return(FALSE);

  // add controller
  Add(m_cont);
  m_cont->SetEnabled(TRUE);
  m_cont->StartSending(200);

  // create info label
  m_labelInfo= new CFLabel(TEXT(" Connecting to servers ..."), 200, 40);
  Add(m_labelInfo);
  m_labelInfo->SetX(20);
  m_labelInfo->SetY(70);  
  
  // create time label
  m_labelTime= new CFLabel(50, 20);
  Add(m_labelTime);
  m_labelTime->SetVisible(TRUE);
  m_labelTime->SetColText(RGB(255, 255, 255));
  m_labelTime->SetColBorder(RGB(51, 102, 153));
  m_labelTime->SetColBack(RGB(51, 102, 153));
  m_labelTime->SetX(160);
  m_labelTime->SetY(4);  

  SetTimer(m_hWnd, 0xCAFE, 500, NULL);
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ActivateFateApp()
{ 
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::CloseFateApp()
{
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFVRena::Draw()
{
  // intro needed?
  if (m_bIntro) DisplayIntro();

  ClearScreen(RGB(51, 102, 153));
  m_bmpTitle->Blit();
  m_labelTime->Draw();
  
  if (m_iMediaType == 0) m_bmpPPT[1]->Blit();
  else m_bmpPPT[0]->Blit();
  if (m_iMediaType == 1) m_bmpVideo[1]->Blit();
  else m_bmpVideo[0]->Blit();
  if (m_iMediaType == 2) m_bmpPic[1]->Blit();
  else m_bmpPic[0]->Blit();
  if (m_iMediaType == 3) m_bmp3D[1]->Blit();
  else m_bmp3D[0]->Blit();
  if (m_iMediaType == 4) m_bmpVR[1]->Blit();
  else m_bmpVR[0]->Blit();
  
  // fill item lists at first time
  if (m_bFirst) {
    m_bFirst= FALSE;
    m_labelInfo->Draw();
    DrawDoubleBuffer();
    // fill the item lists from server
    if (!FillItemListsFromServer()) {
      Error(TEXT("Retrieving file list from server failed!"));
    }
  }
}

//--------------------------------------------------------------------------------
BOOL CFVRena::StylusDown(int xPos, int yPos)
{
  // just save coordinates for possible drag operation
  m_iOldX= xPos;
  m_iOldY= yPos;
  
  if ((!m_bDropListEx)&&(yPos >= m_bmpPPT[0]->GetY())) {
    m_aItemLists[0]->SetVisible(FALSE);
    m_aItemLists[1]->SetVisible(FALSE);
    m_aItemLists[2]->SetVisible(FALSE);
    m_aItemLists[3]->SetVisible(FALSE);    

    if (m_bmpPPT[0]->PointInside(xPos, yPos)) {  // PPT
      m_bModelView= FALSE; // disable ModelViewer interaction
      m_panelPPT[0]->SetVisible(FALSE);
      m_panelPPT[1]->SetVisible(FALSE);
      m_panelPPT[2]->SetVisible(FALSE);
      m_panelVR->SetVisible(FALSE);
      m_panelCtls->SetVisible(TRUE);
      m_iMediaType= 0;
      m_aItemLists[0]->SetVisible(TRUE);
      return(TRUE);

    } else if (m_bmpVideo[0]->PointInside(xPos, yPos)) {  // VIDEO
      m_bModelView= FALSE; // disable ModelViewer interaction
      m_cont->SetEnabled(TRUE);   // activate TRACKD controller
      m_panelPPT[0]->SetVisible(FALSE);
      m_panelPPT[1]->SetVisible(FALSE);
      m_panelPPT[2]->SetVisible(FALSE);
      m_panelVR->SetVisible(FALSE);
      m_panelCtls->SetVisible(TRUE);
      m_iMediaType= 1;
      m_aItemLists[1]->SetVisible(TRUE);
      return(TRUE);

    } else if (m_bmpPic[0]->PointInside(xPos, yPos)) {  // PICS
      m_bModelView= FALSE; // disable ModelViewer interaction
      m_cont->SetEnabled(TRUE);   // activate TRACKD controller
      m_panelPPT[0]->SetVisible(FALSE);
      m_panelPPT[1]->SetVisible(FALSE);
      m_panelPPT[2]->SetVisible(FALSE);
      m_panelVR->SetVisible(FALSE);
      m_panelCtls->SetVisible(TRUE);
      m_iMediaType= 2;
      m_aItemLists[2]->SetVisible(TRUE);
      return(TRUE);

    } else if (m_bmp3D[0]->PointInside(xPos, yPos)) {  // 3D
      m_bModelView= TRUE; // enable ModelViewer interaction
      m_cont->SetEnabled(TRUE);   // activate TRACKD controller
      m_panelPPT[0]->SetVisible(FALSE);
      m_panelPPT[1]->SetVisible(FALSE);
      m_panelPPT[2]->SetVisible(FALSE);
      m_panelVR->SetVisible(FALSE);
      m_panelCtls->SetVisible(TRUE);
      m_iMediaType= 3;
      m_aItemLists[3]->SetVisible(TRUE);
      if ((!m_bHave3DList)&&(m_addrModel[0])) {  
        // receive 3D-file-list over TCP/IP connection if model loaders exits
        CFSocket sock;
        char szRecvBuff[4096];
        char *pszTemp;
        TCHAR szTemp2[1024];
        int iLen= 0;
        int iRet;

        sock.Create();
        if (!sock.Connect(m_addrModel[0])) return(FALSE);
        iRet= sock.Send(CMD_GET_MODELS, strlen(CMD_GET_MODELS));
        if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(FALSE); 
        iRet= sock.Receive(szRecvBuff, 4096);
        if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(FALSE); 
        sock.Close();
  
        // extract model files
        iLen= 0;
        pszTemp= szRecvBuff;
    #ifdef _WIN32_WCE
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, 1024);
    #else
        strcpy(szTemp2, pszTemp);
    #endif
        while(*pszTemp) {
          AddItem(3, szTemp2, szTemp2);
          iLen+= strlen(&szRecvBuff[iLen]);
          pszTemp= &szRecvBuff[++iLen];
    #ifdef _WIN32_WCE
          MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, 1024);
    #else
          strcpy(szTemp2, pszTemp);
    #endif
        }
        m_bHave3DList= TRUE;
      }      
      return(TRUE);

    } else if (m_bmpVR[0]->PointInside(xPos, yPos)) {  // VR
      m_bModelView= FALSE; // disable ModelViewer interaction
      m_cont->SetEnabled(TRUE);   // activate TRACKD controller
      m_panelPPT[0]->SetVisible(FALSE);
      m_panelPPT[1]->SetVisible(FALSE);
      m_panelPPT[2]->SetVisible(FALSE);
      m_iMediaType= 4;
      m_panelCtls->SetVisible(FALSE);
      m_screens->StartVR();
      m_panelVR->SetVisible(TRUE);
      return(TRUE);
    }
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFVRena::StylusMove(int xPos, int yPos)
{
  if (m_bDragMode) {
    DoDrag(xPos, yPos);
    return(TRUE);
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFVRena::KeyDown(PdaKey key)
{
  if (key ==  KeyFour) {  // BUTTON 4
    // reset signal for tracker
    SendTrackerReset();
    return(TRUE);
  }
  
  // if ModelViewer interaction enabled
  if (m_bModelView) {
    switch(key) {
      case KeyOne:  // BUTTON 1
        // rotation state
        m_iModelState= MODEL_STATE_ROT;
        break;

      case KeyTwo:  // BUTTON 2
        // sizing state
        m_iModelState= MODEL_STATE_SIZE;
        break;

      case KeyWest:  // LEFT
        // which state?
        if (m_iModelState == MODEL_STATE_ROT) {
          m_iModelRotZ-= ROTATION_STEP;
          if (m_iModelRotZ < 0) m_iModelRotZ+= 360;
          ModelRotate(ROT_Z, m_iModelRotZ);
        
        } else if (m_iModelState == MODEL_STATE_SIZE) {
          m_fModelSize-= 0.1f;
          if (m_fModelSize < 0.1f) m_fModelSize= 0.1f;
          ModelSize(m_fModelSize);
        }
        return(TRUE);

      case KeyEast:  // RIGHT
        if (m_iModelState == MODEL_STATE_ROT) {
          m_iModelRotZ+= ROTATION_STEP;
          if (m_iModelRotZ > 360) m_iModelRotZ-= 360;
          ModelRotate(ROT_Z, m_iModelRotZ);          
        
        } else if (m_iModelState == MODEL_STATE_SIZE) {
          m_fModelSize+= 0.1f;
          if (m_fModelSize > 5.f) m_fModelSize= 5.f;
          ModelSize(m_fModelSize);
        }

        return(TRUE);
    
      case KeyNorth:  // UP      
        if (m_iModelState == MODEL_STATE_ROT) {
          m_iModelRotX-= ROTATION_STEP;
          if (m_iModelRotX < 0) m_iModelRotX+= 360;
          ModelRotate(ROT_X, m_iModelRotX);
        
        } else if (m_iModelState == MODEL_STATE_SIZE) {
          m_fModelSize+= 0.1f;
          if (m_fModelSize > 5.f) m_fModelSize= 5.f;
          ModelSize(m_fModelSize);
        }
        return(TRUE);        

      case KeySouth:  // DOWN
        if (m_iModelState == MODEL_STATE_ROT) {
          m_iModelRotX+= ROTATION_STEP;
          if (m_iModelRotX > 360) m_iModelRotX-= 360;
          ModelRotate(ROT_X, m_iModelRotX);        
        
        } else if (m_iModelState == MODEL_STATE_SIZE) {
          m_fModelSize-= 0.1f;
          if (m_fModelSize < 0.1f) m_fModelSize= 0.1f;
          ModelSize(m_fModelSize);
        }
        return(TRUE);
    }
  }
   
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_BTN_QUITAPP:
      Exit();
      return(TRUE);
      break;

    case ID_BTN_CLOSEVR:
      m_iMediaType= 0;
      m_panelVR->SetVisible(FALSE);
      m_aItemLists[0]->SetVisible(FALSE);
      m_aItemLists[1]->SetVisible(FALSE);
      m_aItemLists[2]->SetVisible(FALSE);
      m_aItemLists[3]->SetVisible(FALSE);
      m_aItemLists[m_iMediaType]->SetVisible(TRUE);
      m_panelCtls->SetVisible(TRUE);
      return(TRUE);
      break;

    case ID_BTN_CLOSEPPT:
      m_iMediaType= 0;
      m_panelPPT[m_screens->GetActiveScreen()]->SetVisible(FALSE);
      m_aItemLists[0]->SetVisible(FALSE);
      m_aItemLists[1]->SetVisible(FALSE);
      m_aItemLists[2]->SetVisible(FALSE);
      m_aItemLists[3]->SetVisible(FALSE);
      m_aItemLists[m_iMediaType]->SetVisible(TRUE);
      m_cont->SetEnabled(TRUE);
      m_panelCtls->SetVisible(TRUE);
      return(TRUE);
      break;

    default:
      break;
  }

  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::DropListExpanded(DWORD dwListID)
{
  if (dwListID == ID_DROPLIST) {
    m_bDropListEx= TRUE;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::DropListCollapsed(DWORD dwListID)
{
  if (dwListID == ID_DROPLIST) {
    m_bDropListEx= FALSE;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
// Display a nice intro.
void CFVRena::DisplayIntro()
{
  CFBitmap *bmpStart;
  CFBitmap *bmpAnim[11];
  
  bmpStart= new CFBitmap(m_hdc);
  bmpStart->Load(IDB_INTRO);
  bmpAnim[ 0]= new CFBitmap(m_hdc);
  bmpAnim[ 0]->Load(IDB_FRAME0);
  bmpAnim[ 0]->SetX(75); 
  bmpAnim[ 0]->SetY(35);
  bmpAnim[ 1]= new CFBitmap(m_hdc);
  bmpAnim[ 1]->Load(IDB_FRAME1);
  bmpAnim[ 1]->SetX(75); 
  bmpAnim[ 1]->SetY(35);
  bmpAnim[ 2]= new CFBitmap(m_hdc);
  bmpAnim[ 2]->Load(IDB_FRAME2);
  bmpAnim[ 2]->SetX(75); 
  bmpAnim[ 2]->SetY(35);
  bmpAnim[ 3]= new CFBitmap(m_hdc);
  bmpAnim[ 3]->Load(IDB_FRAME3);
  bmpAnim[ 3]->SetX(75); 
  bmpAnim[ 3]->SetY(35);
  bmpAnim[ 4]= new CFBitmap(m_hdc);
  bmpAnim[ 4]->Load(IDB_FRAME4);
  bmpAnim[ 4]->SetX(75); 
  bmpAnim[ 4]->SetY(35);
  bmpAnim[ 5]= new CFBitmap(m_hdc);
  bmpAnim[ 5]->Load(IDB_FRAME5);
  bmpAnim[ 5]->SetX(75); 
  bmpAnim[ 5]->SetY(35);
  bmpAnim[ 6]= new CFBitmap(m_hdc);
  bmpAnim[ 6]->Load(IDB_FRAME6);
  bmpAnim[ 6]->SetX(75); 
  bmpAnim[ 6]->SetY(35);
  bmpAnim[ 7]= new CFBitmap(m_hdc);
  bmpAnim[ 7]->Load(IDB_FRAME7);
  bmpAnim[ 7]->SetX(75); 
  bmpAnim[ 7]->SetY(35);
  bmpAnim[ 8]= new CFBitmap(m_hdc);
  bmpAnim[ 8]->Load(IDB_FRAME8);
  bmpAnim[ 8]->SetX(75); 
  bmpAnim[ 8]->SetY(35);
  bmpAnim[ 9]= new CFBitmap(m_hdc);
  bmpAnim[ 9]->Load(IDB_FRAME9);
  bmpAnim[ 9]->SetX(75); 
  bmpAnim[ 9]->SetY(35);
  bmpAnim[10]= new CFBitmap(m_hdc);
  bmpAnim[10]->Load(IDB_FRAME10);
  bmpAnim[10]->SetX(75); 
  bmpAnim[10]->SetY(35);

  bmpStart->Blit();
  DrawDoubleBuffer();

  for (int i=0; i<11; i++) {
    bmpAnim[i]->Blit();
    DrawDoubleBuffer();
    Sleep(300);
  }
  Sleep(500);

  delete(bmpStart);
  for (i=0; i<11; i++) delete(bmpAnim[i]);
  m_bIntro= FALSE;
}

//--------------------------------------------------------------------------------
// Sets the entries for the item lists.
BOOL CFVRena::FillItemListsFromServer()
{
  // receive MEDIA-file-list over TCP/IP connection if address was specified
  if (m_addrVideo[0]) {  
    char szMsg[2];
  
    // prepare message
    szMsg[0]= CMD_FILEINFO;
    szMsg[1]= 0;
    if (!m_panelPPT[0]->SendToPPTHost(szMsg)) return(FALSE); 
  }
  return(TRUE);
}

//--------------------------------------------------------------------------------
// Reads IP configuration data from the specified configuration file.
BOOL CFVRena::ReadConfigFile(LPCTSTR pszFileName)
{
	FILE *configFile;
	int iRead;
	int i= 0;
	int iState= 0;
  char szReadBuff[2048];
  char cRead[2];  
  char szHostName[32];
  char szHostAddr[256];
  char szPort[8];

	
  cRead[1]   = 0;
	szHostName[0]= 0;
  szHostAddr[0]= 0;
  szPort[0]    = 0;
  if ((configFile= _tfopen(pszFileName, TEXT("rt"))) == NULL) {
    Error(TEXT("Could not open file \"IPs.txt\"."));
		return(FALSE);

  } else {
    iRead= fread(szReadBuff, sizeof(char), 2048, configFile);
    szReadBuff[iRead]= EOF;

    for (int i=0; i<=iRead; i++) {
      cRead[0]= szReadBuff[i];
      if ((cRead[0] != 32)&&(cRead[0] != 11)) {  // ignore white space
        switch(iState) {
          case 0:
            if (cRead[0] == ':') iState= 1;  // now read IP
            else strcat(szHostName, cRead);
            break;

          case 1:
            if (cRead[0] == ':') iState= 2;  // now read port
            else strcat(szHostAddr, cRead);
            break;

          case 2:
            if ((cRead[0] == '\n')||(cRead[0] == EOF)) {  // IP-settings complete
              iState= 0;
              if (!_stricmp(szHostName, "MEDIA1")) {                
                m_addrVideo[0]= new CFInetAddr(szHostAddr, atoi(szPort));
                m_panelPPT[0]->SetAddrPPT(new CFInetAddr(szHostAddr, atoi(szPort)));

              } else if (!_stricmp(szHostName, "MEDIA2")) {
                m_panelPPT[1]->SetAddrPPT(new CFInetAddr(szHostAddr, atoi(szPort)));
                m_addrVideo[1]= new CFInetAddr(szHostAddr, atoi(szPort));
              
              } else if (!_stricmp(szHostName, "MEDIA3")) {
                m_addrVideo[2]= new CFInetAddr(szHostAddr, atoi(szPort));
                m_panelPPT[2]->SetAddrPPT(new CFInetAddr(szHostAddr, atoi(szPort))); 

              } else if (!_stricmp(szHostName, "Matrix")) {
                m_addrMatrix= new CFInetAddr(szHostAddr, atoi(szPort));
              
              } else if (!_stricmp(szHostName, "VR")) {
                m_addrVR[m_iVrHostCount++]= new CFInetAddr(szHostAddr, atoi(szPort));
              
              } else if (!_stricmp(szHostName, "Model")) {
                m_addrModel[m_iModelHostCount++]= new CFInetAddr(szHostAddr, atoi(szPort));
              
              } else if (!_stricmp(szHostName, "TRACKD")) {
                m_cont->SetDestAddr(new CFInetAddr(szHostAddr, atoi(szPort)));
              
              } else if (!_stricmp(szHostName, "TRACKER_RESET")) {
                m_addrTrack= new CFInetAddr(szHostAddr, atoi(szPort));
              
              } else {
                // unknown entry found?
                TCHAR szErr[256];
                TCHAR szHost2[256];
#ifdef _WIN32_WCE
                MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szHostName, -1, szHost2, 256);
#else
                strcpy(szHost2, szHostName);
#endif

                _stprintf(szErr, TEXT("Unknown entry \"%s\" in file \"IPs.txt\" found."), szHost2);
                Error(szErr);
                return(FALSE);
              }
              // reset strings
              szHostName[0]= 0;
              szHostAddr[0]= 0;
              szPort[0]= 0;
            } else strcat(szPort, cRead);
            break;
        }
      }            
    }
  }
  fclose(configFile);  
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Used for error messages during initialization phase
void CFVRena::Error(LPCTSTR pszErrMsg)
{
  MessageBox(m_hWnd, pszErrMsg, TEXT("Error"), MB_OK);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ItemListSelected(DWORD dwListID, LPITEMLISTENTRY pEntry)
{  
  LPDRAGGINGINFO pDI;
  RECT rect;
  int iPos= pEntry->dwIndex - m_aItemLists[m_iMediaType]->GetItemPos();
  int iPosX, iPosY;
  int iWidth = m_aItemLists[0]->GetItemWidth();
  int iHeight= m_aItemLists[0]->GetItemHeight();
  
  // calculate upper left pixel of selected item
  iPosX= m_aItemLists[0]->GetX();
  iPosY= m_aItemLists[0]->GetY() + iPos * iHeight;
  
  // create bitmap for drag and drop
  m_bmpDrag->Create(iWidth, iHeight);
  m_bmpDrag->SolidFill(RGB(255, 0, 0));
  m_bmpDrag->SetX(iPosX);
  m_bmpDrag->SetY(iPosY);
  rect.left= 0; rect.top= 0;
  rect.right= iWidth; rect.bottom= iHeight;
  SetTextColor(m_bmpDrag->GetSourceDC(), RGB(255, 255, 255));
  SetBkColor(m_bmpDrag->GetSourceDC(), RGB(255, 0, 0));
  DrawText(m_bmpDrag->GetSourceDC(), pEntry->pszItem, _tcslen(pEntry->pszItem), &rect, 
           DT_SINGLELINE|DT_LEFT|DT_VCENTER);
  
  // start drag and drop operation
  StartDragMode(m_bmpDrag, m_iOldX, m_iOldY);
  pDI= m_app->GetDragInfo();
  pDI->strDesc= pEntry->pszAddInfo;

  return(TRUE);
}

//--------------------------------------------------------------------------------
bool CFVRena::SendToMediaHost(int iScreen, char *pMsg)
{
  CFSocket sock;
  int iLen= strlen(pMsg) + 1;

  // send to mono video first
  if (m_addrVideo[iScreen]) {
    // establish connection
    if (sock.Create()) {
      if (sock.Connect(m_addrVideo[iScreen])) {
        // send message
        sock.Write(pMsg, iLen);
        sock.Close();
      }
    }
  }

  return(true);
}

//--------------------------------------------------------------------------------
bool CFVRena::SendToMatrixHost(char *pMsg)
{
  CFSocket sock;
  int iRet;

  // check if socket address was specified?
  if (m_addrMatrix) {
    // establish connection first
    if (!sock.Create()) return(false);    
    if (!sock.Connect(m_addrMatrix)) return(false);
  
    // send message
    iRet= sock.Send(pMsg, strlen(pMsg));
    if ((iRet == SOCKET_TIMEOUT)||(iRet == SOCKET_ERROR)) return(false);
    sock.Close();
    return(true);
  }
  return(true);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::SendCaveMenuChange(int iNr)
{
  char szSendBuff[16];
  int iSent= 0;
  CFSocket sock;

	// iterate through hosts and inform them about menuchange
  for (int i=0; i<m_iVrHostCount; i++) {
    if (!sock.Create()) return(FALSE);
    if (!sock.Connect(m_addrVR[i])) return(FALSE);

    sprintf(szSendBuff, "%d\n", iNr);
    sock.Write(szSendBuff, strlen(szSendBuff) + 1);
    sock.Close();
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ModelLoad(char *pszModel)
{
  char szSendBuff[256];
  int iSent= 0;
  CFSocket sock;

	// iterate through hosts and inform them about the model to load
  for (int i=0; i<m_iModelHostCount; i++) {
    if (!sock.Create()) return(FALSE);
    if (!sock.Connect(m_addrModel[i])) return(FALSE);

    sprintf(szSendBuff, "obj|file(\"%s\")\n", pszModel);
    sock.Write(szSendBuff, strlen(szSendBuff) + 1);
    sock.Close();
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ModelSize(float fSize)
{
  char szSendBuff[256];
  int iSent= 0;
  CFSocket sock;

	// iterate through hosts and inform them about resizing of model
  for (int i=0; i<m_iModelHostCount; i++) {
    if (!sock.Create()) return(FALSE);
    if (!sock.Connect(m_addrModel[i])) return(FALSE);

    sprintf(szSendBuff, "sizer|size(%f)\n", fSize);
    sock.Write(szSendBuff, strlen(szSendBuff) + 1);
    sock.Close();
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ModelRotate(int iAxis, int iAngle)
{
  char szSendBuff[256];
  int iSent= 0;
  CFSocket sock;

	// iterate through hosts and inform them about rotation of the model
  for (int i=0; i<m_iModelHostCount; i++) {
    if (!sock.Create()) return(FALSE);
    if (!sock.Connect(m_addrModel[i])) return(FALSE);

    // which axis?
    if (iAxis == ROT_X)
      sprintf(szSendBuff, "xrotor|orientation(%d, 0, 0)\n", iAngle);
    else 
      sprintf(szSendBuff, "zrotor|orientation(0, 0, %d)\n", iAngle);

    sock.Write(szSendBuff, strlen(szSendBuff) + 1);
    sock.Close();
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::SendTrackerReset()
{
  m_sockTrack->Send("RESET", 6, m_addrTrack);

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFVRena::AddItem(int iNr, TCHAR *pszItem, TCHAR *pszAddInfo)
{
  return(m_aItemLists[iNr]->AddItem(pszItem, pszAddInfo));
}

//--------------------------------------------------------------------------------
BOOL CFVRena::ExtraEventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (message == WM_TIMER) {
    // clock timer
    if (wParam == 0xCAFE) {
      TCHAR szTime[32];

      GetLocalTime(&m_time);
      _stprintf(szTime, TEXT("%02d:%02d"), m_time.wHour, m_time.wMinute);
      m_labelTime->SetText(szTime);

      return(TRUE);
    }
    return(FALSE);
  }

  return(FALSE);
}
