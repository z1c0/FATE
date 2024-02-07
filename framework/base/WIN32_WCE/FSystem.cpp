#include "FSystem_WIN32_WCE.h"
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

//--------------------------------------------------------------------------------
void CFSystem::RenderDoubleBuffer()
{
  switch(m_DrawMode) {
    case DM_PORTRAIT:
      // draw the double buffer to the screen in default (portrait) mode.
      m_pDoubleBuffer->Blit();
      break;

    case DM_PORTRAIT_FLIPPED:
      DrawPortraitFlipped();
      break;

    case DM_LANDSCAPE:
      DrawLandScape();
      break;

    case DM_LANDSCAPE_FLIPPED:
      DrawLandScapeFlipped();
      break;
  }
}

//--------------------------------------------------------------------------------
/// Draws the double buffer to the screen in flipped portrait mode.
void CFSystem::DrawPortraitFlipped()
{ 
  /*
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT i = m_nWidth * m_nHeight * 3 - 1;
  UINT iColRed, iColGreen, iColBlue;
  char *pBits = m_pDoubleBuffer->GetBits();
    
  gxdp = GXGetDisplayProperties();

  // draw in landscape mode
  pusBase= (unsigned short*)GXBeginDraw();
  if ((!pusBase)||(!pBits)) return; // NOT OK TO DRAW  
  
  for (int y=m_nHeight-1; y>=0; y--) {
    for (int x=0; x<m_nWidth; x++) {
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
  */
}

//--------------------------------------------------------------------------------
/// Draws the doublebuffer to screen in landscape mode.
void CFSystem::DrawLandScape()
{
  /*
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT iColRed, iColGreen, iColBlue;
  UINT i= m_nWidth * m_nHeight * 3 - 1;
  char *pBits= m_pDoubleBuffer->GetBits();
    
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
  */
}

//--------------------------------------------------------------------------------
/// Draws the doublebuffer to screen in flipped landscape mode.
void CFSystem::DrawLandScapeFlipped()
{ 
  /*
  GXDisplayProperties gxdp;
  unsigned short *pusBase;
  int iOffs;
  unsigned short usPixelCol;
  UINT i= 0;
  UINT iColRed, iColGreen, iColBlue;
  char *pBits= m_pDoubleBuffer->GetBits();
  
  gxdp= GXGetDisplayProperties();

  // draw in landscape mode
  pusBase= (unsigned short*)GXBeginDraw();
  if ((!pusBase)||(!pBits)) return; // NOT OK TO DRAW 
 
  for (int x=m_nHeight-1; x>=0; x--) {
    for (int y=m_nWidth-1; y>=0; y--) {
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
  */
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