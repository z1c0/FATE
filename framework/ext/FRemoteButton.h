// FRemoteButton.h: interface for the CFRemoteButton class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FREMOTEBUTTON_H__6E3B612C_ABD5_4796_BCB3_E5B5D7DFF324__INCLUDED_)
#define AFX_FREMOTEBUTTON_H__6E3B612C_ABD5_4796_BCB3_E5B5D7DFF324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFRemoteWidget.h"
#include "../gui/FButton.h"

//------------------------------------------------------------------------------
class CFRemoteButton : public CFButton, public IFRemoteWidget
{
public:
	CFRemoteButton(LPCTSTR pCaption);
	virtual ~CFRemoteButton();
  virtual bool Action();
};

#endif // !defined(AFX_FREMOTEBUTTON_H__6E3B612C_ABD5_4796_BCB3_E5B5D7DFF324__INCLUDED_)
