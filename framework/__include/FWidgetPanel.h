// FWidgetPanel.h: interface for the CFWidgetPanel class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FWIDGETPANEL_H__8D9D5F70_185B_460B_9342_2D6926FF954D__INCLUDED_)
#define AFX_FWIDGETPANEL_H__8D9D5F70_185B_460B_9342_2D6926FF954D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FPanel.h"
#include "FWidgetMgr.h"

//------------------------------------------------------------------------------
class CFWidgetPanel : public CFPanel, public CFWidgetMgr  
{
public:
	CFWidgetPanel();
	virtual ~CFWidgetPanel();
  virtual bool ClientConnect(CFServer *pServer);
  virtual void AddWidget(IFRemoteWidget *pWidget);
};

#endif // !defined(AFX_FWIDGETPANEL_H__8D9D5F70_185B_460B_9342_2D6926FF954D__INCLUDED_)
