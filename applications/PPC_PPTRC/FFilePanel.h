#pragma once

#include "../../framework/include/fate.h"

class CPPTRC;

class CFFilePanel : public CFPanel  
{
public:
	CFFilePanel();
	virtual ~CFFilePanel();
  virtual void Draw();
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  virtual bool StylusDown(int xPos, int yPos);
  bool Create();
  bool ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  bool AddItem(CFBitmap *bmpPic, const TCHAR *pszItem, const TCHAR *pszAddInfo) 
  { 
    return m_ItemList->AddPicItem(bmpPic, pszItem, pszAddInfo); 
  };
  void ClearItems() { m_ItemList->ClearItems(); };
  TCHAR* GetFilePath() { return(m_pszFullPath); };
  virtual bool ButtonReleased(DWORD dwBtnID);
  void ClearLabel() 
  { 
    SAFE_DELETE(m_pszFullPath);
    m_fileLabel->SetText(TEXT("")); 
  };
private:
  bool FillItemListFromServer(const char *pszDir);
  void UpdatePos();
  CFItemList *m_ItemList;
  CFButton *m_btnLoad;
  CFButton *m_btnRefresh;
  CFButton *m_btnDirUp;
  CFButton *m_btnHome;
  CFLabel *m_fileLabel;
  int m_iListIndent;
  int m_iMaxItems;
  TCHAR *m_pszFullPath;
  CPPTRC *m_theApp;
};

