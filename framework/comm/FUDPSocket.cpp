#include "FUDPSocket.h"

#if defined(_WIN32_WCE)
  #include "WIN32_WCE/FSystem.h"
#elif defined (_WIN32)
  #include "WIN32/FSystem.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FUDPSocketImpl.h"
#elif defined (_PALM_OS)
  #include "PalmOS/FSystem.h"
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