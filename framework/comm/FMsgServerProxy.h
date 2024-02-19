#pragma once

#include "FInetAddr.h"

//--------------------------------------------------------------------------------
/// Constants
#define MAX_VR_NODES 6

//--------------------------------------------------------------------------------
class CFMsgServerProxy  
{
public:
	CFMsgServerProxy();
	virtual ~CFMsgServerProxy();
  bool ReadConfigFile(LPCTSTR pszFileName);
  bool ReadConfigData(char *pszData);
  void AddVRAddr(CFInetAddr *pAddr) { m_addrVR[m_iNodeCount++]= pAddr; }
  void AddVRAddr(LPCTSTR pszHost, int iPort) { AddVRAddr(new CFInetAddr(pszHost, iPort)); }
  bool SendCall(char *pObjName, char *pMethName, char **ppArgs);
  bool SendCall(char *pData);

private:
  CFInetAddr *m_addrVR[MAX_VR_NODES];
  int m_iNodeCount;
};

