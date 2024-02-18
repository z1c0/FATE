#pragma once

#include "FInetAddr.h"

class CFUDPSocketImpl;

class CFUDPSocket
{
public:
  CFUDPSocket(CFInetAddr *pAddr = NULL);
  ~CFUDPSocket();

	bool Bind(int port);
  bool Create();
  int Send(const char* pBuff, const int iSize);
	int Receive(char* pBuff, const int iSize);
  int Send(const char* pBuff, const int iSize, const CFInetAddr* pInetAddr);
  int Receive(char* pBuff, const int iSize, const CFInetAddr* pInetAddr);  
  void SetRemoteAddr(CFInetAddr* pAddrRemote);
  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);
  void SetTimeout(DWORD dwTimeout);
  bool IsReceiving() const;

private:
  CFUDPSocketImpl* m_pImpl;
};
