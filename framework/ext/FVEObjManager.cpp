#include "FVEObjManager.h"
#include "../comm/FSocket.h"

//--------------------------------------------------------------------------------
CFVEObjManager::CFVEObjManager()
{
  // create a dummy object
  m_pAddrServer= new CFInetAddr(TEXT("127.0.0.1"), 1234);
}

//--------------------------------------------------------------------------------
CFVEObjManager::~CFVEObjManager()
{
  SAFE_DELETE(m_pAddrServer);
  Clear();
}

//--------------------------------------------------------------------------------
void CFVEObjManager::Clear()
{
  for (int i=0; i<m_ListObj.GetSize(); i++) delete(m_ListObj[i]);
  m_ListObj.Clear();
}

//--------------------------------------------------------------------------------
/// Reads IP configuration data for the message servers from the specified 
/// configuration file.
/// Syntax: " VR : IP-address : Port" (e.g. VR : 192.168.5.110 : 2233).
bool CFVEObjManager::ReadConfigFile(const TCHAR* pszFileName)
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
/// Reads IP configuration data for the message servers from the specified 
/// configuration text string.
/// Syntax: " OBJ_SERVER : IP-address : Port" (e.g. OBJ_SERVER : 192.168.5.110 : 2233).
bool CFVEObjManager::ReadConfigData(char *pszData)
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
            if (!strcmp(szHostName, "VR"))
            {
              SAFE_DELETE(m_pAddrServer);
              m_pAddrServer= new CFInetAddr(szHostAddr, atoi(szPort));
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
  return(true);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::Update()
{
  CFSocket sock;
  VE_PACKET pack= {0};

  // first clear object list
  m_ListObj.Clear();

  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the message
  pack.uiKind= REQ_LIST;

  // send the message
  sock.Send((const char*)&pack, sizeof(VE_PACKET));
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  if (pack.uiKind == REQ_LIST_OK) {    
    bool bRet= false;

    if (pack.uiLen) {
      int iRet= 0;
      DWORD *pdwList= (DWORD*)malloc(pack.uiLen);
      // receive rest of message (object list)
      while (((UINT)iRet < pack.uiLen)&&(iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
        iRet+= sock.Receive((char*)pdwList, pack.uiLen - iRet);
      }      
      if ((iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
        // create object list with received IDs
        for (UINT i=0; i<pack.uiLen / sizeof(DWORD); i++) {
          CFVEObj *pObj= new CFVEObj(pdwList[i]);
          m_ListObj.Append(pObj);
        }
        bRet= true;
      }
      delete(pdwList);
    }
    return(bRet);
  } else {
    return(false);
  }
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::SendPosition(CFVEObj *pObj)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_3D pack3D= {0};
  
  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the messages
  pack.uiKind= REQ_CHANGE;
  pack.uiExtra= CHANGE_POS;
  pack3D.ulID= pObj->GetID();
  pack3D.fVal1=pObj->GetPosX();
  pack3D.fVal2=pObj->GetPosY();
  pack3D.fVal3=pObj->GetPosZ();

  // send the messages
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  sock.Send((char*)&pack3D, sizeof(VE_PACKET_3D));
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  return(pack.uiKind == REQ_CHANGE_OK);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::SendOrientation(CFVEObj *pObj)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_3D pack3D= {0};
  
  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the messages
  pack.uiKind= REQ_CHANGE;
  pack.uiExtra= CHANGE_ORIENTATION;
  pack3D.ulID= pObj->GetID();
  pack3D.fVal1=pObj->GetAzim();
  pack3D.fVal2=pObj->GetElev();
  pack3D.fVal3=pObj->GetRoll();

  // send the messages
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  sock.Send((char*)&pack3D, sizeof(VE_PACKET_3D));
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  return(pack.uiKind == REQ_CHANGE_OK);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::SendScale(CFVEObj *pObj)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_3D pack3D= {0};
  
  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the messages
  pack.uiKind= REQ_CHANGE;
  pack.uiExtra= CHANGE_SCALE;
  pack3D.ulID= pObj->GetID();
  pack3D.fVal1=pObj->GetScaleX();
  pack3D.fVal2=pObj->GetScaleY();
  pack3D.fVal3=pObj->GetScaleZ();

  // send the messages
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  sock.Send((char*)&pack3D, sizeof(VE_PACKET_3D));
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  return(pack.uiKind == REQ_CHANGE_OK);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::SendExtra(CFVEObj *pObj, CFVEObjAttr *pAttr)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_ATTR packAttr= {0};
  
  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the messages
  pack.uiKind= REQ_CHANGE;
  pack.uiExtra= CHANGE_EXTRA;

#ifdef _WIN32_WCE
  WideCharToMultiByte(CP_ACP, 0, pAttr->GetName(), -1, packAttr.szName, MAX_ATTR_NAME_LEN, NULL, NULL);
#else
  strcpy(packAttr.szName, pAttr->GetName());
#endif
  packAttr.uiLen= pAttr->GetSize();

  // send the messages
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  sock.Send((char*)&packAttr, sizeof(VE_PACKET_ATTR));
  sock.Send((char*)pAttr->GetData(), pAttr->GetSize());
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  return(pack.uiKind == REQ_CHANGE_OK);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::SendAll(CFVEObj *pObj)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_OBJ packObj= {0};
  
  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the messages
  pack.uiKind= REQ_CHANGE;
  pack.uiExtra= CHANGE_ALL;
  packObj.ulID= pObj->GetID();
#ifdef _WIN32_WCE
  WideCharToMultiByte(CP_ACP, 0, pObj->GetName(), -1, packObj.szName, MAX_OBJ_NAME_LEN, NULL, NULL);
#else
  strcpy(packObj.szName, pObj->GetName());
#endif
  packObj.fPosX= pObj->GetPosX();
  packObj.fPosY= pObj->GetPosY();
  packObj.fPosZ= pObj->GetPosZ();
  packObj.fAzim= pObj->GetAzim();
  packObj.fElev= pObj->GetElev();
  packObj.fRoll= pObj->GetRoll();
  packObj.fScaleX= pObj->GetScaleX();
  packObj.fScaleY= pObj->GetScaleY();
  packObj.fScaleZ= pObj->GetScaleZ();
  packObj.iExtra= pObj->GetExtraAttrCount();

  // send the messages
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  sock.Send((char*)&packObj, sizeof(VE_PACKET_OBJ));
  for (int i=0; i<packObj.iExtra; i++) {
    // create the messages
    VE_PACKET_ATTR packAttr= {0};
#ifdef _WIN32_WCE
      WideCharToMultiByte(CP_ACP, 0, pObj->GetExtraAttr(i)->GetName(), -1, 
                          packAttr.szName, MAX_ATTR_NAME_LEN, NULL, NULL);
#else
      strcpy(packAttr.szName, pObj->GetExtraAttr(i)->GetName());
#endif
    packAttr.uiLen= pObj->GetExtraAttr(i)->GetSize();

    // send the messages
    sock.Send((char*)&packAttr, sizeof(VE_PACKET_ATTR));
    sock.Send((char*)pObj->GetExtraAttr(i)->GetData(),
               pObj->GetExtraAttr(i)->GetSize());  
  }
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  return(pack.uiKind == REQ_CHANGE_OK);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::RequestAllValues()
{
  for (int i=0; i<m_ListObj.GetSize(); i++) {
    if (!RequestValues(m_ListObj[i])) return(false);
  }
  return(true);
}

//--------------------------------------------------------------------------------
bool CFVEObjManager::RequestValues(CFVEObj *pObj)
{
  CFSocket sock;
  VE_PACKET pack= {0};
  VE_PACKET_OBJ packObj= {0};
  TCHAR szObjName[MAX_OBJ_NAME_LEN];

  // first clear object list
  m_ListObj.Clear();

  // connect to server
  sock.Create();
  if (!sock.Connect(m_pAddrServer)) return(false);

  // create the message
  pack.uiKind= REQ_OBJ;
  pack.uiExtra= pObj->GetID();

  // send the message
  sock.Send((char*)&pack, sizeof(VE_PACKET));
  
  // receive answer
  sock.Receive((char*)&pack, sizeof(VE_PACKET));
  if (pack.uiKind == REQ_OBJ_OK) {    
    sock.Receive((char*)&packObj, sizeof(VE_PACKET_OBJ));

    // set values
    pObj->SetAutoPost(false);
#ifdef _WIN32_WCE
    MultiByteToWideChar(CP_ACP, 0, packObj.szName, -1, szObjName, MAX_OBJ_NAME_LEN);
#else
    strcpy(szObjName, packObj.szName);
#endif
    pObj->SetName(szObjName);
    pObj->SetPosX(packObj.fPosX);
    pObj->SetPosY(packObj.fPosY);
    pObj->SetPosZ(packObj.fPosZ);
    pObj->SetAzim(packObj.fAzim);
    pObj->SetElev(packObj.fElev);
    pObj->SetRoll(packObj.fRoll);
    pObj->SetScaleX(packObj.fScaleX);
    pObj->SetScaleY(packObj.fScaleY);
    pObj->SetScaleZ(packObj.fScaleZ);

    // extra attribute(s)
    for (int i=0; i<packObj.iExtra; i++)
    {     
      VE_PACKET_ATTR packAttr= {0};
      unsigned char* pData;
      int iRet= 0;
      TCHAR szAttrName[MAX_ATTR_NAME_LEN];

      // receive information about attribute
      sock.Receive((char*)&packAttr, sizeof(VE_PACKET_ATTR));
      pData = (unsigned char*)malloc(packAttr.uiLen);
      
      // receive actual attribute
      while (((UINT)iRet < packAttr.uiLen)&&(iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
        iRet+= sock.Receive((char*)pData, packAttr.uiLen - iRet);
      }      
      if ((iRet != SOCKET_ERROR)&&(iRet != SOCKET_TIMEOUT)) {
        // add received attribute
#ifdef _WIN32_WCE
        MultiByteToWideChar(CP_ACP, 0, packAttr.szName, -1, szAttrName, MAX_OBJ_NAME_LEN);
#else
        strcpy(szAttrName, packAttr.szName);
#endif
        pObj->SetExtraAttr(szAttrName, pData, packAttr.uiLen);
        delete(pData);
      
      } else {
        delete(pData);
        return(false);
      }
    }
    pObj->SetAutoPost(true);
    return(true);
  } else {
    return(false);
  }
}