// FFilePanel.h: interface for the CFFilePanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FFILEPANEL_H__354FB8AB_E98F_4F9D_8B4B_EB71766877C7__INCLUDED_)
#define AFX_FFILEPANEL_H__354FB8AB_E98F_4F9D_8B4B_EB71766877C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
  virtual BOOL StylusDown(int xPos, int yPos);
  BOOL Create();
  BOOL ItemListSelected(DWORD dwListID, LPITEMLISTENTRY pEntry);
  BOOL AddItem(CFBitmap *bmpPic, TCHAR *pszItem, TCHAR *pszAddInfo) 
  { 
    return(m_ItemList->AddPicItem(bmpPic, pszItem, pszAddInfo)); 
  };
  void ClearItems() { m_ItemList->ClearItems(); };
  TCHAR* GetFilePath() { return(m_pszFullPath); };
  virtual BOOL ButtonReleased(DWORD dwBtnID);
  void ClearLabel() 
  { 
    SAFE_RELEASE(m_pszFullPath);
    m_fileLabel->SetText(TEXT("")); 
  };
private:
  BOOL FillItemListFromServer(char *pszDir);
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

#endif // !defined(AFX_FFILEPANEL_H__354FB8AB_E98F_4F9D_8B4B_EB71766877C7__INCLUDED_)
