#pragma once

#include "FInetAddrImpl.h"

class CFSocketImpl
{
public:
	CFSocketImpl();
  bool Create();
  bool Close();
  bool Bind(int port);
  bool Bind(const CFInetAddrImpl* pInetAddr);
  bool Listen();
  int Accept(CFSocketImpl& sock);
  bool Connect(const CFInetAddrImpl* pInetAddr);
  bool Connect(char *pAddrStr, int port);
  int Send(const char* pBuff, const int size);
	int Receive(char* pBuff, const int size);	
  int Write(const char* pBuff, const int size);	
  bool IsClientConnecting() const;
  bool IsReceiving() const;
  int GetListenPort() const;
  void SetTimeout(DWORD dwTimeout);

  static bool InitSocketLibrary() { return true; }
  static bool CleanupSocketLibrary() { return true; }
	static CFInetAddrImpl* GetHostByName(const char* pHostName, int port = 0);
	static const char* GetHostByAddr(const CFInetAddrImpl* pInetAddr);
};