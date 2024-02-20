#pragma once

class CFDirPanel : public CFPanel  
{
public:
	CFDirPanel(LPTSTR pDirectory);
	virtual ~CFDirPanel();
  
  void Refresh();
	
  virtual bool ButtonPressed(DWORD dwBtnID);
	virtual bool ButtonReleased(DWORD dwBtnID);
	virtual bool ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  
  BOOL Create();
  
  virtual void SetX(int iPosX); 
  virtual void SetY(int iPosY); 
  
  void SetDirectory(const TCHAR* pDirectory) { m_pDirectory = pDirectory;};
  LPCTSTR GetDirectory() { return(m_pDirectory); };

private:
  void UpdatePos();
  
  CFDirList *m_dirList;
  CFButton *m_btnOK;
  CFButton *m_btnCancel;
  
  const TCHAR* m_pDirectory;
  
  BOOL m_bCreated;
};

