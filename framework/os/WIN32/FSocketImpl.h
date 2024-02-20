#pragma once

#include "../../comm/FInetAddr.h"

class CFSocketImpl
{
public:
  CFSocketImpl();

  bool Create();	
  bool Close();	
  bool Listen();	
  bool Connect(const CFInetAddrImpl* pInetAddr);
	bool Connect(char *pAddrStr, USHORT usPort);	
  bool Bind(int port);	
  bool Bind(const CFInetAddrImpl* pInetAddr);
  bool Accept(CFSocketImpl& sock);	
	int Receive(char* pBuff, const int size);	
  int Send(const char* pBuff, const int size);
  bool GetRemoteAddr(CFInetAddrImpl* pIntAddr);
  bool GetInetAddr(CFInetAddrImpl* pInetAddr);
  bool IsReceiving() const;
  bool IsClientConnecting() const;
  int GetListenPort() const { return m_listenPort; }
  void SetTimeout(DWORD dwTimeout) { m_dwTimeout= dwTimeout; };  

  static bool InitSocketLibrary();
  static bool CleanupSocketLibrary();
	static CFInetAddrImpl* GetHostByName(const char* pHostName, int port);
	static const char* GetHostByAddr(const CFInetAddrImpl* pInetAddr);

private:
  int SendInternal(const char* pBuff, const int iSize);

  SOCKET m_hSocket;
  DWORD m_dwTimeout;
  int m_listenPort;
};


