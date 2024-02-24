#pragma once

#include "FInetAddrImpl.h"

class CFUDPSocketImpl
{
public:
	CFUDPSocketImpl();
	~CFUDPSocketImpl();

	bool Bind(int port);
	bool Create();
	int Send(const char* pBuff, const int iSize);
	int Receive(char* pBuff, const int iSize);
	int Send(const char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr);
	int Receive(char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr);  
	void SetRemoteAddr(CFInetAddrImpl* pAddrRemote);
	void SetTimeout(DWORD dwTimeout);
	bool IsReceiving() const;

private:
	int m_socket;
	int m_listenPort;
	unsigned long m_timeout;
};