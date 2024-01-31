#include "UDPSocket.h"

//--------------------------------------------------------------------------------
bool CUDPSocket::m_bWinsockInitialized= false;


//--------------------------------------------------------------------------------
CUDPSocket::CUDPSocket()
{
  m_hSocket= INVALID_SOCKET; 
  m_dwTimeout= DEFAULT_TIMEOUT; 
  m_iListenPort= -1;
}

//--------------------------------------------------------------------------------
CUDPSocket::~CUDPSocket()
{
  if ((m_hSocket)&&(m_hSocket != INVALID_SOCKET)) 
    Close();
}

//--------------------------------------------------------------------------------
/// Creates a UDP socket.
bool CUDPSocket::Create()
{
  // at first instantiation, init winsock library
  if (!m_bWinsockInitialized) {
    // windows sockets stuff
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    m_bWinsockInitialized= true;
  }
  
  if (m_hSocket != INVALID_SOCKET) return(false);
  m_hSocket= socket(AF_INET, SOCK_DGRAM, IPPROTO_IP); 
  return(m_hSocket != INVALID_SOCKET);
}

//--------------------------------------------------------------------------------
/// Receive UDP datagrams.
/// Returns the number of bytes received, or SOCKET_TIMEOUT in case of a timeout
/// or SOCKET_ERROR in case of a problem.
int CUDPSocket::Receive(char* pBuff, const int iSize, LPINETADDR pInetAddr)
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
int CUDPSocket::Send(const char* pBuff, const int iSize, LPCINETADDR pInetAddr)
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
/// Closes an open socket.
/// NOTE: A closed socket cannot be reused again without a call to "Create()".
bool CUDPSocket::Close()
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	if(closesocket(m_hSocket) == SOCKET_ERROR) {
	  return(false);
  }
  m_hSocket= INVALID_SOCKET;
  return(true);
}

//--------------------------------------------------------------------------------
bool CUDPSocket::Bind(LPCINETADDR pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);

  m_iListenPort= ntohs(pInetAddr->sin_port);
	return(bind(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
bool CUDPSocket::Bind(USHORT usPort)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
  
  m_iListenPort= usPort;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(usPort);
  addr.sin_addr.s_addr = INADDR_ANY;

	return(bind(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
void CUDPSocket::EnableBroadcast()
{
  const char on= 1;
  setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
}

//------------------------------------------------------------------------------
void CUDPSocket::JoinMultiCastGroup(const char *pszGroupAddress)
{
  struct ip_mreq mreq= {0};
  
  mreq.imr_multiaddr.s_addr= inet_addr(pszGroupAddress);
  mreq.imr_interface.s_addr= INADDR_ANY;
  setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
}
