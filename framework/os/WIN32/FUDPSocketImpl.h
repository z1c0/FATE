#pragma once

#include "FInetAddrImpl.h"

class CFUDPSocketImpl
{
public:
  CFUDPSocketImpl();
  virtual ~CFUDPSocketImpl() 
  { 
    SAFE_DELETE(m_pAddrRemote);
  };  
  bool Create();	
  bool Bind(int port);
  int Send(const char* pBuff, const int iSize);
	int Receive(char* pBuff, const int iSize);
  int Send(const char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr);
  int Receive(char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr);  
  void SetRemoteAddr(CFInetAddrImpl* pAddrRemote) 
  { 
    if (!m_pAddrRemote) m_pAddrRemote = new CFInetAddrImpl();
    memcpy(m_pAddrRemote, m_pAddrRemote, sizeof(CFInetAddrImpl));

  };
  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);
  void SetTimeout(DWORD dwTimeout) { m_dwTimeout= dwTimeout; };  
  bool IsReceiving() const;

private:
  CFInetAddrImpl* m_pAddrRemote;
  SOCKET m_hSocket;
  DWORD m_dwTimeout;
  int m_listenPort;
};


