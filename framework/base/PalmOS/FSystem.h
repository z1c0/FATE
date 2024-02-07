#ifndef __FSYSTEMCE_H__
#define __FSYSTEMCE_H__

#include "FSystem.h"
#include "FateApp.h"

//------------------------------------------------------------------------------
#define PALMOS_REPAINT  (firstUserEvent + 1)

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
class CFSystemPalmOS : public CFSystem
{
public:
  CFSystemPalmOS(int iWidth, int iHeight) FATE_SECTION;
  virtual ~CFSystemPalmOS() FATE_SECTION;

  virtual void RenderDoubleBuffer() FATE_SECTION;

  virtual void ForceRedraw() FATE_SECTION;

  virtual void QueueEvent(int iEventID, int iComponentID, void *pCustomData) FATE_SECTION;

  virtual bool ShutDownSystem() FATE_SECTION;
  
protected:

};

#endif  // __FSYSTEMCE_H__
