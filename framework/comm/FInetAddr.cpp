#include "FInetAddr.h"

#if defined(_WIN32)
  #include "../os/WIN32/FInetAddrImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FInetAddrImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FInetAddrImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFInetAddr::CFInetAddr() : m_pImpl(new CFInetAddrImpl())
{
}

//------------------------------------------------------------------------------
CFInetAddr::CFInetAddr(const char* ip, int port) : m_pImpl(new CFInetAddrImpl(ip, port))
{
}

//------------------------------------------------------------------------------
CFInetAddr::CFInetAddr(const wchar_t* ip, int port) : m_pImpl(new CFInetAddrImpl(ip, port))
{
}


//------------------------------------------------------------------------------
CFInetAddr::~CFInetAddr()
{
  delete m_pImpl;
}