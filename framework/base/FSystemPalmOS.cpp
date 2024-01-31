#include "FSystemPalmOS.h"


//------------------------------------------------------------------------------
CFSystemPalmOS::CFSystemPalmOS(int iWidth, int iHeight)
{
  m_iWidth = iWidth;
  m_iHeight= iHeight;  

  m_pDoubleBuffer= new CFBitmap();
  m_pDoubleBuffer->Create(m_iWidth, m_iHeight);
  m_pDoubleBuffer->SolidFill(0);  // BLACK
}

//------------------------------------------------------------------------------
CFSystemPalmOS::~CFSystemPalmOS()
{
}

//------------------------------------------------------------------------------
void CFSystemPalmOS::RenderDoubleBuffer()
{
  m_pDoubleBuffer->Blit();
}

//------------------------------------------------------------------------------
void CFSystemPalmOS::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  FateEventType event;
  
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)iEventID;
  event.data.FateEventData.ulID= iComponentID;
  
  EvtAddEventToQueue((EventPtr)&event);
}

//------------------------------------------------------------------------------
bool CFSystemPalmOS::ShutDownSystem()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= appStopEvent;
  
  EvtAddEventToQueue(&event);

  return(true);
}

//------------------------------------------------------------------------------
void CFSystemPalmOS::ForceRedraw()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)PALMOS_REPAINT;
  
  EvtAddEventToQueue(&event);
}
