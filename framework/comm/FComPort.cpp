#include "FComPort.h"

#if defined(_WIN32)
  #include "../os/WIN32/FComPortImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FComPortImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FComPortImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFComPort::CFComPort() : m_pImpl(new CFComPortImpl())
{
}

//------------------------------------------------------------------------------
CFComPort::~CFComPort()
{
	delete m_pImpl;
}

//------------------------------------------------------------------------------
bool CFComPort::Open(int iPort /* = 1 */, BaudRate brBaud /* = BD_9600 */)
{
  return m_pImpl->Open(iPort, brBaud);
}

//------------------------------------------------------------------------------
int CFComPort::Write(const char *pszBuffer, int iBuffSize)
{
  return m_pImpl->Write(pszBuffer, iBuffSize);
}
