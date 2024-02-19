#pragma once

#include "../../base/FateTypeDefs.h"

class CFBitmapImpl;

class CFSystemImpl
{
public:
  CFSystemImpl(HWND hWnd, int nWidth, int nHeight, EFateDrawMode drawMode);
  virtual ~CFSystemImpl();

  int GetWidth() const { return m_nWidth; }
  int GetHeight() const { return m_nHeight; }
  void DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
  CFBitmapImpl* CreateDoubleBuffer();  
  void RenderDoubleBuffer(CFBitmapImpl& doubleBuffer);
  void ForceRedraw();  
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
  void AddTimer(unsigned long id, int interval);
  bool ShutDownSystem();
  bool EnableSuspend(bool suspend);
  void ShowError(const TCHAR* msg);

  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);

private:  
  void DrawPortraitFlipped(CFBitmapImpl& doubleBuffer);
  void DrawLandScape(CFBitmapImpl& doubleBuffer);
  void DrawLandScapeFlipped(CFBitmapImpl& doubleBuffer);

  EFateDrawMode m_drawMode;
  HWND m_hWnd;  
  HDC m_hdc;
  int m_nWidth;
  int m_nHeight;
};


