#pragma once

#include "../base/FateTypeDefs.h"
#include "FItemList.h"

class CFFileList : public CFItemList
{
public:
  // Structure containing info about file-filters set.
  struct FILEFILTER
  {
    TCHAR* pszFilter;
    struct FILEFILTER* pNext;
  };

	CFFileList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow);
	virtual ~CFFileList();

  void Refresh() { SetCurrDir(m_szCurrDir);};	
  bool SetCurrDir(LPCTSTR pszDir);
  bool SetFilters(LPTSTR pFilterStr);
  void RemoveFilters();

private:
  virtual void BeforeDrawItems();
  virtual void ItemSelected(ITEMLISTENTRY* pEntry);

  bool ReadDir();
  TCHAR m_szCurrDir[MAX_PATH];
  bool m_bDirRead;
  FILEFILTER* m_pFilters;
  FILEFILTER* m_pLastFilter;
};
