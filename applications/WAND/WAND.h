// WAND.h: interface for the CWAND class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAND_H__ABFAAE45_696E_4005_A506_1116A01DAFA7__INCLUDED_)
#define AFX_WAND_H__ABFAAE45_696E_4005_A506_1116A01DAFA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
class CWAND : public CFateApp  
{
public:
	CWAND();
	virtual ~CWAND();
  virtual bool InitFateApp();
  virtual bool ButtonReleased(DWORD dwBtnID);
private:
  CFButton *m_btnQuit;
  CFController *m_pCont;
};

#endif // !defined(AFX_WAND_H__ABFAAE45_696E_4005_A506_1116A01DAFA7__INCLUDED_)
