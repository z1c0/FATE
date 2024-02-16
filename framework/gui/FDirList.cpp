#ifdef _WIN32

#include <windows.h>
#include "FDirList.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"


//--------------------------------------------------------------------------------
CFDirList::CFDirList(int iMaxVisItems, int iItemWidth, CFBitmap *cbUpArrow, CFBitmap *cbDownArrow) :
            CFItemList(iMaxVisItems, iItemWidth, cbUpArrow, cbDownArrow)
{
  CFSystem::GetPathToApplication(m_szCurrDir);
  m_bDirRead = FALSE;
  m_isFileList = true;
}


//--------------------------------------------------------------------------------
void CFDirList::BeforeDrawItems()
{
  // have directory contents been read?
  if (!m_bDirRead)
  {
    ClearItems();
    ReadDir();
    m_bDirRead= TRUE;
  }

}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
bool CFDirList::SetCurrDir(LPTSTR pCurrDir)
{
  // directory up?
  if (_tcscmp(pCurrDir, TEXT("..")) == 0)
  {
    int i= _tcslen(m_szCurrDir);
    while ((i > 0)&&(m_szCurrDir[i] != '\\')) i--;
    m_szCurrDir[i]= 0;
    m_bDirRead = FALSE;
  }
  else if ((_tcscmp(m_szCurrDir, pCurrDir) == 0)) // it the same as before? then read directory
  {
    m_bDirRead = FALSE;
  }
  else
  {
    _tcscpy(m_szCurrDir, pCurrDir);  // if not, set new name and highlight
    m_bDirRead = TRUE;
  }

  return true;
}

//--------------------------------------------------------------------------------
bool CFDirList::ReadDir()
{
  TCHAR szFilePath[256];
  TCHAR szFullPath[256];
  
  // first entry is always "directory up" except for root-directory
  if (_tcscmp(m_szCurrDir, TEXT("")))
  {
    AddItem(TEXT(".."), TEXT(".."));
  }
  
  // create wild card string for directory exploring
  _tcscpy(szFilePath, m_szCurrDir);
  _tcscat(szFilePath, TEXT("\\*.*"));

  WIN32_FIND_DATA ffd = {0};
  HANDLE hFile = FindFirstFile(szFilePath, &ffd);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    return false;
  }
  
  do
  {
    // create full qualified path of file
    _tcscpy(szFullPath, m_szCurrDir);
    _tcscat(szFullPath, TEXT("\\"));
    _tcscat(szFullPath, ffd.cFileName);
    if (IsDirectory(szFullPath) && _tcscmp(ffd.cFileName, TEXT(".")) != 0 && _tcscmp(ffd.cFileName, TEXT("..")) != 0)
    {
      AddItem(ffd.cFileName, szFullPath);
    }
  }
  while (FindNextFile(hFile, &ffd));
  
  FindClose(hFile);

  return true;
}

//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFDirList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  DrawItems();

  if (SetCurrDir(pEntry->pszAddInfo))
  {
    m_app->DrawDoubleBuffer();
    Draw();
  }
  
  m_pSystem->QueueEvent(WM_ITEMLISTSELECT, m_ulID, (void*)pEntry);
}

//--------------------------------------------------------------------------------
/// Checks whether the specified directory name is really one.
bool CFDirList::IsDirectory(LPCTSTR pDir) const
{
  DWORD dwAttr = GetFileAttributes(pDir);
  return (dwAttr != (DWORD)-1) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY);
}

#endif