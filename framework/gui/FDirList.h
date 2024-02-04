// FDirList.h: interface for the CFDirList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDIRLIST_H__9D16FD64_BBC9_4FE9_8CC1_69291E3DD663__INCLUDED_)
#define AFX_FDIRLIST_H__9D16FD64_BBC9_4FE9_8CC1_69291E3DD663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FItemList.h"
#include "../base/FateTypeDefs.h"

//------------------------------------------------------------------------------
class CFDirList : public CFItemList  
{
public:
	CFDirList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow);
  virtual ~CFDirList();  
  virtual void SetSystem(CFSystem *pSystem);
  void Refresh() { SetCurrDir(m_szCurrDir);};	
  BOOL SetCurrDir(LPTSTR pCurrDir);
  LPTSTR GetCurrDir() { return (LPTSTR)&m_szCurrDir; };  

private:

  virtual void ItemSelected(ITEMLISTENTRY *pEntry);

  BOOL IsDirectory(LPCTSTR pDir);  
  virtual void DrawItems();  
  BOOL ReadDir();  
  BOOL AddItem(TCHAR *pszItem, TCHAR *pszAddInfo= NULL);  
  TCHAR m_szCurrDir[MAX_PATH];  
  BOOL m_bDirRead;
};

#endif // !defined(AFX_FDIRLIST_H__9D16FD64_BBC9_4FE9_8CC1_69291E3DD663__INCLUDED_)
