#include "FSystem.h"

#if defined(_WIN32_WCE)
  #include "WIN32_WCE/FSystem.h"
#elif defined (_WIN32)
  #include "WIN32/FSystem.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FSystemImpl.h"
#elif defined (_PALM_OS)
  #include "PalmOS/FSystem.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFSystem::CFSystem(CFSystemImpl* pImpl) : m_pImpl(pImpl)
{
}

//------------------------------------------------------------------------------
CFSystem::~CFSystem()
{
  delete m_pImpl;
}

//------------------------------------------------------------------------------
bool CFSystem::ShutDownSystem()
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
void CFSystem::RenderDoubleBuffer()
{
  assert(false);
}

//------------------------------------------------------------------------------
void CFSystem::ForceRedraw()
{
  assert(false);
}

//------------------------------------------------------------------------------
CFBitmap& CFSystem::GetDoubleBuffer()
{
  return m_doubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystem::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  assert(false);
}

//------------------------------------------------------------------------------
/* static */ void CFSystem::GetPathToApplication(TCHAR *pszAppPath)
{
  CFSystemImpl::GetPathToApplication(pszAppPath);
}