#ifndef __FSYSTEM_WIN32_WCE_H__
#define __FSYSTEM_WIN32_WCE_H__

#include "FateTypeDefs.h"

class CFBitmap;

//------------------------------------------------------------------------------
class CFSystem
{

public:

  CFSystem(HWND hWnd, int nWidth, int nHeight);
  virtual ~CFSystem();

  CFBitmap* GetDoubleBuffer();
  
  void RenderDoubleBuffer();

  void ForceRedraw();
  
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);

  bool ShutDownSystem();

  int GetWidth() { return m_nWidth; };

  int GetHeight() { return m_nHeight; };

  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);

protected:
  
  void DrawPortraitFlipped();
  void DrawLandScape();
  void DrawLandScapeFlipped();

  EFateDrawMode m_DrawMode;
  CFBitmap *m_pDoubleBuffer;
  HWND m_hWnd;  
  HDC m_hdc;
  int m_nWidth;
  int m_nHeight;
};

#endif  // __FSYSTEM_WIN32_WCE_H__


