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
bool CFDirList::SetCurrDir(const TCHAR* pCurrDir)
{
  // directory up?
  if (_tcscmp(pCurrDir, TEXT("..")) == 0)
  {
    size_t i = _tcslen(m_szCurrDir);
    while ((i > 0) && (m_szCurrDir[i] != '\\'))
    {
      i--;
    }
    m_szCurrDir[i]= 0;
    m_bDirRead = false;
  }
  else if ((_tcscmp(m_szCurrDir, pCurrDir) == 0)) // it the same as before? then read directory
  {
    m_bDirRead = false;
  }
  else
  {
    _tcscpy(m_szCurrDir, pCurrDir);  // if not, set new name and highlight
    m_bDirRead = false;
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

  CFDirectory directory(szFilePath);
  while (true)
  {
    const TCHAR* fileName = directory.GetNextChild();
    if (!fileName)
    {
      break;
    }

    // create full qualified path of file
    _tcscpy(szFullPath, m_szCurrDir);
    _tcscat(szFullPath, TEXT("\\"));
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
