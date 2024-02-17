#include "FUDPSocketImpl.h"

//--------------------------------------------------------------------------------
/// Creates a UDP socket.
bool CFUDPSocketImpl::Create()
{
  Err err;

  if (m_refSock != INVALID_SOCKET) return(false);
  m_refSock= NetLibSocketOpen(CFInetAddrImpl::m_uiLibRefNum, netSocketAddrINET, netSocketTypeDatagram,
                              netSocketProtoIPUDP, 0, &err);
  return(m_refSock != INVALID_SOCKET);
}


//--------------------------------------------------------------------------------
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize)
{
  /*
  if (m_pAddrRemote) {
    return(Send(pBuff, iSize, m_pAddrRemote));
  } else {
    return(SOCKET_ERROR);
  }
  */
}

//--------------------------------------------------------------------------------
int CFUDPSocketImpl::Receive(char* pBuff, const int iSize)
{
  /*
  if (!m_pAddrRemote) {
    // create a dummy address
    m_pAddrRemote= new CFInetAddr();
  }
  return(Receive(pBuff, iSize, m_pAddrRemote));
  */
}

//--------------------------------------------------------------------------------
/// Receive UDP datagrams.
/// Returns the number of bytes received, or SOCKET_TIMEOUT in case of a timeout
/// or SOCKET_ERROR in case of a problem.
int CFUDPSocketImpl::Receive(char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr)
{
  /*
  if (m_refSock == INVALID_SOCKET) return(SOCKET_ERROR);

  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_refSock};
	  TIMEVAL tv= {m_dwTimeout, 0};
	  if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

	// input buffer should be big enough for the entire datagram
	int iFromSize= sizeof(SOCKADDR);
	return(recvfrom(m_refSock, pBuff, iSize, 0, (LPSOCKADDR)pInetAddr, &iFromSize));
  */
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFUDPSocketImpl::Send(const char* pBuff, const int iSize, const CFInetAddrImpl* pInetAddr)
{
  if (m_refSock == INVALID_SOCKET) return(SOCKET_ERROR);

  Err err;
  const NetSocketAddrINType *pAddr= &(pInetAddr->m_addr);

	return(NetLibSend(CFInetAddrImpl::m_uiLibRefNum, m_refSock, (void*)pBuff, iSize, 0, 
                    (void*)pAddr, sizeof(pAddr), 0, &err));
}

//--------------------------------------------------------------------------------
void CFUDPSocketImpl::EnableBroadcast()
{
  /*
  const char on= 1;
  setsockopt(m_refSock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
  */
}

//------------------------------------------------------------------------------
void CFUDPSocketImpl::JoinMultiCastGroup(const char *pszGroupAddress)
{
  /*
  struct ip_mreq mreq= {0};
  
  mreq.imr_multiaddr.s_addr= inet_addr(pszGroupAddress);
  mreq.imr_interface.s_addr= INADDR_ANY;
  setsockopt(m_refSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
  */
}
