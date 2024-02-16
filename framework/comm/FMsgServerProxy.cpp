#include "FMsgServerProxy.h"
#include "FSocket.h"

//--------------------------------------------------------------------------------
CFMsgServerProxy::CFMsgServerProxy()
{
  for (int i=0; i<MAX_VR_NODES; i++) m_addrVR[i]= NULL;
  m_iNodeCount= 0;
}

//--------------------------------------------------------------------------------
CFMsgServerProxy::~CFMsgServerProxy()
{
  for (int i=0; i<MAX_VR_NODES; i++) SAFE_DELETE(m_addrVR[i])
}

//--------------------------------------------------------------------------------
/// Reads IP configuration data for the message servers from the specified 
/// configuration file.
/// Syntax: " VR : IP-address : Port" (e.g. VR : 192.168.5.110 : 2233).
bool CFMsgServerProxy::ReadConfigFile(LPCTSTR pszFileName)
{
	FILE *configFile;
	int iRead;
  char szReadBuff[2048];
  
  if ((configFile= _tfopen(pszFileName, TEXT("rt"))) == NULL)
  {
		return false;

  }
  else
  {
    iRead = fread(szReadBuff, sizeof(char), 2048, configFile);
    szReadBuff[iRead]= 0;
    ReadConfigData(szReadBuff);
    fclose(configFile);
  }
  return true;
}

//--------------------------------------------------------------------------------
/// Reads IP configuration data for the message servers from the specified 
/// configuration text string.
/// Syntax: " VR : IP-address : Port" (e.g. VR : 192.168.5.110 : 2233).
bool CFMsgServerProxy::ReadConfigData(char *pszData)
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
          if ((cRead[0] == '\n')||(!cRead[0]))  // IP-settings complete
          {
            iState= 0;
            if (!_tcscmp(szHostName, "VR"))
            {
              m_addrVR[m_iNodeCount++]= new CFInetAddr(szHostAddr, atoi(szPort));

            }
            else
            {
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
  return true;
}

//--------------------------------------------------------------------------------
/// Creates correct call-string from the speciefied parameters.
/// Make sure that last entry in the "ppArgs" array is a NULL-pointer!
bool CFMsgServerProxy::SendCall(char *pObjName, char *pMethName, char **ppArgs)
{
  char szBuff[1024];
  bool bFirst = true;

  // create the call string
  strcpy(szBuff, pObjName);  
  strcat(szBuff, "|");
  strcat(szBuff, pMethName); 
  strcat(szBuff, "(");
  while (ppArgs++)
  {
    if (!bFirst) strcat(szBuff, ",");
    strcat(szBuff, *ppArgs);
    bFirst = false;
  }
  strcat(szBuff, ")\n");
  return SendCall(szBuff);
}

//--------------------------------------------------------------------------------
/// Sends the string (message server method call) in "pData" to all specified VR
/// servers.
/// Syntax: "object-name|method-name(parameters)\n"
bool CFMsgServerProxy::SendCall(char *pData)
{
  CFSocket sock;

  for (int i=0; i<m_iNodeCount; i++)
  {    
    if (!sock.Create()) return false;
    if (!sock.Connect(m_addrVR[i])) return false;
    sock.Write(pData, strlen(pData) + 1);
    sock.Close();
  }
  return true;
}