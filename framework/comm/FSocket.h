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
  bool Accept(CFSocket& sock);	
  int Send(const char* pBuff, const int size);
	int Receive(char* pBuff, const int size);
  bool GetRemoteAddr(CFInetAddr* pIntAddr);
  bool GetInetAddr(CFInetAddr* pInetAddr);
  bool IsClientConnecting() const;
  bool IsReceiving() const;
  int GetListenPort() const;
  void SetTimeout(DWORD dwTimeout);

  static bool InitSocketLibrary();
  static bool CleanupSocketLibrary();
	static CFInetAddr* GetHostByName(const char* pHostName, int port = 0);
	static const char* GetHostByAddr(const CFInetAddr* pInetAddr);

private:
  CFSocketImpl* m_pImpl;
};
