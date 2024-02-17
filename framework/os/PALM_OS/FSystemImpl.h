#pragma once

#include "../../base/FateTypeDefs.h"

class CFBitmapImpl;

#define PALMOS_REPAINT  (firstUserEvent + 1)

struct FateEventType
{
  eventsEnum eType;
  Boolean penDown;
  UInt8 tapCount;
  Int16 screenX;
  Int16 screenY;
  union 
  {
    struct _GenericEventType generic; // sets size of union
    struct 
    {
      // Must not exceed 16 bytes in length!
      unsigned long ulID;
    } FateEventData;
  } data;
};

class CFSystemImpl
{
public:
  CFSystemImpl(int iWidth, int iHeight);

  void RenderDoubleBuffer(CFBitmapImpl& doubleBuffer);
  void ForceRedraw();
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
  bool ShutDownSystem();
  int GetWidth() const { return m_iWidth; }
  int GetHeight() const { return m_iHeight; }
  CFBitmapImpl* CreateDoubleBuffer();
  void ShowError(const TCHAR* msg);
  void DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
  void AddTimer(unsigned long id, int interval);
  
  static void GetPathToApplication(TCHAR *pszAppPath);
  static unsigned int GetTicks();
  static int GetRandomNumber(int max);
  
private:
  int m_iWidth;
  int m_iHeight;
};

