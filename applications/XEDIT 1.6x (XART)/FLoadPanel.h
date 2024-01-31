// FLoadPanel.h: interface for the CFLoadPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOADPANEL_H__A6BBD991_F4A6_4839_8450_1151597B7669__INCLUDED_)
#define AFX_FLOADPANEL_H__A6BBD991_F4A6_4839_8450_1151597B7669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FATE/stdafx.h"

//--------------------------------------------------------------------------------
class CFLoadPanel : public CFPanel  
{
public:
	CFLoadPanel(LPCTSTR pFilePath);
	virtual ~CFLoadPanel();
  void Refresh();
  virtual BOOL ButtonPressed(DWORD dwBtnID);
	virtual BOOL ButtonReleased(DWORD dwBtnID);
	virtual BOOL ItemListSelected(DWORD dwListID, LPITEMLISTENTRY pEntry);
  BOOL Create();
  void SetFilePath(LPCTSTR pFilePath);
  virtual void SetX(int iPosX); 
  virtual void SetY(int iPosY); 
  void SetFileToLoad(LPCTSTR pFileToLoad) { m_pFileToLoad= pFileToLoad; };
  LPCTSTR GetFileToLoad() { return(m_pFileToLoad); };  
private:
  void UpdatePos();  
  CFFileList *m_waveList;  
  TCHAR m_szFilePath[256];  
  CFButton *m_btnOK;
  CFButton *m_btnCancel;  
  LPCTSTR m_pFileToLoad;  
  BOOL m_bCreated;
};

#endif // !defined(AFX_FLOADPANEL_H__A6BBD991_F4A6_4839_8450_1151597B7669__INCLUDED_)
