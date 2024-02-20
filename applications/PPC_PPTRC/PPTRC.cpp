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
}

//--------------------------------------------------------------------------------
CPPTRC::~CPPTRC()
{
	SAFE_DELETE(m_bmpTitle);
	SAFE_DELETE(m_btnClose);
	SAFE_DELETE(m_panelPPT);
	SAFE_DELETE(m_panelFiles);
	SAFE_DELETE(m_addrPPT);
	SAFE_DELETE(m_pSockRecv);
}

//--------------------------------------------------------------------------------
bool CPPTRC::InitFateApp()
{
	TCHAR szPath[1024];

	// disable automatic suspension of app
	m_pSystem->EnableSuspend(false);

	// parse config file for IP configuration
	_tcscpy(szPath, m_app->GetAppPath());
	_tcscat(szPath, TEXT("IP.txt"));
	if (!ReadConfigFile(szPath))
	{
		Error(TEXT("Configuration file could not be read!"));
		return false;
	}  
	
	// create a socket and bind it to port  
	m_pSockRecv = new CFSocket();
	m_pSockRecv->Create();
	m_pSockRecv->SetTimeout(0);
	m_pSockRecv->Bind(PPC_PORT);
	m_pSockRecv->Listen();

	// create title bar
	m_bmpTitle = new CFBitmap(m_pSystem->GetDoubleBuffer());
	if (!m_bmpTitle->Load(IDB_TITLE, "res_pics/title.bmp")) return(false);

	// create close button
	CFBitmap *bmpClose= new CFBitmap(m_pSystem->GetDoubleBuffer());
	if (!bmpClose->Load(IDB_SKIP, "res_pics/skip.bmp")) return(false);
	CFBitmap *bmpCloseDis= new CFBitmap(m_pSystem->GetDoubleBuffer());
	if (!bmpCloseDis->Load(IDB_SKIP_DIS, "res_pics/skip_dis.bmp")) return(false);
	m_btnClose= new CFButton(bmpClose, NULL, bmpCloseDis);
	m_btnClose->SetX(m_bmpTitle->GetRight() - m_btnClose->GetWidth() - 3);
	m_btnClose->SetY(3);
	m_btnClose->SetVisible(true);
	m_btnClose->SetId(ID_BTN_QUITAPP);
	Add(*m_btnClose);

	// create PPT panel
	m_panelPPT= new CFPPTPanel();
	Add(*m_panelPPT);
	if (!m_panelPPT->Create()) return(false);
	m_panelPPT->SetY(m_bmpTitle->GetBottom());
	m_panelPPT->SetVisible(false);

	// create panel for chosing files
	m_panelFiles = new CFFilePanel();
	Add(*m_panelFiles);
	if (!m_panelFiles->Create()) return(false);

	m_panelFiles->SetY(m_bmpTitle->GetBottom());
	m_panelFiles->SetVisible(true);
	
	// init a timer for polling the receive socket
	m_pSystem->AddTimer(POLL_TIMER, 100);

	return true;
}

//--------------------------------------------------------------------------------
bool CPPTRC::ActivateFateApp()
{ 
	return(true);
}

//--------------------------------------------------------------------------------
bool CPPTRC::CloseFateApp()
{
	return(true);
}

//--------------------------------------------------------------------------------
void CPPTRC::Draw()
{
	ClearScreen(RGB(51, 102, 153));
	m_bmpTitle->Blit();
}

