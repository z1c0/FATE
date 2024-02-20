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

  if (CFFile::IsDirectory(pCurrDir))
  {
    // directory up?
    if (!_tcscmp(pCurrDir, TEXT("..")))
    {
      int i = _tcslen(m_szCurrDir) - 2;
      while (i > 0 && m_szCurrDir[i] != CFSystem::GetDirectorySeparator()[0])
      {
        i--;
      }
      m_szCurrDir[i + 1]= 0;

    }
    else
    {
      _tcscpy(m_szCurrDir, pCurrDir);
      _tcscat(m_szCurrDir, CFSystem::GetDirectorySeparator());
    }
    m_bDirRead = false;
    return true;
  }
  return false;
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
  printf("dir: %s\n", m_szCurrDir);
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
  DrawItems();

  if (SetCurrDir(pEntry->pszAddInfo))
  {
    m_app->DrawDoubleBuffer();
    Draw();
  }
  
  m_pSystem->QueueEvent(FATE_EVENT_ID_ITEMLISTSELECT, m_ulID, (void*)pEntry);
}
