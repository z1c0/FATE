// FWidgetPanel.cpp: implementation of the CFWidgetPanel class.
//////////////////////////////////////////////////////////////////////
#include "FWidgetPanel.h"
#include "FRemoteButton.h"

//--------------------------------------------------------------------------------
CFWidgetPanel::CFWidgetPanel() : CFPanel(), CFWidgetMgr()
{
}

//--------------------------------------------------------------------------------
CFWidgetPanel::~CFWidgetPanel()
{
}

//--------------------------------------------------------------------------------
/// Receive necessary information.
bool CFWidgetPanel::ClientConnect(CFServer *pServer)
{
  return(CFWidgetMgr::ClientConnect(pServer));
}

//--------------------------------------------------------------------------------
void CFWidgetPanel::AddWidget(IFRemoteWidget *pWidget)
{
  CFWidgetMgr::AddWidget(pWidget);
  
  Add((CFRemoteButton*)pWidget);
  if (m_bVisible) DrawNotify();
}