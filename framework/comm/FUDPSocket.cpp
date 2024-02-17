#include "FUDPSocket.h"

#if defined (_WIN32)
  #include "../os/WIN32/FUDPSocketImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FUDPSocketImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FUDPSocketImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFUDPSocket::CFUDPSocket(CFInetAddr *pAddr /* = NULL */) : m_pImpl(new CFUDPSocketImpl())
{
  if (pAddr)
  {
    SetRemoteAddr(pAddr);
  }
}

//------------------------------------------------------------------------------
CFUDPSocket::~CFUDPSocket()
{
  delete m_pImpl;
}

//------------------------------------------------------------------------------
bool CFUDPSocket::Create()
{
  return m_pImpl->Create();
}

//------------------------------------------------------------------------------
bool CFUDPSocket::Bind(int port)
{
  return m_pImpl->Bind(port);
}

//------------------------------------------------------------------------------
int CFUDPSocket::Send(const char* pBuff, const int iSize, const CFInetAddr* pInetAddr)
{
  return m_pImpl->Send(pBuff, iSize, pInetAddr->m_pImpl);
}

//------------------------------------------------------------------------------
int CFUDPSocket::Receive(char* pBuff, const int iSize)
{
  return m_pImpl->Receive(pBuff, iSize);
}

//------------------------------------------------------------------------------
int CFUDPSocket::Receive(char* pBuff, const int iSize, const CFInetAddr* pInetAddr)
{
  return m_pImpl->Receive(pBuff, iSize, pInetAddr->m_pImpl);
}

//------------------------------------------------------------------------------
void CFUDPSocket::SetRemoteAddr(CFInetAddr* pAddrRemote)
{
  m_pImpl->SetRemoteAddr(pAddrRemote->m_pImpl);
}

//------------------------------------------------------------------------------
void CFUDPSocket::SetTimeout(DWORD dwTimeout)
{
  m_pImpl->SetTimeout(dwTimeout);
}

//------------------------------------------------------------------------------
bool CFUDPSocket::IsReceiving() const
{
  return m_pImpl->IsReceiving();
}

