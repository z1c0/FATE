#pragma once

#include "IFSocket.h"

class CFUDPSocket : public IFSocket  
{
public:
  CFUDPSocket(CFInetAddr *pAddr= NULL) : IFSocket() { m_pAddrRemote= pAddr; };
  virtual ~CFUDPSocket() 
  { 
    if (m_pAddrRemote) delete(m_pAddrRemote);
  };
  
  virtual bool Create();
	
  virtual int Send(const char* pBuff, const int iSize);
	virtual int Receive(char* pBuff, const int iSize);
  int Send(const char* pBuff, const int iSize, const CFInetAddr* pInetAddr);
  int Receive(char* pBuff, const int iSize, const CFInetAddr* pInetAddr);
  
  void SetRemoteAddr(CFInetAddr* pAddrRemote) 
  { 
    if (!m_pAddrRemote) m_pAddrRemote= new CFInetAddr();
    memcpy(m_pAddrRemote, m_pAddrRemote, sizeof(CFInetAddr));

  };

  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);

protected:

  CFInetAddr* m_pAddrRemote;
};

