#pragma once

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
  
  void SetDirectory(const TCHAR* pDirectory);
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

