#include "FDirList.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"
#include "../base/FFile.h"
#include "../base/FDirectory.h"

//--------------------------------------------------------------------------------
CFDirList::CFDirList(int iMaxVisItems, int iItemWidth, CFBitmap *cbUpArrow, CFBitmap *cbDownArrow) :
  CFItemList(iMaxVisItems, iItemWidth, cbUpArrow, cbDownArrow)
{
  CFSystem::GetPathToApplication(m_szCurrDir);
  m_bDirRead = false;
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
    m_bDirRead= true;
  }
}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
bool CFDirList::SetCurrDir(const TCHAR* pszDir)
{
  TCHAR pCurrDir[MAX_PATH];
  _tcscpy(pCurrDir, pszDir);
  
  // remove finishing '\' if necessary
  int iLen = _tcslen(pCurrDir);
  if (iLen && pCurrDir[iLen-1] == CFSystem::GetDirectorySeparator()[0])
  {
    pCurrDir[iLen-1] = 0;
  }

  // directory up?
  if (_tcscmp(pCurrDir, TEXT("..")) == 0)
  {
    size_t i = _tcslen(m_szCurrDir);
    while (i > 0 && m_szCurrDir[i] != CFSystem::GetDirectorySeparator()[0])
    {
      i--;
    }
    m_szCurrDir[i] = 0;
    m_bDirRead = false;
  }
  else if ((_tcscmp(m_szCurrDir, pCurrDir) == 0)) // it the same as before? then read directory
  {
    m_bDirRead = false;
  }
  else
  {
    _tcscpy(m_szCurrDir, pCurrDir);  // if not, set new name and highlight
    m_bDirRead = true;
  }

  return true;
}

//--------------------------------------------------------------------------------
bool CFDirList::ReadDir()
{
  // first entry is always "directory up" except for root-directory
  if (!CFFile::IsRoot(m_szCurrDir))
  {
    AddItem(TEXT(".."), TEXT(".."));
  }
  
  CFDirectory directory(m_szCurrDir);
  while (true)
  {
    const TCHAR* fileName = directory.GetNextChild();
    if (!fileName)
    {
      break;
    }

    // create full qualified path of file
    TCHAR szFullPath[256];
    _tcscpy(szFullPath, m_szCurrDir);
    _tcscat(szFullPath, CFSystem::GetDirectorySeparator());
    _tcscat(szFullPath, fileName);
    if (CFFile::IsDirectory(szFullPath) && _tcscmp(fileName, TEXT(".")) != 0 && _tcscmp(fileName, TEXT("..")) != 0)
    {
      AddItem(fileName, szFullPath);
    }
  }

  return true;
}

//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFDirList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  // move to selected directory
  if (SetCurrDir(pEntry->pszAddInfo))
  {
    m_pSystem->QueueEvent(FATE_EVENT_ID_ITEMLISTSELECT, m_ulID, (void*)pEntry);
    Draw();
  }
  else
  {
    CFItemList::ItemSelected(pEntry);
  }
}
