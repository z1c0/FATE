#pragma once

#include "../../comm/FInetAddr.h"

class CFSocketImpl
{
public:
  bool Create();	
  bool Listen();	
  bool Connect(const CFInetAddr* pInetAddr);
	bool Connect(char *pAddrStr, USHORT usPort);	
  int Accept(CFSocketImpl& sock);	
  virtual int Send(const char* pBuff, const int iSize);
	virtual int Receive(char* pBuff, const int iSize);	
  int Write(const char* pBuff, const int iSize);	
  bool GetRemoteAddr(CFInetAddr* pIntAddr);	
  bool GetInetAddr(CFInetAddr* pInetAddr);  
  bool IsClientConnecting();

  static bool InitSocketLibrary();
  static bool CleanupSocketLibrary();

private:
  SOCKET m_hSocket;
  DWORD m_dwTimeout;
};

