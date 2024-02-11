#pragma once

#include "../FateTypeDefs.h"

class CFBitmap;

//------------------------------------------------------------------------------
class CFSystem
{

public:

  CFSystem(HWND hWnd, int nWidth, int nHeight, EFateDrawMode drawMode);
  virtual ~CFSystem();

  int GetWidth() const { return m_nWidth; }
  int GetHeight() const { return m_nHeight; }
  void DrawFileIcon(CFBitmap& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
  CFBitmap* GetDoubleBuffer();  
  void RenderDoubleBuffer();
  void ForceRedraw();  
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
  void AddTimer(unsigned long id, int interval);
  bool ShutDownSystem();
  bool EnableSuspend(bool suspend);

  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);

private:  
  void DrawPortraitFlipped();
  void DrawLandScape();
  void DrawLandScapeFlipped();

  EFateDrawMode m_drawMode;
  CFBitmap *m_pDoubleBuffer;
  HWND m_hWnd;  
  HDC m_hdc;
  int m_nWidth;
  int m_nHeight;
};



