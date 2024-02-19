#include "FBitmap.h"

#if defined(_WIN32) || defined(_WIN32_WCE)
  #include "../os/WIN32/FBitmapImpl.h"
#elif defined (_LINUX)
  #include "../os/LINUX/FBitmapImpl.h"
#elif defined (_PALM_OS)
  #include "../os/PALM_OS/FBitmapImpl.h"
#else
  #error "unknown OS"
#endif

//------------------------------------------------------------------------------
CFBitmap::CFBitmap() : m_pImpl(new CFBitmapImpl())
{
}

//------------------------------------------------------------------------------
CFBitmap::CFBitmap(const CFBitmap& bmp) : m_pImpl(new CFBitmapImpl())
{
  m_pImpl->SetDestBitmap(*bmp.m_pImpl);
}

//------------------------------------------------------------------------------
CFBitmap::CFBitmap(CFBitmapImpl* pImpl) : m_pImpl(pImpl)
{
}

//------------------------------------------------------------------------------
CFBitmap::~CFBitmap()
{
  delete m_pImpl;
}

//------------------------------------------------------------------------------
CFBitmap& CFBitmap::operator=(const CFBitmap& bmp)
{
  m_pImpl->operator=(*bmp.m_pImpl);
  return *this;
}

//------------------------------------------------------------------------------
bool CFBitmap::Load(int wResourceID)
{
  return m_pImpl->Load(wResourceID);
}

//------------------------------------------------------------------------------
bool CFBitmap::Load(const TCHAR* pszFileName)
{
  return m_pImpl->Load(pszFileName);
}

//------------------------------------------------------------------------------
bool CFBitmap::Load(char *pszData, unsigned long dwSize)
{
  return m_pImpl->Load(pszData, dwSize);
}

//------------------------------------------------------------------------------
bool CFBitmap::Create(int width, int height)
{
  return m_pImpl->Create(width, height);
}

//------------------------------------------------------------------------------
void CFBitmap::SetDestBitmap(const CFBitmap& bmp)
{
  m_pImpl->SetDestBitmap(*bmp.m_pImpl);
}

//------------------------------------------------------------------------------
bool CFBitmap::IsValid() const
{
  return m_pImpl->IsValid();
}

//------------------------------------------------------------------------------
char* CFBitmap::GetBits()
{
  return m_pImpl->GetBits();
}

//------------------------------------------------------------------------------
int CFBitmap::GetX() const
{
  return m_pImpl->GetX();
}

//------------------------------------------------------------------------------
int CFBitmap::GetY() const
{
  return m_pImpl->GetY();
}

//------------------------------------------------------------------------------
void CFBitmap::SetX(int posX)
{
  m_pImpl->SetX(posX);
}

//------------------------------------------------------------------------------
void CFBitmap::SetY(int posY)
{
  m_pImpl->SetY(posY);
}

//------------------------------------------------------------------------------
int CFBitmap::GetWidth() const
{
  return m_pImpl->GetWidth();
}

//------------------------------------------------------------------------------
int CFBitmap::GetHeight() const
{
  return m_pImpl->GetHeight();
}

//------------------------------------------------------------------------------
bool CFBitmap::Blit()
{
  return m_pImpl->Blit();
}

//------------------------------------------------------------------------------
bool CFBitmap::Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty)
{
  return m_pImpl->Blit(srcx, srcy, srcw, srch, destx, desty);
}

//------------------------------------------------------------------------------
bool CFBitmap::StretchBlit(int width, int height)
{
  return m_pImpl->StretchBlit(width, height);
}

//------------------------------------------------------------------------------
bool CFBitmap::ClipBlit(int iWidth, int iHeight)
{
  return m_pImpl->ClipBlit(iWidth, iHeight);
}

//------------------------------------------------------------------------------
bool CFBitmap::TransBlit(COLORREF colTrans)
{
  return m_pImpl->TransBlit(colTrans);
}

//------------------------------------------------------------------------------
bool CFBitmap::SaveUnder(const CFBitmap& bmp)
{
  return m_pImpl->SaveUnder(*bmp.m_pImpl);
}

//------------------------------------------------------------------------------
bool CFBitmap::RestoreUnder(const CFBitmap& bmp)
{
  return m_pImpl->RestoreUnder(*bmp.m_pImpl);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetColor(COLORREF col)
{
  return m_pImpl->SetColor(col);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetTextColor(COLORREF col)
{
  return m_pImpl->SetTextColor(col);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetBackgroundColor(COLORREF col)
{
  return m_pImpl->SetBackgroundColor(col);
}

//------------------------------------------------------------------------------
bool CFBitmap::SolidFill(const COLORREF colFill)
{
  return m_pImpl->SolidFill(colFill);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight)
{
  return m_pImpl->DrawFilledRect(iLeft, iTop, iWidth, iHeight);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawPolygon(POINT *points, int count)
{
  return m_pImpl->DrawPolygon(points, count);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(const RECT& rect)
{
  return DrawFilledRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawText(const TCHAR *pszText, RECT& rect)
{
  return m_pImpl->DrawText(pszText, rect);
}

//------------------------------------------------------------------------------
bool CFBitmap::SaveToFile(const TCHAR *pszFileName) const
{
  return m_pImpl->SaveToFile(pszFileName);
}

//------------------------------------------------------------------------------
/* static */ bool CFBitmap::CalcRectForText(const TCHAR *pszText, RECT& rect)
{
  return CFBitmapImpl::CalcRectForText(pszText, rect);
}