#ifdef _WIN32

#include "FController.h"
#include "../base/FateTypeDefs.h"
#include "../comm/FInetAddr.h"
#include "../comm/FUDPSocket.h"

//--------------------------------------------------------------------------------
/// Global Thread function which sends controller data to the specified TRACKD host.
DWORD WINAPI SendThreadFunc(LPVOID pParam)
{
  // cast back parameter
  CFController *cont= (CFController*)pParam;
  DWORD dwSleep= cont->m_dwSleep;
  
  while(cont->m_bRun) {
    cont->ProcessData();
    Sleep(dwSleep);
  }
  return(1);
}




//--------------------------------------------------------------------------------
/// Creates controller object without specified host where data is sent to.
CFController::CFController() : IFateComponent()
{
  m_bVisible= false;  // no graphical representation

  // create dummy address object
  CFInetAddr *pAddr= new CFInetAddr(TEXT("127.0.0.1"), 0);
  Init(pAddr);
}

//-------------------------------------------------------------------------------
/// Specify host and port where the controller data is sent to.
CFController::CFController(CFInetAddr *pAddr) : IFateComponent()
{
  Init(pAddr);
}

//--------------------------------------------------------------------------------
/// Specify host and port where the controller data is sent to.
CFController::CFController(char *pszHost, USHORT usPort) : IFateComponent()
{
  // prepare socket connection to controller host
  CFInetAddr *addr= new CFInetAddr(pszHost, usPort);
  Init(addr);
}

//--------------------------------------------------------------------------------
CFController::~CFController()
{
  if (m_bRun) {
    m_bRun= false;
    WaitForSingleObject(m_hThread, INFINITE);
    CloseHandle(m_hThread);
  }
  SAFE_DELETE(m_addrCont);
  SAFE_DELETE(m_sockCont);
}

//--------------------------------------------------------------------------------
void CFController::Init(CFInetAddr *pAddr)
{
  m_hThread= INVALID_HANDLE_VALUE;
  m_bRun= false;
  m_addrCont= pAddr;
  m_sockCont= new CFUDPSocket();
  m_sockCont->Create();
  ResetValues();
}

//--------------------------------------------------------------------------------
void CFController::SetDestAddr(CFInetAddr *pAddr)
{
  SAFE_DELETE(m_addrCont);
  m_addrCont= pAddr;
}

