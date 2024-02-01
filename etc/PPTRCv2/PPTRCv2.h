// PPTRCv2.h : main header file for the PPTRCV2 application
//

#if !defined(AFX_PPTRCV2_H__E5947A60_E5DC_481A_B02A_4551DD539C24__INCLUDED_)
#define AFX_PPTRCV2_H__E5947A60_E5DC_481A_B02A_4551DD539C24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPPTRCv2App:
// See PPTRCv2.cpp for the implementation of this class
//

class CPPTRCv2App : public CWinApp
{
public:
	CPPTRCv2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPTRCv2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPPTRCv2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTRCV2_H__E5947A60_E5DC_481A_B02A_4551DD539C24__INCLUDED_)
