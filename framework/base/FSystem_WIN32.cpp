#include "FSystem_WIN32.h"
#include "FBitmap_WIN32.h"

//------------------------------------------------------------------------------
CFSystem::CFSystem(HWND hWnd, int nWidth, int nHeight)
{
  m_hWnd = hWnd;
  m_nWidth = nWidth;
  m_nHeight = nHeight;

  m_hdc = GetDC(hWnd);
  m_pDoubleBuffer = new CFBitmap();
  m_pDoubleBuffer->m_hDestDC = m_hdc;
  m_pDoubleBuffer->Create(GetWidth(), GetHeight());
  m_pDoubleBuffer->SolidFill(0);

  srand(GetTickCount());
}

//------------------------------------------------------------------------------
CFSystem::~CFSystem()
{
  if (m_pDoubleBuffer) {
    delete m_pDoubleBuffer;
  }
  if (m_hdc) {
    ::ReleaseDC(m_hWnd, m_hdc);
  }
}

//------------------------------------------------------------------------------
CFBitmap* CFSystem::GetDoubleBuffer()
{
  return m_pDoubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystem::RenderDoubleBuffer()
{
  // Draw the double buffer to the screen in default (portrait) mode.
  m_pDoubleBuffer->Blit();
}

//------------------------------------------------------------------------------
void CFSystem::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  PostMessage(m_hWnd, iEventID, iComponentID, (LPARAM)pCustomData);
}

//------------------------------------------------------------------------------
bool CFSystem::ShutDownSystem()
{
  DestroyWindow(m_hWnd);

  return true;
}

//------------------------------------------------------------------------------
void CFSystem::ForceRedraw()
{
  InvalidateRect(m_hWnd, NULL, FALSE);
}

//--------------------------------------------------------------------------------
void CFSystem::GetPathToApplication(TCHAR *pszAppPath)
{
  TCHAR *pszDiff;
  int iPos;
  GetModuleFileName(NULL, pszAppPath, MAX_PATH);
    
  pszDiff= _tcsrchr(pszAppPath, TEXT('\\'));
  if (pszDiff) {
    iPos= pszDiff - pszAppPath + 1;
    pszAppPath[iPos] = 0;
  } else {
    pszAppPath[0]= 0;
  }
}

//--------------------------------------------------------------------------------
unsigned int CFSystem::GetTicks()
{
  return ::GetTickCount();
}

//--------------------------------------------------------------------------------
int CFSystem::GetRandomNumber(int max)
{
  return rand() % max;
}