//--------------------------------------------------------------------------------
bool CFController::KeyDown(PdaKey key)
{
  int iVal;

  switch(key) {
    case KeyWest:  // LEFT
      iVal= m_iDirs[0];

      if (iVal > 0) iVal= 0;
      else if (iVal == 0) iVal= -600;
      else break;
      m_iDirs[0]= iVal; 
      return(true);

    case KeyEast:  // RIGHT
      iVal= m_iDirs[0];
      
      if (iVal < 0) iVal= 0;
      else if (iVal == 0) iVal= 600;
      else break;
      m_iDirs[0]= iVal;
      return(true);
    
    case KeyNorth:  // UP      
      iVal= m_iDirs[1];
      
      if (iVal < -800) iVal+= 50;
      else if (iVal < -600) iVal+= 100;
      else if (iVal < -400) iVal+= 200;
      else if (iVal < 400) iVal+= 400;
      else if (iVal < 600) iVal+= 200;
      else if (iVal < 800) iVal+= 100;
      else if (iVal < 1000) iVal+= 50;      
      else break;
      m_iDirs[1]= iVal;
      return(true);

    case KeySouth:  // DOWN
      iVal= m_iDirs[1];

      if (iVal > 800) iVal-= 50;
      else if (iVal > 600) iVal-= 100;
      else if (iVal > 400) iVal-= 200;
      else if (iVal > -400) iVal-= 400;
      else if (iVal > -600) iVal-= 200;
      else if (iVal > -800) iVal-= 100;
      else if (iVal > -1000) iVal-= 50;      
      else break;
      m_iDirs[1]= iVal;
      return(true);

    case KeyOne:  // BUTTON 1
      m_iBtns[0]= 1;
      return(true);
      break;

    case KeyTwo:  // BUTTON 2
      m_iBtns[1]= 1;
      return(true);
      break;

    case KeyThree:  // BUTTON 3
      m_iBtns[2]= 1;
      return(true);
      break;

    case KeyFour:  // BUTTON 4
      m_iBtns[3]= 1;
      return(false);
      break;

    default:
      break;
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFController::KeyUp(PdaKey key)
{
  switch(key) {
    case KeyWest:   // LEFT
      m_iDirs[0]= 0; 
      return(true);

    case KeyEast:  // RIGHT
      m_iDirs[0]= 0;
      return(true);
    
    case KeyNorth:     // UP      
      m_iDirs[1]= 0;
      return(true);

    case KeySouth:   // DOWN
      m_iDirs[1]= 0;
      return(true);
    
    case KeyOne:  // BUTTON 1
      m_iBtns[0]= 0;
      return(true);
      break;

    case KeyTwo:  // BUTTON 2
      m_iBtns[1]= 0;
      return(true);
      break;

    case KeyThree:  // BUTTON 3
      m_iBtns[2]= 0;
      return(true);
      break;

    case KeyFour:  // BUTTON 4
      m_iBtns[3]= 0;
      return(false);
      break;

    default:
      break;
  }
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// Enables permanent sending of controller values to TRACKD.EXE.
/// Parameter dwMillis can specify a waiting time after a sending operation.
/// If sending is already in progress false is returned.
bool CFController::StartSending(DWORD dwMillis /* = 500 */)
{
  if (m_bRun) return(false);

  m_bRun= true;
  m_hThread= CreateThread(NULL, 0, SendThreadFunc, (LPVOID)this, 0, NULL);
  m_dwSleep= dwMillis;
  return(true);
}

//--------------------------------------------------------------------------------
/// Stops permanent sending of controller values to TRACKD.EXE
/// If no sending is in progress false is returned.
bool CFController::StopSending()
{
  if (!m_bRun) return(false);
  
  m_bRun= false;
  return(true);
}

//--------------------------------------------------------------------------------
void CFController::ResetValues()
{
  m_iDirs[0]= 0;
  m_iDirs[1]= 0;
  m_iBtns[0]= 0;
  m_iBtns[1]= 0;
  m_iBtns[2]= 0;
  m_iCounter= 0;
  m_iByte8=   0;
  m_iByte9=   0;
  m_iByte10=  0;
  m_iByte11=  0;
}

//--------------------------------------------------------------------------------
/// Method is supposed to be called by sublasses to process data in a different
/// way.
void CFController::ProcessData()
{
  // default action: send to TRACKD
  SendContInfo();
}



//--------------------------------------------------------------------------------
bool CFController::SendContInfo()
{
  char szData[64];

  // if we don't have a valid destination address, return immediately
  if (!m_addrCont) return(false);

  // send directly to trackd.exe
  // zero init data
  memset(szData, 0, 64);
  
  // bytes: 0 - 7: 
  // constant: 0   0   0  1   0   0   0   0  0   
  szData[3]=  1;
  
  // bytes:  8 - 15
  // coding
  m_iCounter+= 10000;
  // counter overflow?
  if (m_iCounter >= 1000000) {
    m_iCounter-= 1000000;
    m_iByte11++;
    // byte overflow?
    if (m_iByte11 == 256) {
      m_iByte11= 0;
      m_iByte10++;
      // byte overflow?
      if (m_iByte10 == 256) {
        m_iByte10= 0;
        m_iByte9++;
        // byte overflow?
        if (m_iByte9 == 256) {
          m_iByte9= 0;
          m_iByte8++;
        }
      }
    }
  }
  szData[8]=  m_iByte8;
  szData[9]=  m_iByte9;
  szData[10]= m_iByte10;
  szData[11]= m_iByte11;
  // byte 12 is constant 0
  szData[13]= (m_iCounter & 0x00FF0000) >> 16;
  szData[14]= (m_iCounter & 0x0000FF00) >>  8;
  szData[15]= (m_iCounter & 0x000000FF);

  // bytes: 16 - 35
  // constant: 0   0   0   0   0   0   0   2   0   0   0   3   0   0   0  36   0   0   0  44  
  szData[23]=  2;
  szData[27]=  3;
  szData[31]= 36;
  szData[35]= 44;

  // bytes: 36 - 39
  // contain left-right-data
  szData[36]= (m_iDirs[0] & 0xFF000000) >> 24;
  szData[37]= (m_iDirs[0] & 0x00FF0000) >> 16;
  szData[38]= (m_iDirs[0] & 0x0000FF00) >>  8;
  szData[39]=  m_iDirs[0] & 0x000000FF;
  
  // bytes: 40 - 43
  // contain up-down-data
  szData[40]= (m_iDirs[1] & 0xFF000000) >> 24;
  szData[41]= (m_iDirs[1] & 0x00FF0000) >> 16;
  szData[42]= (m_iDirs[1] & 0x0000FF00) >>  8;
  szData[43]=  m_iDirs[1] & 0x000000FF;

  // byte: 44 
  // which button was pressed?
  // 1: button 1
  // 2: button 2
  // 4: button 3
  // 8: button 4
  szData[44]= 0;
  if (m_iBtns[0]) szData[44]|= 0x01;
  if (m_iBtns[1]) szData[44]|= 0x02;
  if (m_iBtns[2]) szData[44]|= 0x04;
  if (m_iBtns[3]) szData[44]|= 0x08;
  
  // bytes: 45 - 47
  // constant: 0   0   0   
  
  m_sockCont->Send(szData, 48, m_addrCont);
  return(true);
}

//--------------------------------------------------------------------------------
/// Reads IP configuration data for the TRACKD server from the specified 
/// configuration file.
/// Syntax: " TRACKD : IP-address : Port" (e.g. TRACKD : 192.168.5.110 : 2233).
bool CFController::ReadConfigFile(LPCTSTR pszFileName)
{
	FILE *configFile;
	int iRead;
  char szReadBuff[2048];
  
  if ((configFile= _tfopen(pszFileName, TEXT("rt"))) == NULL) {
		return(false);

  } else {
    iRead= fread(szReadBuff, sizeof(char), 2048, configFile);
    szReadBuff[iRead]= 0;
    ReadConfigData(szReadBuff);
    fclose(configFile);  
  }
  return(true);
}

//--------------------------------------------------------------------------------
/// Reads IP configuration data for the TRACKD server from the specified 
/// configuration text string.
/// Syntax: " TRACKD : IP-address : Port" (e.g. TRACKD : 192.168.5.110 : 2233).
bool CFController::ReadConfigData(char *pszData)
{
  char cRead[2];  
  char szHostName[32];
  char szHostAddr[256];
  char szPort[8];
	int iState= 0;

  cRead[1]     = 0;
	szHostName[0]= 0;
  szHostAddr[0]= 0;
  szPort[0]    = 0;

  for (UINT i=0; i<=strlen(pszData) + 1; i++) {
    cRead[0]= pszData[i];
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
          if ((cRead[0] == '\n')||(!cRead[0])) {  // IP-settings complete
            iState= 0;
            if (!_stricmp(szHostName, "TRACKD")) {                
              SAFE_DELETE(m_addrCont);
              m_addrCont= new CFInetAddr(szHostAddr, atoi(szPort));

            } else {
              // unknown entry found?
              // ... just skip
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
  return(true);
}

#endif