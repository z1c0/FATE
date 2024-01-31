// PPTRC.cpp: implementation of the CPPTRC class.
//////////////////////////////////////////////////////////////////////
#include "../../framework/include/fate.h"

#include "PPTRC.h"
#include "ppt_protocol.h"
#include "ids.h"
#include "resource.h"

//--------------------------------------------------------------------------------
#define FILE_MARK_DIR      '?'
#define FILE_MARK_CDROM    '<'
#define FILE_MARK_FLOPPY   '>'
#define FILE_MARK_HD       '|'


//--------------------------------------------------------------------------------
// Global function for creating the one and only application object.
CFateApp* CreateFateApp()
{
  return(new CPPTRC());
}


//--------------------------------------------------------------------------------
CPPTRC::CPPTRC() : CFateApp()
{
  m_bmpTitle  = NULL;
  m_btnClose  = NULL;
  m_panelPPT  = NULL;
  m_panelFiles= NULL;
  m_addrPPT   = NULL;
  m_pSockRecv = NULL;
  m_bIntro    = FALSE;  // TODO?
}

//--------------------------------------------------------------------------------
CPPTRC::~CPPTRC()
{
  SAFE_RELEASE(m_bmpTitle);
  SAFE_RELEASE(m_btnClose);
  SAFE_RELEASE(m_panelPPT);
  SAFE_RELEASE(m_panelFiles);
  SAFE_RELEASE(m_addrPPT);
  SAFE_RELEASE(m_pSockRecv);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::InitFateApp()
{
  TCHAR szPath[1024];

  // disable automatic suspension of app
  EnableSuspend(FALSE);
  
  // parse config file for IP configuration
  _tcscpy(szPath, m_app->GetAppPath());
  _tcscat(szPath, TEXT("IP.txt"));
  if (!ReadConfigFile(szPath)) {
    Error(TEXT("Configuration file could not be read!"));
    return(FALSE);
  }  
  
  // create a socket and bind it to port  
  m_pSockRecv= new CFSocket();
  m_pSockRecv->Create();
  m_pSockRecv->SetTimeout(0);
  m_pSockRecv->Bind(PPC_PORT);
  m_pSockRecv->Listen();

  // create title bar
  m_bmpTitle= new CFBitmap(m_hdc);
  if (!m_bmpTitle->Load(IDB_TITLE)) return(FALSE);

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
  Add(m_btnClose);

  // create PPT panel
  m_panelPPT= new CFPPTPanel();
  Add(m_panelPPT);
  if (!m_panelPPT->Create()) return(FALSE);
  m_panelPPT->SetY(m_bmpTitle->GetBottom());
  m_panelPPT->SetVisible(FALSE);

  // create panel for chosing files
  m_panelFiles= new CFFilePanel();
  Add(m_panelFiles);
  if (!m_panelFiles->Create()) return(FALSE);

  m_panelFiles->SetY(m_bmpTitle->GetBottom());
  m_panelFiles->SetVisible(TRUE);
  
  // init a timer for polling the receive socket
  SetTimer(m_hWnd, POLL_TIMER, 10, NULL);

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::ActivateFateApp()
{ 
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::CloseFateApp()
{
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CPPTRC::Draw()
{
  if (m_bIntro) DisplayIntro();

  ClearScreen(RGB(51, 102, 153));
  m_bmpTitle->Blit();  
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::StylusDown(int xPos, int yPos)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::StylusMove(int xPos, int yPos)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::StylusUp(int xPos, int yPos)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::KeyDown(int vkKey)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::KeyUp(int vkKey)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::ClosePresentation()
{
  m_panelPPT->ClosePPT();
  m_panelPPT->SetVisible(FALSE);
  m_panelFiles->ClearLabel();
  m_panelFiles->SetVisible(TRUE);
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::OpenPresentation()
{
  TCHAR *pszFilePathW;
  char *pszFilePath;
  int iLen;
  int i;

  pszFilePathW= m_panelFiles->GetFilePath();
  if (!pszFilePathW) return(FALSE);
  iLen= _tcslen(pszFilePathW) + 1;
  pszFilePath= (char*)malloc(iLen);
  for (i=0; i<iLen; i++) {
    pszFilePath[i]= (char)pszFilePathW[i];
  }
  if (m_panelPPT->StartPPT(pszFilePath)) {
    m_panelFiles->SetVisible(FALSE);
    m_panelPPT->SetVisible(TRUE);
  }
  free(pszFilePath);
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_BTN_QUITAPP:
      Exit();
      return(TRUE);
      break;

    case ID_BTN_CLOSEPPT:
      ClosePresentation();
      return(TRUE);
      break;

    case ID_BTN_LOAD:
      OpenPresentation();
      return(TRUE);
      break;

    default:
      break;
  }

  return(FALSE);
}

//--------------------------------------------------------------------------------
// Display a nice intro.
void CPPTRC::DisplayIntro()
{
  CFBitmap *bmpStart;
  CFBitmap *bmpAnim[11];
  TCHAR szPath[1024];
  TCHAR szFullPath[2048];
  TCHAR szBuff[8];
  
  // set path for bitmaps
  _tcscpy(szPath, m_app->GetAppPath());
  _tcscat(szPath, TEXT("intropics\\"));
  
  // default bitmap
  _tcscpy(szFullPath, szPath);
  _tcscat(szFullPath, TEXT("intro.bmp"));
  bmpStart= new CFBitmap(m_hdc);
  bmpStart->Load(szFullPath);
  for (int i=0; i<11; i++) {
    bmpAnim[i]= new CFBitmap(m_hdc);
    _tcscpy(szFullPath, szPath);
		_tcscat(szFullPath, TEXT("frame"));
		_itot(i, szBuff, 10);
		_tcscat(szFullPath, szBuff);
		_tcscat(szFullPath, TEXT(".bmp"));
    bmpAnim[i]->Load(szFullPath);
    bmpAnim[i]->SetX(75); 
    bmpAnim[i]->SetY(35);
  }

  bmpStart->Blit();
  DrawDoubleBuffer();

  for (i=0; i<11; i++) {
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
// Reads IP configuration data from the specified configuration file.
BOOL CPPTRC::ReadConfigFile(LPCTSTR pszFileName)
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
              if (!_stricmp(szHostName, "PPT")) {                
                m_addrPPT= new CFInetAddr(szHostAddr, atoi(szPort));
              } else {
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
// Used for error messages during initialization phase
void CPPTRC::Error(LPCTSTR pszErrMsg)
{
  MessageBox(m_hWnd, pszErrMsg, TEXT("Error"), MB_OK);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::SendToPPTHost(char *pMsg)
{
  CFSocket sock;
  char *szSendBuff;
  int iMsgLen;
  int iRet;
  int i;

  // establish connection first
  sock.SetTimeout(2);
  if (!sock.Create()) {
    return(FALSE);    
  }
  if (!sock.Connect(m_addrPPT)) {
    return(FALSE);    
  }

  // prepare message
  iMsgLen= strlen(pMsg);
  szSendBuff= (char*)LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT, 5 + iMsgLen + 1);

  i= 3;
  while(iMsgLen) {
    szSendBuff[i--]= (unsigned char)iMsgLen & 0x000000FF;
    iMsgLen>>= 8;
  }
  memcpy(&szSendBuff[4], pMsg, strlen(pMsg) + 1);
  
  // send message
  iRet= sock.Write(szSendBuff, strlen(pMsg) + 5);
  if ((iRet == SOCKET_TIMEOUT)||(iRet == SOCKET_ERROR)) {
    return(FALSE);
  } 
  
  sock.Close();
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::ReceiveFromPPTHost(CFSocket *sock) 
{
  char szRecvBuff1[4];
  char *pszRecvBuff2;
  char cCmd;
  DWORD dwRespLen;
  BOOL bRet= TRUE;
  int iRet;

  // first receive length of response
  iRet= sock->Receive(szRecvBuff1, 4);
  if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(FALSE);

  // bytes 0 ... 3 are length of message
  for (int i=0; i<4; i++) {
    dwRespLen|= (unsigned char)szRecvBuff1[i];
    dwRespLen<<= 8;
  }
  dwRespLen>>= 8;  // correction neeed

  // allocate memory for response
  iRet= 0;
  pszRecvBuff2= (char*)malloc(dwRespLen);
  memset(pszRecvBuff2, 0, dwRespLen);
  while (((UINT)iRet < dwRespLen)&&(iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
    iRet+= sock->Receive((char*)&pszRecvBuff2[iRet], dwRespLen - iRet);
  }
  if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(FALSE); 
  
  // byte 4 is protocol command
  cCmd= pszRecvBuff2[0]; 

  // react depending on protocol command
  switch (cCmd) {

    case ACK_COUNT:
    {
      // read number of slides from response
      char pszTempBuff[4];
      memcpy(pszTempBuff, &pszRecvBuff2[1], 4);
      m_panelPPT->SetSlideCount(atoi(pszTempBuff));
      break;
    }
    
    case ACK_PREV: 
    { 
      // store bitmap data
      m_panelPPT->StorePreview(&pszRecvBuff2[1], iRet - 1);
      break;
    }

    case ACK_NFO:
    {
      // read current slide number from response
      char pszTempBuff[4];
      memcpy(pszTempBuff, &pszRecvBuff2[1], 4);
      m_panelPPT->SetCurrSlide(atoi(pszTempBuff));
      m_panelPPT->Draw();
      break;
    }

    case ACK_FILEINFO:
    {
      char *pszTemp;
      TCHAR szTemp2[1024];
      TCHAR *pszExt;  // extension of file
      int iLen= 0;

      // first clear old list
      m_panelFiles->ClearItems();

      // extract PPT files
      //iLen= 5;
      pszTemp= &pszRecvBuff2[1];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, 1024);
#else
      strcpy(szTemp2, pszTemp);
#endif
      while(*pszTemp) {
        // get extension of file
        pszExt= _tcsrchr(szTemp2, '.');
        if ((pszExt)&&(((!_tcsicmp(pszExt, TEXT(".ppt")))||(!_tcsicmp(pszExt, TEXT(".pps")))))) {
          CFBitmap *bmpPPT= new CFBitmap(m_hdc);
          bmpPPT->Load(IDB_PPT_ICON);
          m_panelFiles->AddItem(bmpPPT, (_tcsrchr(szTemp2, '\\') + 1), szTemp2);

        } else {
          CFBitmap *bmp= NULL;
          switch(pszTemp[strlen(pszTemp) - 1]) {
            case FILE_MARK_DIR:
              bmp= new CFBitmap(m_hdc);
              bmp->Load(IDB_FOLDER_ICON);
              szTemp2[_tcslen(szTemp2) - 1]= 0;
              m_panelFiles->AddItem(bmp, (_tcsrchr(szTemp2, '\\') + 1), TEXT("?"));
              break;

            case FILE_MARK_FLOPPY:
              bmp= new CFBitmap(m_hdc);
              bmp->Load(IDB_FLOPPY_ICON);
              szTemp2[_tcslen(szTemp2) - 1]= 0;
              m_panelFiles->AddItem(bmp, szTemp2, TEXT("?"));
              break;

            case FILE_MARK_CDROM:
              bmp= new CFBitmap(m_hdc);
              bmp->Load(IDB_CDROM_ICON);
              szTemp2[_tcslen(szTemp2) - 1]= 0;
              m_panelFiles->AddItem(bmp, szTemp2, TEXT("?"));
              break;

            case FILE_MARK_HD:
              bmp= new CFBitmap(m_hdc);
              bmp->Load(IDB_HD_ICON);
              szTemp2[_tcslen(szTemp2) - 1]= 0;
              m_panelFiles->AddItem(bmp, szTemp2, TEXT("?"));
              break;

            default:
              break;
          }
        }
        
        iLen+= strlen(&pszRecvBuff2[iLen]);
        pszTemp= &pszRecvBuff2[++iLen];
#ifdef _WIN32_WCE
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, 1024);
#else
        strcpy(szTemp2, pszTemp);
#endif        
      } 
      m_panelFiles->Draw();
    }

    case ERR:
    {
      bRet= FALSE;
      break;
    }
  }

  free(pszRecvBuff2);
  return(bRet);
}

//--------------------------------------------------------------------------------
BOOL CPPTRC::ExtraEventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (wParam == POLL_TIMER) {
    // check for incoming data on socket
    CFSocket sock;
    int iRet;
    
    iRet= m_pSockRecv->Accept(sock);
    if (iRet != SOCKET_TIMEOUT) {
      ReceiveFromPPTHost(&sock);
    }
    return(TRUE);
  }
  return(FALSE);
}
