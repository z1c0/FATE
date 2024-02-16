#include "FSocket.h"

#if defined(_WIN32_WCE)
  #include "WIN32_WCE/FSystem.h"
#elif defined (_WIN32)
  #include "WIN32/FSystem.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FSocketImpl.h"
#elif defined (_PALM_OS)
  #include "PalmOS/FSystem.h"
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
  assert(false);
  return false;
}

//--------------------------------------------------------------------------------
bool CFSocket::Bind(int port)
{
  assert(false);
  return false;
}

//--------------------------------------------------------------------------------
bool CFSocket::Listen()
{
  assert(false);
  return false;
}

//--------------------------------------------------------------------------------
int CFSocket::Receive(char* pBuff, const int size)
{
  assert(false);
  return SOCKET_ERROR;
}

//--------------------------------------------------------------------------------
int CFSocket::Send(const char* pBuff, const int size)
{
  assert(false);
  return SOCKET_ERROR;
}

//--------------------------------------------------------------------------------
int CFSocket::Write(const char* pBuff, const int size)
{
  assert(false);
  return SOCKET_ERROR;
}

//--------------------------------------------------------------------------------
bool CFSocket::IsClientConnecting()
{
  assert(false);
  return false;
}

//--------------------------------------------------------------------------------
bool CFSocket::IsReceiving()
{
  assert(false);
  return false;
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
