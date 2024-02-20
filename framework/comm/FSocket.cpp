#include "FSocket.h"
#include "FInetAddr.h"

#if defined(_WIN32)
  #include "../os/WIN32/FSocketImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FSocketImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FSocketImpl.h"
#else
  #error "unknown OS"
#endif

//--------------------------------------------------------------------------------
CFSocket::CFSocket() : m_pImpl(new CFSocketImpl())
{
}

//--------------------------------------------------------------------------------
CFSocket::~CFSocket()
{
  delete m_pImpl;
}

//--------------------------------------------------------------------------------
bool CFSocket::Create()
{
  return m_pImpl->Create();
}

//--------------------------------------------------------------------------------
bool CFSocket::Close()
{
  return m_pImpl->Close();
}

//--------------------------------------------------------------------------------
bool CFSocket::Bind(int port)
{
  return m_pImpl->Bind(port);
}

//--------------------------------------------------------------------------------
bool CFSocket::Bind(const CFInetAddr* pInetAddr)
{
  return m_pImpl->Bind(pInetAddr->m_pImpl);
}

//--------------------------------------------------------------------------------
bool CFSocket::Listen()
{
  return m_pImpl->Listen();
}

//--------------------------------------------------------------------------------
bool CFSocket::Accept(CFSocket& sock)
{
  return m_pImpl->Accept(*sock.m_pImpl);
}

//--------------------------------------------------------------------------------
bool CFSocket::Connect(const CFInetAddr* pInetAddr)
{
  return m_pImpl->Connect(pInetAddr->m_pImpl);
}

//--------------------------------------------------------------------------------
bool CFSocket::Connect(char *pAddrStr, int port)
{
  return m_pImpl->Connect(pAddrStr, port);
}

//--------------------------------------------------------------------------------
int CFSocket::Receive(char* pBuff, const int size)
{
  return m_pImpl->Receive(pBuff, size);
}

//--------------------------------------------------------------------------------
int CFSocket::Send(const char* pBuff, const int size)
{
  return m_pImpl->Send(pBuff, size);
}

//--------------------------------------------------------------------------------
bool CFSocket::IsClientConnecting() const
{
  return m_pImpl->IsClientConnecting();
}

//--------------------------------------------------------------------------------
bool CFSocket::IsReceiving() const
{
  return m_pImpl->IsReceiving();
}

//--------------------------------------------------------------------------------
int CFSocket::GetListenPort() const
{
  return m_pImpl->GetListenPort();
}

//------------------------------------------------------------------------------
/// User can specify a timeout for send/receive operations. Timeout is specified
/// in seconds. The constant NO_TIMEOUT means, that operations will block.
void CFSocket::SetTimeout(DWORD dwTimeout)
{
  m_pImpl->SetTimeout(dwTimeout);
}

//--------------------------------------------------------------------------------
/* static */ bool CFSocket::InitSocketLibrary()
{
  return CFSocketImpl::InitSocketLibrary();
}

//--------------------------------------------------------------------------------
/* static */ bool CFSocket::CleanupSocketLibrary()
{
  return CFSocketImpl::CleanupSocketLibrary();
}

//--------------------------------------------------------------------------------
/* static */ CFInetAddr* CFSocket::GetHostByName(const char* pHostName, int port /* = 0 */)
{
  return new CFInetAddr(CFSocketImpl::GetHostByName(pHostName, port));
}

//--------------------------------------------------------------------------------
/* static */ const char* CFSocket::GetHostByAddr(const CFInetAddr* pInetAddr)
{
  return CFSocketImpl::GetHostByAddr(pInetAddr->m_pImpl);
}
