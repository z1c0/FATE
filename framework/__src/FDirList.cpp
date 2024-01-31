// FDirList.cpp: implementation of the CFDirList class.
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "FDirList.h"
#include "FBitmap.h"
#include "FateApp.h"


//--------------------------------------------------------------------------------
CFDirList::CFDirList(int iMaxVisItems, int iItemWidth, CFBitmap *cbUpArrow, CFBitmap *cbDownArrow) :
            CFItemList(iMaxVisItems, iItemWidth, cbUpArrow, cbDownArrow)
{
  _tcscpy(m_szCurrDir, TEXT("\\"));
  m_bDirRead= FALSE;
  m_pLast= NULL;
}

//--------------------------------------------------------------------------------
CFDirList::~CFDirList()
{
}

//--------------------------------------------------------------------------------
/// Method is inherited from IFateComponent and overwritten because some additional
/// initialization using the HDC is necessary.
void CFDirList::SetDC(HDC hdc)
{
  m_hdc= hdc;
  // depending on height, calculate number of possible items
  TEXTMETRIC tm;
  GetTextMetrics(m_hdc, &tm);
  m_iItemHeight= max(18, tm.tmHeight + tm.tmExternalLeading + m_iVerMargin);
  
  // create background bitmap
  // TODO
  // m_bmpBack->SetDestDC(m_hdc);
  m_bmpBack->Create(m_iItemWidth + 2 * m_iHorMargin + m_bmpUpArrow->GetWidth(), 
                   m_iItemHeight * m_iMaxVisItems);
  CreateBackGround();

  // calculate remaining space of scrollbar
  m_iScrollBarHeight= m_bmpBack->GetHeight() - m_bmpUpArrow->GetHeight() - m_bmpDownArrow->GetHeight();    
  
  // calculate step width for scrollbar-slider
  if (m_iItemCount > 0) {
    m_iSliderStep= (m_iScrollBarHeight / m_iItemCount); 
  }
  
  UpdatePos();  
}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
void CFDirList::DrawItems()
{
  // have directory contents been read?
  if (!m_bDirRead) {
    ClearItems();
    ReadDir();
    m_bDirRead= TRUE;
  }

  // rectangle in which an item is displayed
  RECT itemRect;
  itemRect.left  = m_iPosX + 1;
  itemRect.top   = m_iPosY + 1;
  itemRect.right = m_iPosX + m_iItemWidth;
  itemRect.bottom= m_iPosY + m_iItemHeight - 1;

  // display list entries
  ITEMLISTENTRY *pAuxEntry= m_pEntries;
  int pos= 0, index= 0;  
  
  while (pAuxEntry) {
    if ((pos >= m_iItemPos)&&(pos < (m_iItemPos + m_iMaxVisItems))) {
      // item selected?
      if ((m_iSelItem != -1)&&(m_iSelItem - m_iItemPos == index)) {      
        SetBkColor(m_hdc, m_colHiBack);
        SetTextColor(m_hdc, m_colHiText);
        
        itemRect.right+= 2 * m_iHorMargin - 1;
        FillRect(m_hdc, &itemRect, m_hBrushHiBack);
        itemRect.right-= 2 * m_iHorMargin - 1;
        // redraw the items        
        ExtTextOut(m_hdc, m_iPosX + m_iHorMargin + 18, m_iPosY + m_iItemHeight * index + 1,
                   ETO_CLIPPED, &itemRect, pAuxEntry->pszItem, _tcslen(pAuxEntry->pszItem), NULL);
        SHFILEINFO shfi;
        HANDLE h= (HANDLE)SHGetFileInfo(pAuxEntry->pszAddInfo, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
        ImageList_Draw((HIMAGELIST)h, shfi.iIcon, m_hdc, m_iPosX + m_iHorMargin, m_iPosY + m_iItemHeight * index + 1, ILD_BLEND50);
      } else {
        SetBkColor(m_hdc, m_colBack);
        SetTextColor(m_hdc, m_colText);
      
        // clear old item
        itemRect.right+= 2 * m_iHorMargin - 1;
        FillRect(m_hdc, &itemRect, m_hBrushBack);
        itemRect.right-= 2 * m_iHorMargin - 1;
        // redraw the items
        ExtTextOut(m_hdc, m_iPosX + m_iHorMargin + 18, m_iPosY + m_iItemHeight * index + 1,
                   ETO_CLIPPED, &itemRect, pAuxEntry->pszItem, _tcslen(pAuxEntry->pszItem), NULL);
        SHFILEINFO shfi;
        HANDLE h= (HANDLE)SHGetFileInfo(pAuxEntry->pszAddInfo, 0, &shfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
        ImageList_Draw((HIMAGELIST)h, shfi.iIcon, m_hdc, m_iPosX + m_iHorMargin, m_iPosY + m_iItemHeight * index + 1, ILD_NORMAL);
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
BOOL CFDirList::SetCurrDir(LPTSTR pCurrDir)
{
  // directory up?
  if (_tcscmp(pCurrDir, TEXT("..")) == 0) {
    int i= _tcslen(m_szCurrDir);
    while ((i > 0)&&(m_szCurrDir[i] != '\\')) i--;
    m_szCurrDir[i]= 0;
    Sleep(10);
    m_bDirRead= FALSE;
  } else if ( (_tcscmp(m_szCurrDir, pCurrDir) == 0)) { // it the same as before? then read directory
    m_bDirRead= FALSE;
  } else {
    _tcscpy(m_szCurrDir, pCurrDir);  // if not, set new name and highlight
    m_bDirRead= TRUE;
  }
  return(TRUE);
  
}

//--------------------------------------------------------------------------------
BOOL CFDirList::ReadDir()
{
  WIN32_FIND_DATA ffd;
  HANDLE hFile;
  TCHAR szFilePath[256];
  TCHAR szFullPath[256];
  
  // first entry is always "directory up" except for root-directory
  if (_tcscmp(m_szCurrDir, TEXT(""))) AddItem(TEXT(".."), TEXT(".."));
  
  // create wild card string for directory exploring
  _tcscpy(szFilePath, m_szCurrDir);
  _tcscat(szFilePath, TEXT("\\*.*"));

  hFile= FindFirstFile(szFilePath, &ffd);
  if (hFile == INVALID_HANDLE_VALUE) return(FALSE);
  
  do {
    // create full qualified path of file
    _tcscpy(szFullPath, m_szCurrDir);
    _tcscat(szFullPath, TEXT("\\"));
    _tcscat(szFullPath, ffd.cFileName);
    if (IsDirectory((LPCTSTR) &szFullPath))
      AddItem(ffd.cFileName, szFullPath);
  } while(FindNextFile(hFile, &ffd));
  CloseHandle(hFile);

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFDirList::AddItem(TCHAR *pszItem, TCHAR *pszAddInfo)
{
  // allocate memory
  ITEMLISTENTRY *pTempEntry= new ITEMLISTENTRY;
  memset(pTempEntry, 0, sizeof(ITEMLISTENTRY));

  if (!pTempEntry) return(FALSE);
  pTempEntry->pszItem= (TCHAR*)malloc((_tcslen(pszItem) + 1) * sizeof(TCHAR));
  if (!pTempEntry->pszItem) return(FALSE);
  _tcscpy(pTempEntry->pszItem, pszItem);
  
  // if additional information was specified
  if (pszAddInfo) {
    pTempEntry->pszAddInfo= (TCHAR*)malloc((_tcslen(pszAddInfo) + 2)*sizeof(TCHAR));
    if (!pTempEntry->pszAddInfo) return(FALSE);
    _tcscpy(pTempEntry->pszAddInfo, pszAddInfo);
  } else {
    pTempEntry->pszAddInfo= pTempEntry->pszItem;
  }
  
  pTempEntry->pNext= NULL;
  
  // enqueue
  if (m_pEntries == NULL) {
    m_pLast= m_pEntries= pTempEntry;
  } else {
    m_pLast->pNext= pTempEntry;
    m_pLast= m_pLast->pNext;
  }

  // increase counter of items
  m_iItemCount++;
  
  // calculate new step width for scrollbar-slider
  m_iSliderStep= (m_iScrollBarHeight / m_iItemCount); 

  // slider bitmap also changes so delete and create a new one on demand
  if (m_bmpSlider) {
    delete(m_bmpSlider);
    m_bmpSlider= NULL;
  }

  return(TRUE);
}


//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFDirList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  DrawItems();

  if (SetCurrDir(pEntry->pszAddInfo)) {
    m_app->DrawDoubleBuffer();
    Draw();
  }
  
  m_pSystem->QueueEvent(WM_ITEMLISTSELECT, m_ulID, (void*)pEntry);
}

//--------------------------------------------------------------------------------
/// Checks whether the specified directory name is really one.
BOOL CFDirList::IsDirectory(LPCTSTR pDir)
{
  DWORD dwAttr= GetFileAttributes(pDir);
  return((dwAttr != (DWORD)-1)&&(dwAttr & FILE_ATTRIBUTE_DIRECTORY));
}

