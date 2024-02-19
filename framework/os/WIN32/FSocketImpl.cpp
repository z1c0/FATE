#include "FSocketImpl.h"
#include "FInetAddrImpl.h"

//--------------------------------------------------------------------------------
CFSocketImpl::CFSocketImpl()
{
  m_hSocket = INVALID_SOCKET; 
  m_dwTimeout = DEFAULT_TIMEOUT; 
  m_listenPort = -1;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Create()
{
  if (m_hSocket != INVALID_SOCKET)
  {
    return(false);
  }
  m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  return m_hSocket != INVALID_SOCKET;
}

//--------------------------------------------------------------------------------
/// Closes an open socket.
/// NOTE: A closed socket cannot be reused again without a call to "Create()".
bool CFSocketImpl::Close()
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false;
  }
	if (closesocket(m_hSocket) == SOCKET_ERROR)
  {
	  return(false);
  }
  m_hSocket= INVALID_SOCKET;
  return true;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(int port)
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false;
  }  
  m_listenPort = port;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_listenPort);
  addr.sin_addr.s_addr = INADDR_ANY;

	return(bind(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Bind(const CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false;
  }
  m_listenPort = ntohs(pInetAddr->sin_port);
  return  ::bind(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Listen()
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	return(listen(m_hSocket, 5) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Accept(CFSocketImpl& sConnect)
{
  sockaddr_in addr;
  int iSize;
  
  if (m_hSocket == INVALID_SOCKET) return(false);
  
  if (m_dwTimeout != NO_TIMEOUT)
  {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};
	  if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

  iSize= sizeof(sockaddr_in);
	sConnect.m_hSocket= accept(m_hSocket, (sockaddr*)&addr, &iSize);
  return(sConnect.m_hSocket != INVALID_SOCKET);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(const CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false;
  }
  return ::connect(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR;
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(char *pAddrStr, USHORT usPort)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
  CFInetAddr addr(pAddrStr, usPort);
	return(connect(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Write(const char* pBuff, const int iSize)
{
	int iBytesSent= 0;
	int iBytesTemp;
	const char* pTemp= pBuff;
	
  do {
	  iBytesTemp= Send(pTemp, iSize - iBytesSent);
    // error occured?
    if (iBytesTemp == SOCKET_ERROR) return(SOCKET_ERROR);
    if (iBytesTemp == SOCKET_TIMEOUT) return(SOCKET_TIMEOUT);

		iBytesSent+= iBytesTemp;
		pTemp+= iBytesTemp;
	} while(iBytesSent < iSize);
	
  return(iBytesSent);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFSocketImpl::Send(const char* pBuff, const int iSize)
{
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);
  
  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};	
    if(select(0, NULL, &fd, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }	
  }
	return(send(m_hSocket, pBuff, iSize, 0));
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFSocketImpl::Receive(char* pBuff, const int iSize)
{
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};	
    if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

	return(recv(m_hSocket, pBuff, iSize, 0));
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::GetRemoteAddr(CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false; 
  }	
	int iSize= sizeof(SOCKADDR);
	return(getpeername(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::GetInetAddr(CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false; 
  }		
	int iSize= sizeof(SOCKADDR);
	return(getsockname(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
/// Is there a client who wants to connect?
bool CFSocketImpl::IsClientConnecting() const
{
  if (m_hSocket == INVALID_SOCKET)
  {
    return false;
  }
  else
  {
    FD_SET fd= {1, m_hSocket};
    TIMEVAL tv= {0, 0};	
    return(select(0, &fd, NULL, NULL, &tv) == 1);
  }
}

//--------------------------------------------------------------------------------
/// Is another socket sending currently to this one?
bool CFSocketImpl::IsReceiving() const
{
  if (m_hSocket == INVALID_SOCKET) 
  {
    return false;
  }
  else
  {
    FD_SET fd= {1, m_hSocket};
    TIMEVAL tv= {0, 0};	
    return(select(0, &fd, NULL, NULL, &tv) == 1);
  }
}


//--------------------------------------------------------------------------------
/* static */ bool CFSocketImpl::InitSocketLibrary()
{
  WORD vr;
  WSADATA wsaData;
  vr = MAKEWORD(1, 1);
  return !::WSAStartup(vr, &wsaData);
}

//--------------------------------------------------------------------------------
/* static */ bool CFSocketImpl::CleanupSocketLibrary()
{
  ::WSACleanup();
  return true;
}

//--------------------------------------------------------------------------------
/// If hostname cannot be resolved, empty CSockAddr-object is returned.
/* static */ CFInetAddrImpl* CFSocketImpl::GetHostByName(const char* pHostName, int port /* = 0 */)
{
  hostent* pHostEnt = ::gethostbyname(pHostName);
	if(!pHostEnt)
  {
		return new CFInetAddrImpl();
	}
	ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
  CFInetAddrImpl* pAddr = new CFInetAddrImpl();
	pAddr->sin_family = AF_INET;
	pAddr->sin_port= htons(port);
	pAddr->sin_addr.s_addr= *pulAddr; // address is already in network byte order
	return pAddr;
}

//--------------------------------------------------------------------------------
/// Returns NULL if hostname cannot be resolved.
/* static */ const char* CFSocketImpl::GetHostByAddr(const CFInetAddrImpl* pInetAddr)
{
	hostent* pHostEnt= gethostbyaddr((char*)&((LPSOCKADDR_IN)pInetAddr)->sin_addr.s_addr, 4, PF_INET);
	if(!pHostEnt)
  {
		return(NULL);
	}
	return(pHostEnt->h_name); // caller shouldn't delete this memory
}

