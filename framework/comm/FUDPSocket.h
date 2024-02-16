#pragma once

#include "FInetAddr.h"

class CFUDPSocketImpl;

//--------------------------------------------------------------------------------
/// Implementation of a UDP socket.
class CFUDPSocket
{
public:
  CFUDPSocket(CFInetAddr *pAddr = NULL) { m_pAddrRemote = pAddr; };
  virtual ~CFUDPSocket() 
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
    if (!m_pAddrRemote)
    {
      m_pAddrRemote = new CFInetAddr();
    }
    memcpy(m_pAddrRemote, m_pAddrRemote, sizeof(CFInetAddr));
  };

  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);

private:
  CFUDPSocketImpl* m_pImpl;
  CFInetAddr* m_pAddrRemote;
};
