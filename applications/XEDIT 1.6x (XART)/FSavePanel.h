// FSavePanel.h: interface for the CFSavePanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FSAVEPANEL_H__B7FA1982_FB24_4B93_889D_97724D6B6797__INCLUDED_)
#define AFX_FSAVEPANEL_H__B7FA1982_FB24_4B93_889D_97724D6B6797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFSavePanel : public CFPanel  
{
public:
	CFSavePanel(LPTSTR pDirectory);
	virtual ~CFSavePanel();
  
  void Refresh();
	
  virtual bool ButtonPressed(DWORD dwBtnID);
	virtual bool ButtonReleased(DWORD dwBtnID);
	virtual bool ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  
  BOOL Create();
  
  virtual void SetX(int iPosX); 
  virtual void SetY(int iPosY); 
  
  void SetDirectory(LPTSTR pDirectory);
  LPCTSTR GetDirectory() { return(m_szDirectory); };

  void SetFileToSave(LPCTSTR pFileToSave);
  LPCTSTR GetFileToSave() { return(m_szFileToSaveFullPath); };

private:
  void UpdatePos();

  CFLabel *m_fileToSaveLabel;
  
  CFDirList *m_dirList;
  CFButton *m_btnOK;
  CFButton *m_btnCancel;
  
  TCHAR m_szDirectory[256];
  
  BOOL m_bCreated;

  TCHAR m_szFileToSaveFullPath[256];

  TCHAR m_szFileName[256];

};

#endif // !defined(AFX_FSAVEPANEL_H__B7FA1982_FB24_4B93_889D_97724D6B6797__INCLUDED_)
