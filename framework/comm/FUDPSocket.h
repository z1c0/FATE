#pragma once

#include "FInetAddr.h"

class CFUDPSocketImpl;

//--------------------------------------------------------------------------------
/// Implementation of a UDP socket.
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
  /// User can specify a timeout for send/receive operations. Timeout is specified
  /// in seconds. The constant NO_TIMEOUT means, that operations will block.
  void SetTimeout(DWORD dwTimeout);
  bool IsReceiving() const;

private:
  CFUDPSocketImpl* m_pImpl;
};
