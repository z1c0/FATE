#pragma once

#include "../base/FateTypeDefs.h"

class CFInetAddr;
class CFSocketImpl;

//--------------------------------------------------------------------------------
/// Implementation of a TCP socket.
class CFSocket
{
public:
  CFSocket();
  virtual ~CFSocket();
  
  bool Create();
  bool Listen();
	bool Bind(const CFInetAddr* pInetAddr);
	bool Bind(int port);
  bool Close();
  bool Connect(const CFInetAddr* pInetAddr);
	bool Connect(char *pAddrStr, int port);	
  int Accept(CFSocket& sock);	
  int Send(const char* pBuff, const int size);
	int Receive(char* pBuff, const int size);	
  int Write(const char* pBuff, const int size);	
  bool GetRemoteAddr(CFInetAddr* pIntAddr);	
  bool GetInetAddr(CFInetAddr* pInetAddr);  
  bool IsClientConnecting();
  bool IsReceiving();

  static bool InitSocketLibrary();
  static bool CleanupSocketLibrary();

private:
  CFSocketImpl* m_pImpl;
};
