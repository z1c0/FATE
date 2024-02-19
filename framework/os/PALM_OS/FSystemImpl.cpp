#include "FSystemImpl.h"
#include "FBitmapImpl.h"

//------------------------------------------------------------------------------
CFSystemImpl::CFSystemImpl(int iWidth, int iHeight)
{
  m_iWidth = iWidth;
  m_iHeight = iHeight;  
}

//------------------------------------------------------------------------------
CFBitmapImpl* CFSystemImpl::CreateDoubleBuffer()
{
  CFBitmapImpl* pDoubleBuffer = new CFBitmapImpl();
  pDoubleBuffer->Create(m_iWidth, m_iHeight);
  pDoubleBuffer->SolidFill(0);  // BLACK
  return pDoubleBuffer;
}

//------------------------------------------------------------------------------
void CFSystemImpl::RenderDoubleBuffer(CFBitmapImpl& doubleBuffer)
{
  doubleBuffer.Blit();
}

//------------------------------------------------------------------------------
void CFSystemImpl::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
  FateEventType event;
  
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)iEventID;
  event.data.FateEventData.ulID= iComponentID;
  
  EvtAddEventToQueue((EventPtr)&event);
}

//------------------------------------------------------------------------------
bool CFSystemImpl::ShutDownSystem()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= appStopEvent;
  
  EvtAddEventToQueue(&event);

  return(true);
}

//------------------------------------------------------------------------------
void CFSystemImpl::ForceRedraw()
{
  EventType event;
  MemSet(&event, sizeof(EventType), 0);
  event.eType= (eventsEnum)PALMOS_REPAINT;
  
  EvtAddEventToQueue(&event);
}

//--------------------------------------------------------------------------------
void CFSystemImpl::GetPathToApplication(TCHAR *pszAppPath)
{
  // TODO
}