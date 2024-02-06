// FFileList.cpp: implementation of the CFFileList class.
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <commctrl.h>
#include "FFileList.h"
#include "../base/FateTypeDefs.h"
#include "../base/FateApp.h"
#include "../base/FBitmap.h"


//--------------------------------------------------------------------------------
CFFileList::CFFileList(int iMaxVisItems, int iItemWidth, CFBitmap *cbUpArrow, CFBitmap *cbDownArrow) :
            CFItemList(iMaxVisItems, iItemWidth, cbUpArrow, cbDownArrow)
{
  _tcscpy(m_szCurrDir, TEXT("\\"));
  m_bDirRead= FALSE;
  m_pFilters= NULL;
  m_pLast= NULL;
}

//--------------------------------------------------------------------------------
CFFileList::~CFFileList()
{
  // filter cleanup?
  if (m_pFilters) RemoveFilters();
}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
void CFFileList::DrawItems()
{
  // have directory contents been read?
  if (!m_bDirRead) {
    ClearItems();
    ReadDir();
    m_bDirRead= TRUE;
  }

  // rectangle in which an item is displayed
  RECT itemRect;
  //itemRect.left  = m_iPosX + 1;
  //itemRect.top   = m_iPosY + 1;
  //itemRect.right = m_iPosX + m_iItemWidth;
  //itemRect.bottom= m_iPosY + m_iItemHeight - 1;
  int posX = 1;
  int posY = 1;
  itemRect.left  = posX + 1;
  itemRect.top   = posY + 1;
  itemRect.right = posX + m_iItemWidth;
  itemRect.bottom= posY + m_iItemHeight - 1;

  // display list entries
  ITEMLISTENTRY *pAuxEntry= m_pEntries;
  int pos= 0, index= 0;  
  
  while (pAuxEntry) {
    if ((pos >= m_iItemPos)&&(pos < (m_iItemPos + m_iMaxVisItems))) {
      // item selected?
      if ((m_iSelItem != -1)&&(m_iSelItem - m_iItemPos == index)) {
       
        m_bmpBack->SetColor(m_colHiBack);
        m_bmpBack->SetBackgroundColor(m_colHiBack);
        m_bmpBack->SetTextColor(m_colHiText);
        
        itemRect.right+= 2 * m_iHorMargin - 1;
        m_bmpBack->DrawFilledRect(itemRect);
        itemRect.right-= 2 * m_iHorMargin - 1;
        // redraw the items        
        ExtTextOut(m_bmpBack->GetSourceDC(), posX + m_iHorMargin + 18, posY + m_iItemHeight * index + 1,
                   ETO_CLIPPED, &itemRect, pAuxEntry->pszItem, _tcslen(pAuxEntry->pszItem), NULL);
        SHFILEINFO shfi;
        HANDLE h= (HANDLE)SHGetFileInfo(pAuxEntry->pszAddInfo, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
        ImageList_Draw((HIMAGELIST)h, shfi.iIcon, m_bmpBack->GetSourceDC(), posX + m_iHorMargin, posY + m_iItemHeight * index + 1, ILD_BLEND50);
      }
      else
      {
        m_bmpBack->SetColor(m_colBack);
        m_bmpBack->SetBackgroundColor(m_colBack);
        m_bmpBack->SetTextColor(m_colText);
      
        // clear old item
        itemRect.right+= 2 * m_iHorMargin - 1;
        m_bmpBack->DrawFilledRect(itemRect);
        itemRect.right-= 2 * m_iHorMargin - 1;
        // redraw the items
        ExtTextOut(m_bmpBack->GetSourceDC(), posX + m_iHorMargin + 18, posY + m_iItemHeight * index + 1,
                   ETO_CLIPPED, &itemRect, pAuxEntry->pszItem, _tcslen(pAuxEntry->pszItem), NULL);
        SHFILEINFO shfi;
        HANDLE h= (HANDLE)SHGetFileInfo(pAuxEntry->pszAddInfo, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
        ImageList_Draw((HIMAGELIST)h, shfi.iIcon, m_bmpBack->GetSourceDC(), posX + m_iHorMargin, posY + m_iItemHeight * index + 1, ILD_NORMAL);
      }
      index++;
      itemRect.top+= m_iItemHeight;
      itemRect.bottom+= m_iItemHeight;
    }
    pos++;
    pAuxEntry= pAuxEntry->pNext;
  }
}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
BOOL CFFileList::SetCurrDir(LPCTSTR pszDir)
{
  TCHAR pCurrDir[MAX_PATH];

  _tcscpy(pCurrDir, pszDir);

  // remove finishing '\' if necessary
  int iLen= _tcslen(pCurrDir);
  if ((iLen)&&(pCurrDir[iLen-1] == '\\')) pCurrDir[iLen-1]= 0;

  if (IsDirectory(pCurrDir)) {
    // directory up?
    if (!_tcscmp(pCurrDir, TEXT(".."))) {
      int i= _tcslen(m_szCurrDir) - 2;

      while ((i > 0)&&(m_szCurrDir[i] != '\\')) i--;
      m_szCurrDir[i+1]= 0;

    } else {
      _tcscpy(m_szCurrDir, pCurrDir);
      _tcscat(m_szCurrDir, TEXT("\\"));
    }

    m_bDirRead= FALSE;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFFileList::ReadDir()
{
  WIN32_FIND_DATA ffd;
  HANDLE hFile;
  TCHAR szFilePath[256];
  TCHAR szFullPath[256];
  
  // first entry is always "directory up" except for root-directory
  if (_tcscmp(m_szCurrDir, TEXT("\\"))) AddItem(TEXT(".."), TEXT(".."));
  
  // create wild card string for directory exploring
  _tcscpy(szFilePath, m_szCurrDir);
  _tcscat(szFilePath, TEXT("*.*"));

  hFile= FindFirstFile(szFilePath, &ffd);
  if (hFile == INVALID_HANDLE_VALUE) return(FALSE);
  
  do {
    // create full qualified path of file
    _tcscpy(szFullPath, m_szCurrDir);
    _tcscat(szFullPath, ffd.cFileName);
    
    // exclude current and parent directory
    if (ffd.cFileName[0] != '.') {
      // any filters specified?
      if ((!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))&&(m_pFilters)) {
        int iStrLen;
        FILEFILTER *pTemp= m_pFilters;

        while (pTemp) {
          iStrLen= _tcslen(ffd.cFileName);
          if ((iStrLen >= 4)&&(!_tcscmp(&ffd.cFileName[iStrLen - 4], pTemp->pszFilter)))        
            AddItem(ffd.cFileName, szFullPath);
          pTemp= pTemp->pNext;
        }
      } else {
        AddItem(ffd.cFileName, szFullPath);
      }
    }
  } while(FindNextFile(hFile, &ffd));
  FindClose(hFile);

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFFileList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  // move to selected directory
  if (SetCurrDir(pEntry->pszAddInfo)) {
    pEntry->ulUser= IL_DIRFLAG;  // mark it as directory
    m_pSystem->QueueEvent(WM_ITEMLISTSELECT, m_ulID, (void*)pEntry);
    Draw();
  
  } else {
    CFItemList::ItemSelected(pEntry);
  }
}

//--------------------------------------------------------------------------------
/// Checks whether the specified directory name is really one.
BOOL CFFileList::IsDirectory(LPCTSTR pDir)
{
  DWORD dwAttr= GetFileAttributes(pDir);
  return((dwAttr != (DWORD)-1)&&(dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}

//--------------------------------------------------------------------------------
/// A set of file-types for filtering can be specified to the filelist.
/// Parameter "pFilterStr" must have the format ".ext1 .ext2 .ext3 .ext4"
/// Return value indicates wether filter string was specified correctly.
BOOL CFFileList::SetFilters(LPTSTR pFilterStr)
{
  if (_tcslen(pFilterStr)) {
    TCHAR *pszCopy;
    LPCTSTR pToken;
    LPCTSTR pDel= TEXT(" ");

    // copy the filter string to get write permission on it
    pszCopy= (TCHAR*)malloc((_tcslen(pFilterStr) + 1) * sizeof(TCHAR));
    _tcscpy(pszCopy, pFilterStr);
   
    pToken= _tcstok(pszCopy, pDel);
    // while there are tokens, add file filters
    while(pToken) {
      // filter valid?
      if ((_tcslen(pToken) >= 2)&&(pToken[0] == '.')) {
        // add filter
        FILEFILTER *pTemp= new FILEFILTER;
        if (!pTemp) return(FALSE);
        pTemp->pszFilter= (TCHAR*)malloc((_tcslen(pToken) + 1) * sizeof(TCHAR));
        _tcscpy(pTemp->pszFilter, pToken);
        pTemp->pNext= NULL;
        // enqueue
        if (!m_pFilters) {
          m_pLastFilter= m_pFilters= pTemp;
        } else {
          m_pLastFilter->pNext= pTemp;
          m_pLastFilter= m_pLastFilter->pNext;
        }
      }
      // get next token
      pToken = _tcstok(NULL, pDel);
    }
    free(pszCopy);
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Removes all filters that have been specified
void CFFileList::RemoveFilters()
{
  // clear file-filter datatstructure
  FILEFILTER *pTemp, *pOld;
  pTemp= m_pFilters;
  while(pTemp) {
    pOld= pTemp;
    pTemp= pTemp->pNext;
    delete(pOld->pszFilter);
    delete(pOld);
  }
}


