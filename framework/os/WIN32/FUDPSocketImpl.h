#pragma once

#include "../../comm/FInetAddr.h"

class CFUDPSocketImpl
{
public:
  CFUDPSocketImpl(CFInetAddr *pAddr = NULL) { m_pAddrRemote = pAddr; };
  virtual ~CFUDPSocketImpl() 
  { 
    SAFE_DELETE(m_pAddrRemote);
  };  
  bool Create();	
  int Send(const char* pBuff, const int iSize);
	int Receive(char* pBuff, const int iSize);
  int Send(const char* pBuff, const int iSize, const CFInetAddr* pInetAddr);
  int Receive(char* pBuff, const int iSize, const CFInetAddr* pInetAddr);  
  void SetRemoteAddr(CFInetAddr* pAddrRemote) 
  { 
    if (!m_pAddrRemote) m_pAddrRemote= new CFInetAddr();
    memcpy(m_pAddrRemote, m_pAddrRemote, sizeof(CFInetAddr));

  };
  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);

private:
  SOCKET m_hSocket;
  CFInetAddr* m_pAddrRemote;
  DWORD m_dwTimeout;
};


