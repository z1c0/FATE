#include "FSystemImpl.h"
#include "../WIN32/FBitmapImpl.h"

//------------------------------------------------------------------------------
CFSystemImpl::CFSystemImpl(HWND hWnd, int nWidth, int nHeight,EFateDrawMode drawMode)
{
  m_hWnd = hWnd;
  if (drawMode == DM_LANDSCAPE || drawMode == DM_LANDSCAPE_FLIPPED)
  {
	  int tmp = nWidth;
	  nWidth = nHeight;
	  nHeight = tmp;
  }
  m_nWidth = nWidth;
  m_nHeight = nHeight;
  m_drawMode = drawMode;

  m_hdc = GetDC(hWnd);
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
CFBitmapImpl* CFSystemImpl::CreateDoubleBuffer()
{
  CFBitmapImpl* pDoubleBuffer = new CFBitmapImpl();
  pDoubleBuffer->m_hDestDC = m_hdc;
  pDoubleBuffer->Create(m_nWidth, m_nHeight);
  pDoubleBuffer->SolidFill(0);

  return pDoubleBuffer;
}

//--------------------------------------------------------------------------------
void CFSystemImpl::RenderDoubleBuffer(CFBitmapImpl& doubleBuffer)
{
  switch(m_drawMode)
  {
    case DM_PORTRAIT:
      // draw the double buffer to the screen in default (portrait) mode.
      doubleBuffer.Blit();
      break;

    case DM_PORTRAIT_FLIPPED:
      DrawPortraitFlipped(doubleBuffer);
      break;

    case DM_LANDSCAPE:
      DrawLandScape(doubleBuffer);
      break;

    case DM_LANDSCAPE_FLIPPED:
      DrawLandScapeFlipped(doubleBuffer);
      break;
  }
}

//--------------------------------------------------------------------------------
/// Draws the double buffer to the screen in flipped portrait mode.
void CFSystemImpl::DrawPortraitFlipped(CFBitmapImpl& doubleBuffer)
{ 
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT i = m_nWidth * m_nHeight * 3 - 1;
  UINT iColRed, iColGreen, iColBlue;
  char *pBits = doubleBuffer.GetBits();
    
  gxdp = GXGetDisplayProperties();

  // draw in landscape mode
  pusBase= (unsigned short*)GXBeginDraw();
  if ((!pusBase)||(!pBits)) return; // NOT OK TO DRAW  
  
  for (int y=m_nHeight-1; y>=0; y--)
  {
    for (int x=0; x<m_nWidth; x++)
	{
      // get address of next pixel in framebuffer
      iOffs= (x * gxdp.cbxPitch >> 1) + (y * gxdp.cbyPitch >> 1);

      iColRed  = pBits[i--];
      iColGreen= pBits[i--];
      iColBlue = pBits[i--];
    	// set color bits for 565 packed format
	    usPixelCol= (unsigned short)(((iColRed   & 0xF8) << 8)|
                                   ((iColGreen & 0xFC) << 3)| 
                                   ((iColBlue  & 0xF8) >> 3));   	  
      *(unsigned short*)(pusBase + iOffs)= usPixelCol;
    }
  }
  GXEndDraw();
}

//--------------------------------------------------------------------------------
/// Draws the doublebuffer to screen in landscape mode.
void CFSystemImpl::DrawLandScape(CFBitmapImpl& doubleBuffer)
{
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT iColRed, iColGreen, iColBlue;
  UINT i= m_nWidth * m_nHeight * 3 - 1;
  char *pBits = doubleBuffer.GetBits();
    
  gxdp= GXGetDisplayProperties();

  // draw in landscape mode
  pusBase= (unsigned short*)GXBeginDraw();
  if ((!pusBase)||(!pBits)) return; // NOT OK TO DRAW

  for (int x=m_nHeight-1; x>=0; x--) {
    for (int y=m_nWidth-1; y>=0; y--) {
      // get address of next pixel in framebuffer
      iOffs= (x * gxdp.cbxPitch >> 1) + (y * gxdp.cbyPitch >> 1);

      iColRed  = pBits[i--];
      iColGreen= pBits[i--];
      iColBlue = pBits[i--];
    	// set color bits for 565 packed format
	    usPixelCol= (unsigned short)(((iColRed   & 0xF8) << 8)|
                                   ((iColGreen & 0xFC) << 3)| 
                                   ((iColBlue  & 0xF8) >> 3));   	  
      *(unsigned short*)(pusBase + iOffs)= usPixelCol;
    }
  }
  GXEndDraw();
}

//--------------------------------------------------------------------------------
/// Draws the doublebuffer to screen in flipped landscape mode.
void CFSystemImpl::DrawLandScapeFlipped(CFBitmapImpl& doubleBuffer)
{ 
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT i= 0;
  UINT iColRed, iColGreen, iColBlue;
  char *pBits = doubleBuffer.GetBits();
  
  gxdp = GXGetDisplayProperties();

  // draw in landscape mode
  pusBase= (unsigned short*)GXBeginDraw();
  if ((!pusBase)||(!pBits)) return; // NOT OK TO DRAW 
 
  for (int x = m_nHeight-1; x >= 0; x--)
  {
    for (int y = m_nWidth-1; y >= 0; y--)
	{
      // get address of next pixel in framebuffer
      iOffs= (x * gxdp.cbxPitch >> 1) + (y * gxdp.cbyPitch >> 1);

      iColBlue = pBits[i++];
      iColGreen= pBits[i++];
      iColRed  = pBits[i++];
    	// set color bits for 565 packed format
	    usPixelCol= (unsigned short)(((iColRed   & 0xF8) << 8)|
                                   ((iColGreen & 0xFC) << 3)| 
                                   ((iColBlue  & 0xF8) >> 3));   	  
      *(unsigned short*)(pusBase + iOffs)= usPixelCol;
    }
  }
  GXEndDraw();
}

//------------------------------------------------------------------------------
void CFSystemImpl::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  PostMessage(m_hWnd, iEventID, iComponentID, (LPARAM)pCustomData);
}

//------------------------------------------------------------------------------
bool CFSystemImpl::ShutDownSystem()
{
  DestroyWindow(m_hWnd);

  return true;
}

//------------------------------------------------------------------------------
bool CFSystemImpl::EnableSuspend(bool suspend)
{
  // TODO
  return false;
}

//------------------------------------------------------------------------------
void CFSystemImpl::ShowError(const TCHAR* msg)
{
  ::MessageBox(m_hWnd, msg, TEXT("Error"), MB_OK); 
}

//------------------------------------------------------------------------------
void CFSystemImpl::ForceRedraw()
{
  InvalidateRect(m_hWnd, NULL, FALSE);
}

//------------------------------------------------------------------------------
void CFSystemImpl::AddTimer(unsigned long id, int interval)
{
  ::SetTimer(m_hWnd, id, interval, NULL);
}

//------------------------------------------------------------------------------
void CFSystemImpl::Sleep(int milliSeconds)
{
  ::Sleep(milliSeconds);
}

//--------------------------------------------------------------------------------
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
  if (pszDiff) {
    iPos= pszDiff - pszAppPath + 1;
    pszAppPath[iPos] = 0;
  } else {
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