#include "FSocketImpl.h"

UInt16 CFInetAddrImpl::m_uiLibRefNum= 0;

//--------------------------------------------------------------------------------
bool CFSocketImpl::Create()
{
  /*
  if (m_hSocket != INVALID_SOCKET) return(false);
  m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  return(m_hSocket != INVALID_SOCKET);
  */
  return(false);
}


//--------------------------------------------------------------------------------
bool CFSocketImpl::Listen()
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
	return(listen(m_hSocket, 5) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Accept(CFSocketImpl& sConnect)
{
  /*
  sockaddr_in addr;
  int iSize;
  
  if (m_hSocket == INVALID_SOCKET) return(false);
  
  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};
	  if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

  iSize= sizeof(sockaddr_in);
	sConnect.m_hSocket= accept(m_hSocket, (sockaddr*)&addr, &iSize);
  return(sConnect.m_hSocket != INVALID_SOCKET);
  */
  return(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(const CFInetAddrImpl* pInetAddr)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
	return(connect(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::Connect(char *pAddrStr, int iPort)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
  CFInetAddr addr(pAddrStr, usPort);
	return(connect(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
int CFSocketImpl::Write(const char* pBuff, const int iSize)
{
  /*
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
  */
  return(0);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFSocketImpl::Send(const char* pBuff, const int iSize)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);
  
  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};	
    if(select(0, NULL, &fd, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }	
  }
	return(send(m_hSocket, pBuff, iSize, 0));
  */
  return(0);
}

//--------------------------------------------------------------------------------
/// Return values:
/// SOCKET_TIMEOUT indicates timeout 
/// SOCKET_ERROR in case of a problem.
int CFSocketImpl::Receive(char* pBuff, const int iSize)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(SOCKET_ERROR);

  if (m_dwTimeout != NO_TIMEOUT) {
    FD_SET fd= {1, m_hSocket};
	  TIMEVAL tv= {m_dwTimeout, 0};	
    if(select(0, &fd, NULL, NULL, &tv) == 0) {
		  return(SOCKET_TIMEOUT);
	  }
  }

	return(recv(m_hSocket, pBuff, iSize, 0));
  */
  return(0);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::GetRemoteAddr(CFInetAddrImpl* pInetAddr)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
	
	int iSize= sizeof(SOCKADDR);
	return(getpeername(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
bool CFSocketImpl::GetInetAddr(CFInetAddrImpl* pInetAddr)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
	
	int iSize= sizeof(SOCKADDR);
	return(getsockname(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
/// Is there a client who wants to connect?
bool CFSocketImpl::IsClientConnecting() const
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
  else {
    FD_SET fd= {1, m_hSocket};
    TIMEVAL tv= {0, 0};	
    return(select(0, &fd, NULL, NULL, &tv) == 1);
  }
  */
  return(false);
}

//--------------------------------------------------------------------------------
/// Initializes the PalmOS socket library.
bool CFSocketImpl::InitSocketLibrary()
{
  Err err;
  UInt16 uiIfErrs;

  err= SysLibFind("Net.lib", &CFInetAddrImpl::m_uiLibRefNum);
  err= NetLibOpen(CFInetAddrImpl::m_uiLibRefNum, &uiIfErrs);

  return(err == 0);
}

//--------------------------------------------------------------------------------
/// De-initializes the Winsock library.
bool CFSocketImpl::CleanupSocketLibrary()
{
  return(true);
}