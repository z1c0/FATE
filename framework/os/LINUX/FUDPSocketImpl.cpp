#include "FUDPSocketImpl.h"

//------------------------------------------------------------------------------
CFUDPSocketImpl::CFUDPSocketImpl() :
  m_socket(INVALID_SOCKET),
  m_listenPort(1),
  m_timeout(NO_TIMEOUT)
{
}

//------------------------------------------------------------------------------
bool CFUDPSocketImpl::Bind(int port)
{
	m_listenPort = port;
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_listenPort);
	addr.sin_addr.s_addr = INADDR_ANY;
	return ::bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

//------------------------------------------------------------------------------
bool CFUDPSocketImpl::Create()
{
  m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP); 
  return m_socket != INVALID_SOCKET;
}

//------------------------------------------------------------------------------
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize)
{
	assert(false);
}

//------------------------------------------------------------------------------
int CFUDPSocketImpl::Receive(char* pBuff, const int size)
{
	CFInetAddrImpl addr;
	return Receive(pBuff, size, &addr);
}

//------------------------------------------------------------------------------
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr)
{
	if (m_timeout != NO_TIMEOUT)
	{
		fd_set writeSet;
		FD_ZERO(&writeSet);
		FD_SET(m_socket, &writeSet);
		// Set up the timeout
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = m_timeout * 1000;
		::select(m_socket + 1, NULL, &writeSet, NULL, &timeout);
		if (!FD_ISSET(m_socket, &writeSet))
		{
			return SOCKET_TIMEOUT;
		}
	}
	return ::sendto(m_socket, pBuff, iSize, 0, (sockaddr*)&pInetAddr->m_addr, sizeof(pInetAddr->m_addr));
}

//------------------------------------------------------------------------------
int CFUDPSocketImpl::Receive(char* pBuff, const int size, const CFInetAddrImpl* pInetAddr)
{
	if (m_timeout != NO_TIMEOUT)
	{
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(m_socket, &readSet);
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = m_timeout * 1000;
		::select(m_socket + 1, &readSet, NULL, NULL, &timeout);
		if (!FD_ISSET(m_socket, &readSet))
		{
			return SOCKET_TIMEOUT;
		}
	}
	socklen_t addrLen = sizeof(pInetAddr->m_addr);
	return ::recvfrom(m_socket, pBuff, size, 0, (sockaddr*)&pInetAddr->m_addr, &addrLen);
}

//------------------------------------------------------------------------------
void CFUDPSocketImpl::SetRemoteAddr(CFInetAddrImpl* pAddrRemote)
{
	assert(false);
}

//------------------------------------------------------------------------------
void CFUDPSocketImpl::SetTimeout(DWORD timeout)
{
	m_timeout = timeout;
}

//------------------------------------------------------------------------------
bool CFUDPSocketImpl::IsReceiving() const
{
	assert(false);
}
