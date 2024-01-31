// FDirPanel.h: interface for the CFDirPanel class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDIRPANEL_H__0BABF920_AA1D_40F8_A67B_A077CB55DF23__INCLUDED_)
#define AFX_FDIRPANEL_H__0BABF920_AA1D_40F8_A67B_A077CB55DF23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FATE/stdafx.h"

class CFDirPanel : public CFPanel  
{
public:
	CFDirPanel(LPTSTR pDirectory);
	virtual ~CFDirPanel();
  
  void Refresh();
	
  virtual BOOL ButtonPressed(DWORD dwBtnID);
	virtual BOOL ButtonReleased(DWORD dwBtnID);
	virtual BOOL ItemListSelected(DWORD dwListID, LPITEMLISTENTRY pEntry);
  
  BOOL Create();
  
  virtual void SetX(int iPosX); 
  virtual void SetY(int iPosY); 
  
  void SetDirectory(LPTSTR pDirectory) { m_pDirectory = pDirectory;};
  LPCTSTR GetDirectory() { return(m_pDirectory); };

private:
  void UpdatePos();
  
  CFDirList *m_dirList;
  CFButton *m_btnOK;
  CFButton *m_btnCancel;
  
  LPTSTR m_pDirectory;
  
  BOOL m_bCreated;
};

#endif // !defined(AFX_FDIRPANEL_H__0BABF920_AA1D_40F8_A67B_A077CB55DF23__INCLUDED_)
