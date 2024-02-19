#pragma once

#include "FItemList.h"
#include "../base/FateTypeDefs.h"

class CFDirList : public CFItemList  
{
public:
	CFDirList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow);
  void Refresh() { SetCurrDir(m_szCurrDir); }
  bool SetCurrDir(const TCHAR* pCurrDir);
  const TCHAR* GetCurrDir() { return m_szCurrDir; }

private:
  virtual void ItemSelected(ITEMLISTENTRY *pEntry);
  virtual void BeforeDrawItems();

  bool ReadDir();  
  TCHAR m_szCurrDir[MAX_PATH];  
  bool m_bDirRead;
};