//--------------------------------------------------------------------------------
bool CPPTRC::StylusDown(int xPos, int yPos)
{
	return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CPPTRC::StylusMove(int xPos, int yPos)
{
	return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CPPTRC::StylusUp(int xPos, int yPos)
{
	return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CPPTRC::KeyDown(int vkKey)
{
	return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CPPTRC::KeyUp(int vkKey)
{
	return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CPPTRC::ClosePresentation()
{
	m_panelPPT->ClosePPT();
	m_panelPPT->SetVisible(false);
	m_panelFiles->ClearLabel();
	m_panelFiles->SetVisible(true);
	return(true);
}

//--------------------------------------------------------------------------------
bool CPPTRC::OpenPresentation()
{
	TCHAR *pszFilePathW;
	char *pszFilePath;
	int iLen;
	int i;

	pszFilePathW= m_panelFiles->GetFilePath();
	if (!pszFilePathW) return(false);
	iLen= _tcslen(pszFilePathW) + 1;
	pszFilePath= (char*)malloc(iLen);
	for (i=0; i<iLen; i++) {
		pszFilePath[i]= (char)pszFilePathW[i];
	}
	if (m_panelPPT->StartPPT(pszFilePath)) {
		m_panelFiles->SetVisible(false);
		m_panelPPT->SetVisible(true);
	}
	free(pszFilePath);
	return(true);
}

//--------------------------------------------------------------------------------
bool CPPTRC::ButtonReleased(DWORD dwBtnID)
{
	switch(dwBtnID)
	{
		case ID_BTN_QUITAPP:
			Exit();
			return(true);
			break;

		case ID_BTN_CLOSEPPT:
			ClosePresentation();
			return(true);
			break;

		case ID_BTN_LOAD:
			OpenPresentation();
			return(true);
			break;

		default:
			break;
	}

	return(false);
}

//--------------------------------------------------------------------------------
// Reads IP configuration data from the specified configuration file.
bool CPPTRC::ReadConfigFile(LPCTSTR pszFileName)
{
	FILE *configFile;
	int iRead;
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
	if ((configFile= _tfopen(pszFileName, TEXT("rt"))) == NULL)
	{
		return false;
	}
	else {
		iRead = fread(szReadBuff, sizeof(char), 2048, configFile);
		szReadBuff[iRead]= EOF;
		for (int i=0; i<=iRead; i++)
		{
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
							if (!strcmp(szHostName, "PPT"))
							{
								m_addrPPT = new CFInetAddr(szHostAddr, atoi(szPort));
							}
							else
							{
								return false;
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
	return true;
}

//--------------------------------------------------------------------------------
// Used for error messages during initialization phase
void CPPTRC::Error(LPCTSTR pszErrMsg)
{
	m_pSystem->ShowError(pszErrMsg);
}

//--------------------------------------------------------------------------------
bool CPPTRC::SendToPPTHost(const char *pMsg)
{
	CFSocket sock;
	// establish connection first
	sock.SetTimeout(2);
	if (!sock.Create())
	{
		return false; 
	}
	if (!sock.Connect(m_addrPPT))
	{
		return false;
	}

	// prepare message
	int iMsgLen = strlen(pMsg);
	char* szSendBuff = new char[iMsgLen + 1];
	memset(szSendBuff, 0, iMsgLen + 1);

	int i = 3;
	while (iMsgLen)
	{
		szSendBuff[i--]= (unsigned char)iMsgLen & 0x000000FF;
		iMsgLen >>= 8;
	}
	memcpy(&szSendBuff[4], pMsg, strlen(pMsg) + 1);
	
	// send message
	int iRet = sock.Send(szSendBuff, strlen(pMsg) + 5);
	if (iRet == SOCKET_TIMEOUT || iRet == SOCKET_ERROR)
	{
		return false;
	} 
	sock.Close();
	
	return true;
}

//--------------------------------------------------------------------------------
bool CPPTRC::ReceiveFromPPTHost(CFSocket *sock) 
{
	char szRecvBuff1[4];
	char *pszRecvBuff2;
	char cCmd;
	DWORD dwRespLen;
	bool bRet = true;

	// first receive length of response
	int iRet = sock->Receive(szRecvBuff1, 4);
	if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(false);

	// bytes 0 ... 3 are length of message
	DWORD b0 = (unsigned char)szRecvBuff1[0] << 16;
	DWORD b1 = (unsigned char)szRecvBuff1[1] << 16;
	DWORD b2 = (unsigned char)szRecvBuff1[2] << 8;
	DWORD b3 = (unsigned char)szRecvBuff1[3];
	dwRespLen = b0 | b1 | b2 | b3;

	// allocate memory for response
	iRet= 0;
	pszRecvBuff2= (char*)malloc(dwRespLen);
	memset(pszRecvBuff2, 0, dwRespLen);
	while (((UINT)iRet < dwRespLen)&&(iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT))
	{
		iRet += sock->Receive((char*)&pszRecvBuff2[iRet], dwRespLen - iRet);
	}
	if ((iRet == SOCKET_ERROR)||(iRet == SOCKET_TIMEOUT)) return(false); 
	
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
			while(*pszTemp)
			{
				// get extension of file
				pszExt = _tcsrchr(szTemp2, '.');
				if ((pszExt)&&(((!_tcscmp(pszExt, TEXT(".ppt")))||(!_tcscmp(pszExt, TEXT(".pps"))))))
				{
					CFBitmap *bmpPPT = new CFBitmap(m_pSystem->GetDoubleBuffer());
					bmpPPT->Load(IDB_PPT_ICON, "res_pics/ppt_icon.bmp");
					m_panelFiles->AddItem(bmpPPT, (_tcsrchr(szTemp2, '\\') + 1), szTemp2);

				}
				else
				{
					CFBitmap *bmp= NULL;
					switch (pszTemp[strlen(pszTemp) - 1])
					{
						case FILE_MARK_DIR:
							bmp= new CFBitmap(m_pSystem->GetDoubleBuffer());
							bmp->Load(IDB_FOLDER_ICON, "res_pics/folder_icon.bmp");
							szTemp2[_tcslen(szTemp2) - 1] = 0;
							m_panelFiles->AddItem(bmp, (_tcsrchr(szTemp2, '\\') + 1), TEXT("?"));
							break;

						case FILE_MARK_FLOPPY:
							bmp= new CFBitmap(m_pSystem->GetDoubleBuffer());
							bmp->Load(IDB_FLOPPY_ICON, "res_pics/floppy_icon.bmp");
							szTemp2[_tcslen(szTemp2) - 1]= 0;
							m_panelFiles->AddItem(bmp, szTemp2, TEXT("?"));
							break;

						case FILE_MARK_CDROM:
							bmp= new CFBitmap(m_pSystem->GetDoubleBuffer());
							bmp->Load(IDB_CDROM_ICON, "res_pics/cdrom_icon.bmp");
							szTemp2[_tcslen(szTemp2) - 1]= 0;
							m_panelFiles->AddItem(bmp, szTemp2, TEXT("?"));
							break;

						case FILE_MARK_HD:
							bmp= new CFBitmap(m_pSystem->GetDoubleBuffer());
							bmp->Load(IDB_HD_ICON, "res_pics/hd_icon.bmp");
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
			m_pSystem->ForceRedraw();
			break;
		}

		case ERR:
		{
			bRet = false;
			break;
		}
	}

	free(pszRecvBuff2);

	return bRet;
}

//--------------------------------------------------------------------------------
bool CPPTRC::Timer(unsigned long id)
{
	if (id == POLL_TIMER)
	{
		// check for incoming data on socket
		CFSocket sock;
		if (m_pSockRecv->Accept(sock))
		{
			ReceiveFromPPTHost(&sock);
		}
		return true;
	}
	return false;
}
