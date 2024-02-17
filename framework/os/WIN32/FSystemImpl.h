#pragma once

#include "../../base/FateTypeDefs.h"
#include "FBitmapImpl.h"

class CFSystemImpl
{
public:
  CFSystemImpl(HWND hWnd, int nWidth, int nHeight);
  ~CFSystemImpl();

  CFBitmapImpl* CreateDoubleBuffer() const;  
  void ForceRedraw();
  void DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
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
  HWND m_hWnd;  
  HDC m_hdc;
  int m_nWidth;
  int m_nHeight;
};
