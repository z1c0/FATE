#include "FSystemImpl.h"
#include "../../base/FBitmap.h"
#include <commctrl.h>

//------------------------------------------------------------------------------
CFSystemImpl::CFSystemImpl(HWND hWnd, int nWidth, int nHeight)
{
  m_hWnd = hWnd;
  m_nWidth = nWidth;
  m_nHeight = nHeight;

  m_hdc = ::GetDC(hWnd);
  ::srand(GetTickCount());
}

//------------------------------------------------------------------------------
CFSystemImpl::~CFSystemImpl()
{
  if (m_hdc)
  {
    ::ReleaseDC(m_hWnd, m_hdc);
  }
}

//------------------------------------------------------------------------------
CFBitmapImpl* CFSystemImpl::CreateDoubleBuffer() const
{
  CFBitmapImpl* doubleBuffer = new CFBitmapImpl();
  doubleBuffer->m_hDestDC = m_hdc;
  doubleBuffer->Create(GetWidth(), GetHeight());
  doubleBuffer->SolidFill(0);
  return doubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystemImpl::RenderDoubleBuffer(CFBitmapImpl& doubleBuffer)
{
  doubleBuffer.Blit();
}

//------------------------------------------------------------------------------
void CFSystemImpl::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  ::PostMessage(m_hWnd, iEventID, iComponentID, (LPARAM)pCustomData);
}

//------------------------------------------------------------------------------
bool CFSystemImpl::ShutDownSystem()
{
  ::DestroyWindow(m_hWnd);
  return true;
}

//------------------------------------------------------------------------------
void CFSystemImpl::Sleep(int milliSeconds)
{
  ::Sleep(milliSeconds);
}

//------------------------------------------------------------------------------
void CFSystemImpl::ForceRedraw()
{
  ::InvalidateRect(m_hWnd, NULL, FALSE);
}

//------------------------------------------------------------------------------
void CFSystemImpl::AddTimer(unsigned long id, int interval)
{
  ::SetTimer(m_hWnd, id, interval, NULL);
}

//------------------------------------------------------------------------------
void CFSystemImpl::ShowError(const TCHAR* msg)
{
  ::MessageBox(m_hWnd, msg, TEXT("Error"), MB_OK); 
}

//------------------------------------------------------------------------------
void CFSystemImpl::DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal)
{
  SHFILEINFO shfi;
  HANDLE h = (HANDLE)SHGetFileInfo(pszFilePath, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
  ::ImageList_Draw((HIMAGELIST)h, shfi.iIcon, bmp.m_hSourceDC, x, y, normal ? ILD_NORMAL : ILD_BLEND50);
}

//--------------------------------------------------------------------------------
void CFSystemImpl::GetPathToApplication(TCHAR *pszAppPath)
{
  TCHAR *pszDiff;
  int iPos;
  GetModuleFileName(NULL, pszAppPath, MAX_PATH);
    
  pszDiff= _tcsrchr(pszAppPath, TEXT('\\'));
  if (pszDiff)
  {
    iPos= pszDiff - pszAppPath + 1;
    pszAppPath[iPos] = 0;
  }
  else
  {
    pszAppPath[0]= 0;
  }
}

//--------------------------------------------------------------------------------
unsigned int CFSystemImpl::GetTicks()
{
  return ::GetTickCount();
}

//--------------------------------------------------------------------------------
int CFSystemImpl::GetRandomNumber(int max)
{
  return rand() % max;
}
