#include "FSocket_WIN32.h"


//--------------------------------------------------------------------------------
bool CFSocket::Create()
{
  if (m_hSocket != INVALID_SOCKET) return(false);
  m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  return(m_hSocket != INVALID_SOCKET);
}


//--------------------------------------------------------------------------------
bool CFSocket::Listen()
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	return(listen(m_hSocket, 5) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
int CFSocket::Accept(CFSocket& sConnect)
{
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
}

//--------------------------------------------------------------------------------
bool CFSocket::Connect(const CFInetAddr* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	return(connect(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
bool CFSocket::Connect(char *pAddrStr, USHORT usPort)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
  CFInetAddr addr(pAddrStr, usPort);
	return(connect(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
int CFSocket::Write(const char* pBuff, const int iSize)
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
int CFSocket::Send(const char* pBuff, const int iSize)
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
int CFSocket::Receive(char* pBuff, const int iSize)
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
bool CFSocket::GetRemoteAddr(CFInetAddr* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	
	int iSize= sizeof(SOCKADDR);
	return(getpeername(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
bool CFSocket::GetInetAddr(CFInetAddr* pInetAddr)
{
  if (m_hSocket == INVALID_SOCKET) return(false);
	
	int iSize= sizeof(SOCKADDR);
	return(getsockname(m_hSocket, (LPSOCKADDR)pInetAddr, &iSize) != SOCKET_ERROR);
}

//--------------------------------------------------------------------------------
/// Is there a client who wants to connect?
bool CFSocket::IsClientConnecting()
{
  if (m_hSocket == INVALID_SOCKET) return(false);
  else {
    FD_SET fd= {1, m_hSocket};
    TIMEVAL tv= {0, 0};	
    return(select(0, &fd, NULL, NULL, &tv) == 1);
  }
}

