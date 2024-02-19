#pragma once

#include "FInetAddrImpl.h"

class CFUDPSocketImpl
{
public:
  CFUDPSocketImpl() : m_pAddrRemote(NULL) {}
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
    memcpy(pAddrRemote, m_pAddrRemote, sizeof(CFInetAddrImpl));
  };
  bool IsReceiving() const;
  void SetTimeout(DWORD dwTimeout);
  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);

private:
  CFInetAddrImpl* m_pAddrRemote;
  NetSocketRef m_refSock;
};

