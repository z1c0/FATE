// FWidgetMgr.h: interface for the CFWidgetMgr class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FWIDGETMGR_H__57438098_172F_414B_8BAC_D5DD32B57D94__INCLUDED_)
#define AFX_FWIDGETMGR_H__57438098_172F_414B_8BAC_D5DD32B57D94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../base/IFConnectionListener.h"
#include "../util/FLinkedList.h"
class IFRemoteWidget;

//--------------------------------------------------------------------------------
class CFWidgetMgr : public IFConnectionListener
{
public:
	CFWidgetMgr();
	virtual ~CFWidgetMgr();
  bool StartListening(int iPort);
  virtual void AddWidget(IFRemoteWidget *pWidget);
  virtual bool RemoveWidget(unsigned long ulID);
  virtual void ClearWidgets();
  virtual bool ClientConnect(CFServer *pServer);

protected:

  CFLinkedList<IFRemoteWidget*> m_WidgetList;
  int m_iPort;
};

#endif // !defined(AFX_FWIDGETMGR_H__57438098_172F_414B_8BAC_D5DD32B57D94__INCLUDED_)
