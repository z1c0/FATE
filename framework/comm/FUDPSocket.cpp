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
bool CFUDPSocket::Create()
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
int CFUDPSocket::Send(const char* pBuff, const int iSize, const CFInetAddr* pInetAddr)
{
  assert(false);
  return SOCKET_ERROR;
}