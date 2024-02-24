#include "FUDPSocketImpl.h"

//--------------------------------------------------------------------------------
CFUDPSocketImpl::CFUDPSocketImpl()
{
  m_pAddrRemote = NULL;
  m_hSocket = INVALID_SOCKET; 
  m_dwTimeout = NO_TIMEOUT; 
  m_listenPort = -1;
}

//--------------------------------------------------------------------------------
/// Creates a UDP socket.
bool CFUDPSocketImpl::Create()
{
  if (m_hSocket != INVALID_SOCKET) return(FALSE);
  m_hSocket= socket(AF_INET, SOCK_DGRAM, IPPROTO_IP); 
  return(m_hSocket != INVALID_SOCKET);
}

//--------------------------------------------------------------------------------
bool CFUDPSocketImpl::Bind(int port)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
  
  m_listenPort = port;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(m_listenPort);
  addr.sin_addr.s_addr = INADDR_ANY;

	return(bind(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize)
{
  if (m_pAddrRemote) {
    return(Send(pBuff, iSize, m_pAddrRemote));
  } else {
    return(SOCKET_ERROR);
  }
}

//--------------------------------------------------------------------------------
int CFUDPSocketImpl::Receive(char* pBuff, const int iSize)
{
  if (!m_pAddrRemote)
  {
    // create a dummy address
    m_pAddrRemote= new CFInetAddrImpl();
  }
  return(Receive(pBuff, iSize, m_pAddrRemote));
}

//--------------------------------------------------------------------------------
/// Receive UDP datagrams.
/// Returns the number of bytes received, or SOCKET_TIMEOUT in case of a timeout
/// or SOCKET_ERROR in case of a problem.
int CFUDPSocketImpl::Receive(char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};
	  if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

	// input buffer should be big enough for the entire datagram
	int iFromSize= sizeof(SOCKADDR);
	return(recvfrom(m_hSocket, pBuff, iSize, 0, (LPSOCKADDR)pInetAddr, &iFromSize));
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

  if (m_dwTimeout != NO_TIMEOUT) {	
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};
	  if(select(0, NULL, &fd, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

	return(sendto(m_hSocket, pBuff, iSize, 0, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)));
}

//--------------------------------------------------------------------------------
void CFUDPSocketImpl::EnableBroadcast()
{
  const char on= 1;
  setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
}

//------------------------------------------------------------------------------
void CFUDPSocketImpl::JoinMultiCastGroup(const char *pszGroupAddress)
{
  struct ip_mreq mreq= {0};
  
  mreq.imr_multiaddr.s_addr= inet_addr(pszGroupAddress);
  mreq.imr_interface.s_addr= INADDR_ANY;
  setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
}

//--------------------------------------------------------------------------------
/// Is another socket sending currently to this one?
bool CFUDPSocketImpl::IsReceiving() const
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
