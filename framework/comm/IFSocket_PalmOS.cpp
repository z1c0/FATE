// IFSocket.cpp: implementation of the IFSocket class.
//////////////////////////////////////////////////////////////////////
#include "IFSocket_PalmOS.h"

//------------------------------------------------------------------------------
UInt16 IFSocket::m_uiLibRefNum= 0;
UInt16 CFInetAddr::m_uiLibRefNum= 0;

//--------------------------------------------------------------------------------
/// Closes an open socket.
/// NOTE: A closed socket cannot be reused again without a call to "Create()".
bool IFSocket::Close()
{

  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
	if(closesocket(m_hSocket) == SOCKET_ERROR) {
	  return(false);
  }
  m_hSocket= INVALID_SOCKET;
  return(true);
  */
  return(false);
}

//--------------------------------------------------------------------------------
bool IFSocket::Bind(const CFInetAddr* pInetAddr)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);

  m_iListenPort= ntohs(pInetAddr->sin_port);
	return(bind(m_hSocket, (LPSOCKADDR)pInetAddr, sizeof(SOCKADDR)) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
bool IFSocket::Bind(int iPort)
{
  /*
  if (m_hSocket == INVALID_SOCKET) return(false);
  
  m_iListenPort= usPort;
  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(usPort);
  addr.sin_addr.s_addr = INADDR_ANY;

	return(bind(m_hSocket, (LPSOCKADDR)&addr, sizeof(SOCKADDR)) != SOCKET_ERROR);
  */
  return(false);
}

//--------------------------------------------------------------------------------
/// Is another socket sending currently to this one?
bool IFSocket::IsReceiving()
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
/// Static methods:

//--------------------------------------------------------------------------------
/// If hostname cannot be resolved, empty CSockAddr-object is returned.
CFInetAddr* IFSocket::GetHostByName(const char* pHostName, const int iPort /* = 0 */)
{
  CFInetAddr *pAddr;
  /*
	hostent* pHostEnt = gethostbyname(pHostName);
	if(!pHostEnt) {
		return(CFInetAddr());
	}
	ULONG* pulAddr= (ULONG*) pHostEnt->h_addr_list[0];
	SOCKADDR_IN addrTemp;
	addrTemp.sin_family= AF_INET;
	addrTemp.sin_port= htons(ushPort);
	addrTemp.sin_addr.s_addr= *pulAddr; // address is already in network byte order
	return(addrTemp);
  */
  return(pAddr);
}

//--------------------------------------------------------------------------------
/// Returns NULL if hostname cannot be resolved.
const char* IFSocket::GetHostByAddr(const CFInetAddr* pInetAddr)
{
  /*
	hostent* pHostEnt= gethostbyaddr((char*)&((LPSOCKADDR_IN)pInetAddr)->sin_addr.s_addr,
                                   4, PF_INET);
	if(!pHostEnt) {
		return(NULL);
	}
	return(pHostEnt->h_name); // caller shouldn't delete this memory
  */
  return(NULL);
}


//--------------------------------------------------------------------------------
/// Initializes the PalmOS socket library.
bool IFSocket::InitSocketLibrary()
{
  Err err;
  UInt16 uiIfErrs;

  err= SysLibFind("Net.lib", &m_uiLibRefNum);
  err= NetLibOpen(m_uiLibRefNum, &uiIfErrs);
  CFInetAddr::m_uiLibRefNum= m_uiLibRefNum;

  return(err == 0);
}

//--------------------------------------------------------------------------------
/// De-initializes the Winsock library.
bool IFSocket::CleanupSocketLibrary()
{
  return(true);
}