#ifndef __FFILELIST__H__
#define __FFILELIST__H__

#include "../base/FateTypeDefs.h"
#include "FItemList.h"


//--------------------------------------------------------------------------------
#define IL_DIRFLAG    0x03


//------------------------------------------------------------------------------
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
  BOOL SetCurrDir(LPCTSTR pszDir);
  BOOL SetFilters(LPTSTR pFilterStr);
  void RemoveFilters();

private:

  virtual void ItemSelected(ITEMLISTENTRY *pEntry);
  BOOL IsDirectory(LPCTSTR pDir);
  virtual void DrawItems();
  BOOL ReadDir();
  TCHAR m_szCurrDir[MAX_PATH];
  BOOL m_bDirRead;
  FILEFILTER *m_pFilters;
  FILEFILTER *m_pLastFilter;
};

#endif  // __FFILELIST__H__