#include "FSocketImpl.h"

//--------------------------------------------------------------------------------
CFSocketImpl::CFSocketImpl() : m_socket(INVALID_SOCKET), m_listenPort(-1), m_timeout(NO_TIMEOUT)
{	
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Create()
{
	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	return m_socket != INVALID_SOCKET;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Close()
{
	return ::close(m_socket) == 0;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(int port)
{
	m_listenPort = port;
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_listenPort);
	addr.sin_addr.s_addr = INADDR_ANY;
	return ::bind(m_socket, (struct sockaddr*)&addr, sizeof(addr)) == 0;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(const CFInetAddrImpl* pInetAddr)
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Listen()
{
	return ::listen(m_socket, 5) == 0;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Accept(CFSocketImpl& sock)
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
			return false;
		}
	}
	sockaddr_in addr;
	socklen_t size = sizeof(sockaddr_in);
	sock.m_socket = ::accept(m_socket, (sockaddr*)&addr, &size);
	return sock.m_socket != INVALID_SOCKET;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(const CFInetAddrImpl* pInetAddr)
{
	return ::connect(m_socket, (sockaddr*)&pInetAddr->m_addr, sizeof(pInetAddr->m_addr)) == 0;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(char *addrStr, int port)
{
	CFInetAddrImpl inetAddr(addrStr, port);
	return Connect(&inetAddr);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Send(const char* pBuff, const int size)
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
	int iBytesSent = 0;
	const char* pTemp= pBuff;
	do
	{
		int tmp = ::send(m_socket, pTemp, size - iBytesSent, 0);
		if (tmp == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		iBytesSent += tmp;
		pTemp += tmp;
	}
	while (iBytesSent < size);
	
	return iBytesSent;
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Receive(char* pBuff, const int size)
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
	return ::recv(m_socket, pBuff, size, 0);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::IsClientConnecting() const
{
	assert(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::IsReceiving() const
{
	assert(false);
}

//--------------------------------------------------------------------------------
/* static */ CFInetAddrImpl* CFSocketImpl::GetHostByName(const char* pHostName, int port)
{
	assert(false);
}

//--------------------------------------------------------------------------------
/* static */ const char* CFSocketImpl::GetHostByAddr(const CFInetAddrImpl* pInetAddr)
{
	assert(false);
}

