#pragma once

#include "../FateTypeDefs.h"

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

class CFSystem
{
public:
  CFSystem(int iWidth, int iHeight);
  virtual ~CFSystem();

  void RenderDoubleBuffer();
  void ForceRedraw();
  void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
  bool ShutDownSystem();
  int GetWidth() const { return m_iWidth; }
  int GetHeight() const { return m_iHeight; }
  CFBitmap* GetDoubleBuffer() { return m_pDoubleBuffer; }
  
  static void GetPathToApplication(TCHAR *pszAppPath);
  
private:
  CFBitmap* m_pDoubleBuffer;
  int m_iWidth;
  int m_iHeight;
};

