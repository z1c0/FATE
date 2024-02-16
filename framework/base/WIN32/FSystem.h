#pragma once

#include "../FateTypeDefs.h"

class CFBitmap;

class CFSystem
{
public:

  CFSystem(HWND hWnd, int nWidth, int nHeight);
  virtual ~CFSystem();

  CFBitmap* GetDoubleBuffer();
  
  void RenderDoubleBuffer();

  void ForceRedraw();
  void DrawFileIcon(CFBitmap& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
  void ShowError(const TCHAR* msg);
  
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);

  bool ShutDownSystem();

  int GetWidth() const { return m_nWidth; };
  int GetHeight() const { return m_nHeight; };
  void AddTimer(unsigned long id, int interval);  
  bool EnableSuspend(bool suspend) { return false; }  

  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);

private:

  CFBitmap *m_pDoubleBuffer;
  HWND m_hWnd;  
  HDC m_hdc;
  int m_nWidth;
  int m_nHeight;
};
