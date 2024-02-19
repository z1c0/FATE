#pragma once

#include "FInetAddrImpl.h"

class CFSocketImpl
{
public:
  CFSocketImpl() {};
  virtual ~CFSocketImpl() {};
	
  bool Create();	
  bool Close();	
  bool Listen();	
  bool Bind(int port);
  bool Bind(const CFInetAddrImpl* pInetAddr);
  bool Connect(const CFInetAddrImpl* pInetAddr);
  bool Connect(char *pAddrStr, int iPort);
  int Accept(CFSocketImpl& sock);	
  int Send(const char* pBuff, const int iSize);
  int Receive(char* pBuff, const int iSize);	
  int Write(const char* pBuff, const int iSize);	
  bool GetRemoteAddr(CFInetAddrImpl* pIntAddr);	
  bool GetInetAddr(CFInetAddrImpl* pInetAddr);  
  bool IsClientConnecting() const;
  bool IsReceiving() const;
  int GetListenPort() const;
  void SetTimeout(DWORD dwTimeout);
  
  static bool InitSocketLibrary();
  static bool CleanupSocketLibrary();
  static CFInetAddrImpl* GetHostByName(const char* pHostName, int port);
  static const char* GetHostByAddr(const CFInetAddrImpl* pInetAddr);
};

