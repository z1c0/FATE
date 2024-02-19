#pragma once

#include "FBitmap.h"

class CFSystemImpl;

class CFSystem
{
public:
  CFSystem(CFSystemImpl* pImpl);
  ~CFSystem();
  
  CFBitmap& GetDoubleBuffer();
  void RenderDoubleBuffer();
  void ForceRedraw();
  void DrawFileIcon(CFBitmap& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
  void ShowError(const TCHAR* msg);  
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
  bool ShutDownSystem();
  int GetWidth() const;
  int GetHeight() const;
  void AddTimer(unsigned long id, int interval);  
  bool EnableSuspend(bool suspend);

  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);

private:
  CFSystemImpl* m_pImpl;
  CFBitmap* m_pDoubleBuffer;
};


