#include "FSystem.h"
#include "FBitmap.h"

//------------------------------------------------------------------------------
CFSystem::CFSystem(int iWidth, int iHeight)
{
  m_iWidth = iWidth;
  m_iHeight = iHeight;  

  m_pDoubleBuffer = new CFBitmap();
  m_pDoubleBuffer->Create(m_iWidth, m_iHeight);
  m_pDoubleBuffer->SolidFill(0);  // BLACK
}

//------------------------------------------------------------------------------
CFSystem::~CFSystem()
{
  delete m_pDoubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystem::RenderDoubleBuffer()
{
  m_pDoubleBuffer->Blit();
}

//------------------------------------------------------------------------------
void CFSystem::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  FateEventType event;
  
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)iEventID;
  event.data.FateEventData.ulID= iComponentID;
  
  EvtAddEventToQueue((EventPtr)&event);
}

//------------------------------------------------------------------------------
bool CFSystem::ShutDownSystem()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= appStopEvent;
  
  EvtAddEventToQueue(&event);

  return(true);
}

//------------------------------------------------------------------------------
void CFSystem::ForceRedraw()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)PALMOS_REPAINT;
  
  EvtAddEventToQueue(&event);
}

//--------------------------------------------------------------------------------
void CFSystem::GetPathToApplication(TCHAR *pszAppPath)
{
  // TODO
}