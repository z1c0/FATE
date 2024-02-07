// PPTPanel.cpp: implementation of the CPPTPanel class.
//////////////////////////////////////////////////////////////////////
#include "FPPTPanel.h"
#include "ids.h"
#include "ppt_protocol.h"
#include "FVRena.h"
#include "resource.h"

//--------------------------------------------------------------------------------
CFPPTPanel::CFPPTPanel() : CFPanel()
{
  m_iMaxItems= 4;
  m_iItemWidth= 70;
  m_bmpPreviews= NULL;
  m_iPrevWidth = 220;
  m_iPrevHeight= 165;
  m_addrPPT= NULL;
}

//--------------------------------------------------------------------------------
CFPPTPanel::~CFPPTPanel()
{
  SAFE_DELETE(m_dropList);
  SAFE_DELETE(m_btnClose);
  SAFE_DELETE(m_bmpCurr);
  SAFE_DELETE(m_bmpNext);
  SAFE_DELETE(m_bmpSlide);
  SAFE_DELETE(m_btnNextSlide);
  SAFE_DELETE(m_btnPrevSlide);
  SAFE_DELETE(m_btnNextLink);
  SAFE_DELETE(m_btnPrevLink);
  SAFE_DELETE(m_btnActLink);
  SAFE_DELETE(m_btnBlack);
  SAFE_DELETE(m_btnWhite);
  SAFE_DELETE(m_addrPPT);
  DeletePreviews();
}

//--------------------------------------------------------------------------------
void CFPPTPanel::Draw()
{
  m_btnClose->Draw();
  if (m_bShowCurr) {
    DrawCurrent();
    m_bmpCurr->TransBlit(RGB(255, 0, 51));
    m_dropList->SetSelectedItem(m_iCurrSlide - 1);
    m_dropList->Draw();
  } else {
    DrawNext();
    m_bmpNext->TransBlit(RGB(255, 0, 51));
  }
  m_btnClose->Draw();
  m_bmpSlide->Blit();
  m_btnNextSlide->Draw();
  m_btnPrevSlide->Draw();
  m_btnActLink->Draw();
  m_btnNextLink->Draw();
  m_btnPrevLink->Draw();
  m_btnBlack->Draw();
  m_btnWhite->Draw();
}

//--------------------------------------------------------------------------------
void CFPPTPanel::DrawCurrent()
{
  char szMsg[6];
  BOOL bOK= TRUE;

  // if preview for current slide not ready, receive from server
  if (!m_bmpPreviews[m_iCurrSlide - 1]) {
    memset(szMsg, 0, 6);
    szMsg[0]= CMD_PREVIEW;
    sprintf(&szMsg[1], "%d", m_iCurrSlide);
    if((bOK= SendToPPTHost(szMsg))) {
      // image data was sent, create image
      m_bmpPreviews[m_iCurrSlide - 1]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
      m_bmpPreviews[m_iCurrSlide - 1]->Load(m_pPrevData, m_dwPrevSize);
    }
  }    
  // draw preview for current slide
  if (bOK) {
    m_bmpPreviews[m_iCurrSlide - 1]->SetX(10);
    m_bmpPreviews[m_iCurrSlide - 1]->SetY(50);
    m_bmpPreviews[m_iCurrSlide - 1]->Blit();  
  }
}
  
//--------------------------------------------------------------------------------
void CFPPTPanel::DrawNext()
{
  char szMsg[6];
  int iNextIndex;
  BOOL bOK= TRUE;

  // if preview for next slide not ready, receive from server
  iNextIndex= m_iCurrSlide + 1 <= m_iSlideCount ? m_iCurrSlide + 1 : 1;
  if (!m_bmpPreviews[iNextIndex - 1]) {
    memset(szMsg, 0, 6);
    szMsg[0]= CMD_PREVIEW;
    sprintf(&szMsg[1], "%d", iNextIndex);
    if((bOK= SendToPPTHost(szMsg))) {
      // image data was sent, create image
      m_bmpPreviews[iNextIndex - 1]= new CFBitmap(*m_pSystem->GetDoubleBuffer());
      m_bmpPreviews[iNextIndex - 1]->Load(m_pPrevData, m_dwPrevSize);
    }
  }  
  // draw preview for next slide
  if (bOK) {
    m_bmpPreviews[iNextIndex - 1]->SetX(10);
    m_bmpPreviews[iNextIndex - 1]->SetY(50);
    m_bmpPreviews[iNextIndex - 1]->StretchBlit(m_iPrevWidth, m_iPrevHeight);  
  }
}

