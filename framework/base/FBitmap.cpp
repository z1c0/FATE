#include "FBitmap.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
  #include "WIN32/FBitmap.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FBitmapImpl.h"
#elif defined (_PALM_OS)
  #include "PalmOS/FBitmap.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFBitmap::CFBitmap() : m_pImpl(new CFBitmapImpl())
{
  Init();
}

//------------------------------------------------------------------------------
CFBitmap::CFBitmap(const CFBitmap& bmp) : m_pImpl(new CFBitmapImpl())
{
  m_pImpl = bmp.m_pImpl;
  Init();
}

//------------------------------------------------------------------------------
void CFBitmap::Init()
{
  m_posX = 0;
  m_posY = 0;
}

//------------------------------------------------------------------------------
CFBitmap::~CFBitmap()
{
  delete m_pImpl;
}

//------------------------------------------------------------------------------
bool CFBitmap::Create(int width, int height)
{
  return m_pImpl->Create(width, height);
}

//------------------------------------------------------------------------------
bool CFBitmap::Blit()
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::TransBlit(COLORREF colTrans)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::SaveUnder(const CFBitmap& bmp)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::RestoreUnder(const CFBitmap& bmp)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetColor(COLORREF col)
{
  assert(false);
  return col;
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetBackgroundColor(COLORREF col)
{
  assert(false);
  return col;
}

//------------------------------------------------------------------------------
bool CFBitmap::SolidFill(const COLORREF colFill)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawText(const TCHAR *pszText, RECT& rect)
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
bool CFBitmap::SaveToFile(const TCHAR *pszFileName) const
{
  assert(false);
  return false;
}

//------------------------------------------------------------------------------
/* static */ bool CFBitmap::CalcRectForText(const TCHAR *pszText, RECT& rect)
{
  assert(false);
  return false;
}