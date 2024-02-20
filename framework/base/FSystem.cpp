#include "FSystem.h"

#if defined(_WIN32_WCE)
  #include "../os/WIN32_WCE/FSystemImpl.h"
#elif defined (_WIN32)
  #include "../os/WIN32/FSystemImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FSystemImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FSystemImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFSystem::CFSystem(CFSystemImpl* pImpl) : m_pImpl(pImpl)
{
  m_pDoubleBuffer = new CFBitmap(m_pImpl->CreateDoubleBuffer());
}

//------------------------------------------------------------------------------
CFSystem::~CFSystem()
{
  delete m_pDoubleBuffer;
  delete m_pImpl;
}

//------------------------------------------------------------------------------
int CFSystem::GetWidth() const
{
  return m_pImpl->GetWidth();
}

//------------------------------------------------------------------------------
int CFSystem::GetHeight() const
{
  return m_pImpl->GetHeight();
}

//------------------------------------------------------------------------------
bool CFSystem::ShutDownSystem()
{
  return m_pImpl->ShutDownSystem();
}

//------------------------------------------------------------------------------
void CFSystem::ShowError(const TCHAR* msg)
{
  m_pImpl->ShowError(msg);
}

//------------------------------------------------------------------------------
void CFSystem::RenderDoubleBuffer()
{
  m_pImpl->RenderDoubleBuffer(*m_pDoubleBuffer->m_pImpl);
}

//------------------------------------------------------------------------------
void CFSystem::ForceRedraw()
{
  m_pImpl->ForceRedraw();
}

//------------------------------------------------------------------------------
CFBitmap& CFSystem::GetDoubleBuffer()
{
  return *m_pDoubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystem::DrawFileIcon(CFBitmap& bmp, const TCHAR *pszFilePath, int x, int y, bool normal)
{
  m_pImpl->DrawFileIcon(*bmp.m_pImpl, pszFilePath, x, y, normal);
}

//------------------------------------------------------------------------------
void CFSystem::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  m_pImpl->QueueEvent(iEventID, iComponentID, pCustomData);
}

//------------------------------------------------------------------------------
void CFSystem::AddTimer(unsigned long id, int interval)
{
  m_pImpl->AddTimer(id, interval);
}

//------------------------------------------------------------------------------
void CFSystem::Sleep(int milliSeconds)
{
  m_pImpl->Sleep(milliSeconds);
}

//------------------------------------------------------------------------------
bool CFSystem::EnableSuspend(bool suspend)
{
  return m_pImpl->EnableSuspend(suspend);
}

//------------------------------------------------------------------------------
/* static */ int CFSystem::GetDefaultWidth()
{
  return CFSystemImpl::GetDefaultWidth();
}

//------------------------------------------------------------------------------
/* static */ int CFSystem::GetDefaultHeight()
{
  return CFSystemImpl::GetDefaultHeight();
}

//------------------------------------------------------------------------------
/* static */ const TCHAR* CFSystem::GetDirectorySeparator()
{
  return CFSystemImpl::GetDirectorySeparator();
}

//------------------------------------------------------------------------------
/* static */ void CFSystem::GetPathToApplication(TCHAR *pszAppPath)
{
  CFSystemImpl::GetPathToApplication(pszAppPath);
}

//------------------------------------------------------------------------------
/* static */ unsigned int CFSystem::GetTicks()
{
  return CFSystemImpl::GetTicks();
}

//------------------------------------------------------------------------------
/* static */ int CFSystem::GetRandomNumber(int max)
{
  return CFSystemImpl::GetRandomNumber(max);
}