//--------------------------------------------------------------------------------
BOOL CFPPTPanel::Create()
{
  // slides icon
  m_bmpSlide= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpSlide->Load(IDB_BUTTON_FOLIEN)) return(FALSE);
  
  // current slide "window"
  m_bmpCurr= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpCurr->Load(IDB_CURRENT_SLIDE)) return(FALSE);
  
  // next slide "window"
  m_bmpNext= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpNext->Load(IDB_NEXT_SLIDE)) return(FALSE);

  // close button
  CFBitmap *bmpClose= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpClose->Load(IDB_SKIP_SMALL)) return(FALSE);
  m_btnClose= new CFButton(bmpClose);
  m_btnClose->SetVisible(TRUE);
  m_btnClose->SetId(ID_BTN_CLOSEPPT);
  Add(*m_btnClose);

  // create droplist
  CFBitmap *bmpDrop= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDrop->Load(IDB_DROP_ARROW)) return(FALSE);
  CFBitmap *bmpUp= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpUp->Load(IDB_UP_SCROLL)) return(FALSE);
  CFBitmap *bmpDown= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDown->Load(IDB_DOWN_SCROLL)) return(FALSE);
  m_dropList= new CFDropList(m_iMaxItems, m_iItemWidth, bmpDrop, bmpUp, bmpDown);
  m_dropList->SetVisible(TRUE);
  m_dropList->SetId(ID_DROPLIST);    
  Add(*m_dropList);

  // next-slide button
  CFBitmap *bmpNextSlide= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpNextSlide->Load(IDB_BUTTON_NEXT_S)) return(FALSE);
  CFBitmap *bmpNextSlidePressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpNextSlidePressed->Load(IDB_BUTTON_NEXT_S_P)) return(FALSE);
  m_btnNextSlide= new CFButton(bmpNextSlide, bmpNextSlidePressed);
  m_btnNextSlide->SetVisible(TRUE);
  m_btnNextSlide->SetId(ID_BTN_NEXTSLIDE);
  Add(*m_btnNextSlide);
  
  // prev-slide button
  CFBitmap *bmpPrevSlide= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPrevSlide->Load(IDB_BUTTON_PREV_S)) return(FALSE);
  CFBitmap *bmpPrevSlidePressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPrevSlidePressed->Load(IDB_BUTTON_PREV_S_P)) return(FALSE);
  m_btnPrevSlide= new CFButton(bmpPrevSlide, bmpPrevSlidePressed);
  m_btnPrevSlide->SetVisible(TRUE);
  m_btnPrevSlide->SetId(ID_BTN_PREVSLIDE);
  Add(*m_btnPrevSlide);
  
  // next-link button
  CFBitmap *bmpNextLink= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpNextLink->Load(IDB_BUTTON_NEXT_HL)) return(FALSE);
  CFBitmap *bmpNextLinkPressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpNextLinkPressed->Load(IDB_BUTTON_NEXT_HL_P)) return(FALSE);
  m_btnNextLink= new CFButton(bmpNextLink, bmpNextLinkPressed);
  m_btnNextLink->SetVisible(TRUE);
  m_btnNextLink->SetId(ID_BTN_NEXTLINK);
  Add(*m_btnNextLink);
  
  // prev-link button  
  CFBitmap *bmpPrevLink= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPrevLink->Load(IDB_BUTTON_PREV_HL)) return(FALSE);
  CFBitmap *bmpPrevLinkPressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPrevLinkPressed->Load(IDB_BUTTON_PREV_HL_P)) return(FALSE);
  m_btnPrevLink= new CFButton(bmpPrevLink, bmpPrevLinkPressed);
  m_btnPrevLink->SetVisible(TRUE);
  m_btnPrevLink->SetId(ID_BTN_PREVLINK);
  Add(*m_btnPrevLink);
  
  // activate-link button  
  CFBitmap *bmpActLink= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpActLink->Load(IDB_BUTTON_HYPER)) return(FALSE);
  CFBitmap *bmpActLinkPressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpActLinkPressed->Load(IDB_BUTTON_HYPER_P)) return(FALSE);
  m_btnActLink= new CFButton(bmpActLink, bmpActLinkPressed);
  m_btnActLink->SetVisible(TRUE);
  m_btnActLink->SetId(ID_BTN_ACTLINK);
  Add(*m_btnActLink);
  
  // black button  
  CFBitmap *bmpBlack= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpBlack->Load(IDB_BUTTON_BLACK)) return(FALSE);
  CFBitmap *bmpBlackPressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpBlackPressed->Load(IDB_BUTTON_BLACK_P)) return(FALSE);
  m_btnBlack= new CFButton(bmpBlack, bmpBlackPressed);
  m_btnBlack->SetVisible(TRUE);
  m_btnBlack->SetId(ID_BTN_BLACK);
  Add(*m_btnBlack);
  
  // white button  
  CFBitmap *bmpWhite= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpWhite->Load(IDB_BUTTON_WHITE)) return(FALSE);
  CFBitmap *bmpWhitePressed= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpWhitePressed->Load(IDB_BUTTON_WHITE_P)) return(FALSE);
  m_btnWhite= new CFButton(bmpWhite, bmpWhitePressed);
  m_btnWhite->SetVisible(TRUE);
  m_btnWhite->SetId(ID_BTN_WHITE);
  Add(*m_btnWhite);
  
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
bool CFPPTPanel::StylusDown(int xPos, int yPos)
{
  // switch between current and next slide?
  if ((!m_bShowCurr)&&(xPos >= m_rectTab[0].left)&&(xPos <= m_rectTab[0].right)&&
      (yPos >= m_rectTab[0].top)&&(yPos <= m_rectTab[0].bottom)) {
    m_bShowCurr= TRUE;
    m_dropList->SetVisible(TRUE);
    return(TRUE);
  
  } else if ((m_bShowCurr)&&(xPos >= m_rectTab[1].left)&&(xPos <= m_rectTab[1].right)&&
             (yPos >= m_rectTab[1].top)&&(yPos <= m_rectTab[1].bottom)) {
    m_bShowCurr= FALSE;
    m_dropList->SetVisible(FALSE);
    return(TRUE);
  }

  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFPPTPanel::KeyDown(PdaKey key)
{
  switch(key) {
    case KeyWest:   // LEFT
      PrevSlide();      
      return(TRUE);

    case KeyEast:   // RIGHT
      NextSlide();
      return(TRUE);
    
    case KeyNorth:   // UP      
      NextLink();
      return(TRUE);

    case KeySouth:   // DOWN
      PrevLink();
      return(TRUE);
    
    case KeyEnter:  // RETURN
      ActivateLink();
      return(TRUE);
      break;

    default:
      break;
  }
  return(FALSE);  // event not handled
}


//--------------------------------------------------------------------------------
void CFPPTPanel::UpdatePos()
{
  if (m_pSystem) {
    m_bmpCurr->SetX(m_iPosX);
    m_bmpCurr->SetY(m_iPosY);
    m_bmpNext->SetX(m_iPosX);
    m_bmpNext->SetY(m_iPosY);
    m_btnClose->SetX(m_bmpCurr->GetRight() - m_btnClose->GetWidth());
    m_btnClose->SetY(m_iPosY + 3);

    m_dropList->SetX(m_iPosX + 20);
    m_dropList->SetY(m_iPosY + 183);
    
    m_btnPrevSlide->SetX(m_iPosX + 42);
    m_btnPrevSlide->SetY(m_bmpCurr->GetBottom() - m_btnPrevSlide->GetHeight() - 6);
    m_btnNextSlide->SetX(m_btnPrevSlide->GetRight() + 1);
    m_btnNextSlide->SetY(m_btnPrevSlide->GetY());
    m_bmpSlide->SetX(m_btnPrevSlide->GetX() - m_bmpSlide->GetWidth() - 2);
    m_bmpSlide->SetY(m_btnPrevSlide->GetY() + 5);
    
    m_btnNextLink->SetX(m_iPosX + 120);
    m_btnNextLink->SetY(m_btnPrevSlide->GetY() + 1);
    m_btnPrevLink->SetX(m_btnNextLink->GetX() + 1);
    m_btnPrevLink->SetY(m_btnNextLink->GetBottom() + 1);
    m_btnActLink->SetX(m_btnPrevLink->GetX() - m_btnActLink->GetWidth() - 2);
    m_btnActLink->SetY(m_btnNextLink->GetY());

    m_btnBlack->SetX(m_iPosX + 170);
    m_btnBlack->SetY(m_btnPrevSlide->GetY() + 5);
    m_btnWhite->SetX(m_btnBlack->GetRight() + 1);
    m_btnWhite->SetY(m_btnBlack->GetY());

    m_rectTab[0].left  = m_iPosX +   0;
    m_rectTab[0].top   = m_iPosY + 205;
    m_rectTab[0].right = m_iPosX + 119;
    m_rectTab[0].bottom= m_iPosY + 220;
    m_rectTab[1].left  = m_iPosX + 120;
    m_rectTab[1].top   = m_iPosY + 205;
    m_rectTab[1].right = m_iPosX + 240;
    m_rectTab[1].bottom= m_iPosY + 220;
  }
}

//--------------------------------------------------------------------------------
bool CFPPTPanel::ButtonReleased(DWORD dwBtnID)
{
  if (!m_bEnabled) return(FALSE);

  switch(dwBtnID) {
    case ID_BTN_NEXTSLIDE:
      NextSlide();
      return(TRUE);
      break;

    case ID_BTN_PREVSLIDE:
      PrevSlide();
      return(TRUE);
      break;

    case ID_BTN_NEXTLINK:
      NextLink();
      return(TRUE);
      break;

    case ID_BTN_PREVLINK:
      PrevLink();
      return(TRUE);
      break;

    case ID_BTN_ACTLINK:       
      ActivateLink();
      return(TRUE);
      break;

    case ID_BTN_BLACK:       
      BlackScreen();
      return(TRUE);
      break;

    case ID_BTN_WHITE:       
      WhiteScreen();
      return(TRUE);
      break;

    default:
      break;
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
bool CFPPTPanel::DropListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry)
{
  if (!m_bEnabled) return(FALSE);

  switch(dwListID) {
    case ID_DROPLIST:      
      GotoSlide(pEntry->ulIndex + 1);
      return(TRUE);
      break;

    default:
      break;
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFPPTPanel::StartPPT(char *pszFileName)
{
  BOOL bRet;
  char szMsg[1024];

  szMsg[0]= CMD_OPEN;
  szMsg[1]= 0;
  strcat(szMsg, pszFileName);
  
  // delete old previews if existing
  DeletePreviews();
  m_bShowCurr= TRUE;  // always start with showing the current screen
  m_iCurrSlide= 1;

  bRet= SendToPPTHost(szMsg);

  /*if (bRet) {
    // set pen mode
    szMsg[0]= CMD_PEN;
    szMsg[1]= 0;
    SendToPPTHost(szMsg);
  }*/
  return(bRet);
}

//--------------------------------------------------------------------------------
void CFPPTPanel::NextSlide()
{
  char szMsg[2];

  szMsg[0]= CMD_NEXT;
  szMsg[1]= 0;

  if (SendToPPTHost(szMsg)) {
    // information about current slide was sent, set preview pictures
    Draw();
  }
}

//--------------------------------------------------------------------------------
void CFPPTPanel::PrevSlide()
{
  char szMsg[2];

  szMsg[0]= CMD_PREV;
  szMsg[1]= 0;

  if (SendToPPTHost(szMsg)) {
    // information about current slide was sent, set preview pictures
    Draw();
  }
}

//--------------------------------------------------------------------------------
void CFPPTPanel::GotoSlide(DWORD dwNr)
{
  char szMsg[128];

  szMsg[0]= CMD_GOTO;
  _itoa(dwNr, &szMsg[1], 10);

  if (SendToPPTHost(szMsg)) {
    // information about current slide was sent, set preview pictures
    Draw();
  }
}

//--------------------------------------------------------------------------------
void CFPPTPanel::BlackScreen()
{
  char szMsg[2];

  szMsg[0]= CMD_BLACK;
  szMsg[1]= 0;  
  SendToPPTHost(szMsg);  
}

//--------------------------------------------------------------------------------
void CFPPTPanel::WhiteScreen()
{
  char szMsg[2];

  szMsg[0]= CMD_WHITE;
  szMsg[1]= 0;  
  SendToPPTHost(szMsg);
}

//--------------------------------------------------------------------------------
void CFPPTPanel::NextLink()
{
  char szMsg[2];

  szMsg[0]= CMD_NEXTHL;
  szMsg[1]= 0;  
  SendToPPTHost(szMsg);
}

//--------------------------------------------------------------------------------
void CFPPTPanel::PrevLink()
{
  char szMsg[2];

  szMsg[0]= CMD_PREVHL;
  szMsg[1]= 0;  
  SendToPPTHost(szMsg);
}

//--------------------------------------------------------------------------------
void CFPPTPanel::ActivateLink()
{
  char szMsg[2];

  szMsg[0]= CMD_CLICKHL;
  szMsg[1]= 0;  
  SendToPPTHost(szMsg);
}

//--------------------------------------------------------------------------------
BOOL CFPPTPanel::ClosePPT()
{
  char szMsg[2];

  szMsg[0]= CMD_CLOSE;
  szMsg[1]= 0;

  return(SendToPPTHost(szMsg));
}

//--------------------------------------------------------------------------------
BOOL CFPPTPanel::SendToPPTHost(char *pMsg)
{
  CFSocket sock;
  char *szSendBuff;
  int iMsgLen;
  int iRet;

  // prepare message
  iMsgLen= strlen(pMsg);
  szSendBuff= (char*)LocalAlloc(LMEM_FIXED|LMEM_ZEROINIT, iMsgLen + 5);

  // store length of message
  memcpy(szSendBuff, (char*)&iMsgLen, 4);
  // actual message
  memcpy(&szSendBuff[4], pMsg, strlen(pMsg) + 1);

  // establish connection first
  if (!sock.Create()) return(FALSE);    

  if (!sock.Connect(m_addrPPT)) return(FALSE);
  
  // send message
  iRet= sock.Write(szSendBuff, strlen(pMsg) + 5);
  if ((iRet == SOCKET_TIMEOUT)||(iRet == SOCKET_ERROR)) 
    return(FALSE);
  
  // receive answer from host
  if (!ReceiveFromPPTHost(&sock)) 
    return(FALSE);
  
  sock.Close();
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFPPTPanel::ReceiveFromPPTHost(CFSocket* sock) 
{
  unsigned char szLenBuff[4];
  char *pszRecvBuff2;
  char cCmd;
  DWORD dwRespLen= 0;
  BOOL bRet= TRUE;
  int iRet;

  // first receive length of response
  iRet= sock->Receive((char*)szLenBuff, 4);
  if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) {
    return(FALSE);
  }
  for (int i=0; i<4; i++) {
    dwRespLen<<= 8;
    dwRespLen|= szLenBuff[i];
  }

  // allocate memory for response
  iRet= 0;
  pszRecvBuff2= (char*)malloc(dwRespLen);
  memset(pszRecvBuff2, 0, dwRespLen);
  while (((UINT)iRet < dwRespLen)&&(iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
    iRet+= sock->Receive((char*)&pszRecvBuff2[iRet], dwRespLen - iRet);
  }
  if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) 
    return(FALSE); 
  
  // byte 4 is protocol command
  cCmd= pszRecvBuff2[0]; 

  // react depending on protocol command
  switch (cCmd) {

    case ACK_COUNT:
    {
      // read number of slides from response
      char pszTempBuff[4];
      memcpy(pszTempBuff, &pszRecvBuff2[1], 4);
      SetSlideCount(atoi(pszTempBuff));
      break;
    }
    
    case ACK_PREV: 
    { 
      // store bitmap data
      m_dwPrevSize= iRet - 1;
      m_pPrevData= (char*)malloc(m_dwPrevSize);
      memcpy(m_pPrevData, &pszRecvBuff2[1], m_dwPrevSize);
      break;
    }

    case ACK_NFO:
    {
      // read current slide number from response
      char pszTempBuff[4];
      memcpy(pszTempBuff, &pszRecvBuff2[1], 4);
      m_iCurrSlide= atoi(pszTempBuff);
      break;
    }

    case ACK_FILEINFO:
    {
      char *pszTemp;
      TCHAR szTemp2[MAX_PATH];
      TCHAR *pszExt;  // extension of file
      int iLen= 0;
      CFVRena *m_theApp= (CFVRena*)m_app;
    
      // extract PPT files
      iLen= 5;
      pszTemp= &pszRecvBuff2[1];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      while(*pszTemp) {
        // get extension of file
        pszExt= _tcsrchr(szTemp2, '.');
        if ((pszExt)&&(((!_tcsicmp(pszExt, TEXT(".ppt")))||(!_tcsicmp(pszExt, TEXT(".pps"))))))
          m_theApp->AddItem(0, (_tcsrchr(szTemp2, '\\') + 1), szTemp2);
        iLen+= strlen(&pszRecvBuff2[iLen]);
        pszTemp= &pszRecvBuff2[++iLen];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      }

      // extract Video files
      iLen= 5;
      pszTemp= &pszRecvBuff2[1];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      while(*pszTemp) {
        // get extension of file
        pszExt= _tcsrchr(szTemp2, '.');
        if ((pszExt)&&(((!_tcsicmp(pszExt, TEXT(".avi")))||(!_tcsicmp(pszExt, TEXT(".qt")))||(!_tcsicmp(pszExt, TEXT(".mov")))||(!_tcsicmp(pszExt, TEXT(".mpg")))||(!_tcsicmp(pszExt, TEXT(".mpeg")))||(!_tcsicmp(pszExt, TEXT(".m1v")))||(!_tcsicmp(pszExt, TEXT(".asf")))||(!_tcsicmp(pszExt, TEXT(".wma")))||(!_tcsicmp(pszExt, TEXT(".wmv"))))))
          m_theApp->AddItem(1, (_tcsrchr(szTemp2, '\\') + 1), szTemp2);
        iLen+= strlen(&pszRecvBuff2[iLen]);
        pszTemp= &pszRecvBuff2[++iLen];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      }       
 
      // extract Picture files
      iLen= 5;
      pszTemp= &pszRecvBuff2[1];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      while(*pszTemp) {
        // get extension of file
        pszExt= _tcsrchr(szTemp2, '.');
        if ((pszExt)&&(((!_tcsicmp(pszExt, TEXT(".jpg")))||(!_tcsicmp(pszExt, TEXT(".jpeg")))||(!_tcsicmp(pszExt, TEXT(".bmp")))||(!_tcsicmp(pszExt, TEXT(".gif")))||(!_tcsicmp(pszExt, TEXT(".tga"))))))
          m_theApp->AddItem(2, (_tcsrchr(szTemp2, '\\') + 1), szTemp2);
        iLen+= strlen(&pszRecvBuff2[iLen]);
        pszTemp= &pszRecvBuff2[++iLen];
#ifdef _WIN32_WCE
      MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pszTemp, -1, szTemp2, MAX_PATH);
#else
      strcpy(szTemp2, pszTemp);
#endif
      }
      break;
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
void CFPPTPanel::SetSlideCount(int iSlideCount)
{
  m_iSlideCount= iSlideCount;

  // allocate memory for previews
  m_bmpPreviews= new CFBitmap*[m_iSlideCount];
  
  // set entry in droplist for each slide
  m_dropList->ClearItems();
  for (int i=0; i<m_iSlideCount; i++) {
    TCHAR szText[80];
    TCHAR szBuff[10];

		_tcscpy(szText, TEXT("Slide "));
		_itot(i + 1, szBuff, 10);
		_tcscat(szText, szBuff);
    m_dropList->AddItem(szText); 
    
    // NULL-initialize previews
    m_bmpPreviews[i]= NULL;
  } 
}

//--------------------------------------------------------------------------------
// Does a safe-release of the preview pictures.
void CFPPTPanel::DeletePreviews()
{
  if (m_bmpPreviews) {
    for (int i=0; i<m_iSlideCount; i++) {
      if (m_bmpPreviews[i]) {
        delete(m_bmpPreviews[i]);
        m_bmpPreviews[i]= NULL;
      }
    }
    delete(m_bmpPreviews);
    m_bmpPreviews= NULL;
  }
}