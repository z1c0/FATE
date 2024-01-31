// FMsgServerProxy.h: interface for the CFMsgServerProxy class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMSGSERVERPROXY_H__19E0ECA9_590B_4754_BF73_DC050C4DA12E__INCLUDED_)
#define AFX_FMSGSERVERPROXY_H__19E0ECA9_590B_4754_BF73_DC050C4DA12E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
  BOOL ReadConfigFile(LPCTSTR pszFileName);
  BOOL ReadConfigData(char *pszData);
  void AddVRAddr(CFInetAddr *pAddr) { m_addrVR[m_iNodeCount++]= pAddr; };
  void AddVRAddr(LPCTSTR pszHost, int iPort) { AddVRAddr(new CFInetAddr(pszHost, iPort)); };
  BOOL SendCall(char *pObjName, char *pMethName, char **ppArgs);
  BOOL SendCall(char *pData);
private:
  CFInetAddr *m_addrVR[MAX_VR_NODES];
  int m_iNodeCount;
};

#endif // !defined(AFX_FMSGSERVERPROXY_H__19E0ECA9_590B_4754_BF73_DC050C4DA12E__INCLUDED_)
