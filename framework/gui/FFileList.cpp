#include "FFileList.h"
#include "../base/FateApp.h"
#include "../base/FBitmap.h"
#include "../base/FFile.h"
#include "../base/FDirectory.h"

//--------------------------------------------------------------------------------
CFFileList::CFFileList(int iMaxVisItems, int iItemWidth, CFBitmap *cbUpArrow, CFBitmap *cbDownArrow) :
  CFItemList(iMaxVisItems, iItemWidth, cbUpArrow, cbDownArrow)
{
  CFSystem::GetPathToApplication(m_szCurrDir);
  m_bDirRead = false;
  m_pFilters = NULL;
  m_isFileList = true;
}

//--------------------------------------------------------------------------------
CFFileList::~CFFileList()
{
  // filter cleanup?
  if (m_pFilters)
  {
    RemoveFilters();
  }
}

//--------------------------------------------------------------------------------
void CFFileList::BeforeDrawItems()
{
  // have directory contents been read?
  if (!m_bDirRead)
  {
    ClearItems();
    ReadDir();
    m_bDirRead = true;
  }
}

//--------------------------------------------------------------------------------
/// Sets current directory for file-choser. If directory cannot be found FALSE is
/// returned and change is not done.
bool CFFileList::SetCurrDir(LPCTSTR pszDir)
{
  TCHAR pCurrDir[MAX_PATH];

  _tcscpy(pCurrDir, pszDir);

  // remove finishing '\' if necessary
  int iLen= _tcslen(pCurrDir);
  if ((iLen)&&(pCurrDir[iLen-1] == '\\')) pCurrDir[iLen-1]= 0;

  if (CFFile::IsDirectory(pCurrDir))
  {
    // directory up?
    if (!_tcscmp(pCurrDir, TEXT(".."))) {
      int i= _tcslen(m_szCurrDir) - 2;

      while ((i > 0)&&(m_szCurrDir[i] != '\\')) i--;
      m_szCurrDir[i+1]= 0;

    } else {
      _tcscpy(m_szCurrDir, pCurrDir);
      _tcscat(m_szCurrDir, TEXT("\\"));
    }

    m_bDirRead= false;
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
bool CFFileList::ReadDir()
{
  TCHAR szFilePath[256];
  TCHAR szFullPath[256];
  
  // first entry is always "directory up" except for root-directory
  if (_tcscmp(m_szCurrDir, TEXT("\\")))
  {
    AddItem(TEXT(".."), TEXT(".."));
  }
  
  // create wild card string for directory exploring
  _tcscpy(szFilePath, m_szCurrDir);
  _tcscat(szFilePath, TEXT("*.*"));

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
    _tcscat(szFullPath, fileName);
    
    // exclude current and parent directory
    if (fileName[0] != '.')
    {
      // any filters specified?
      if (!CFFile::IsDirectory(szFullPath) && m_pFilters)
      {
        FILEFILTER *pTemp = m_pFilters;
        while (pTemp)
        {
          int iStrLen = _tcslen(fileName);
          if (iStrLen >= 4 && !_tcscmp(&fileName[iStrLen - 4], pTemp->pszFilter))
          {
            AddItem(fileName, szFullPath);
          }
          pTemp= pTemp->pNext;
        }
      }
      else
      {
        AddItem(fileName, szFullPath);
      }
    }
  }  

  return true;
}

//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFFileList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  // move to selected directory
  if (SetCurrDir(pEntry->pszAddInfo))
  {
    pEntry->ulUser= IL_DIRFLAG;  // mark it as directory
    m_pSystem->QueueEvent(FATE_EVENT_ID_ITEMLISTSELECT, m_ulID, (void*)pEntry);
    Draw();
  }
  else
  {
    CFItemList::ItemSelected(pEntry);
  }
}

//--------------------------------------------------------------------------------
/// A set of file-types for filtering can be specified to the filelist.
/// Parameter "pFilterStr" must have the format ".ext1 .ext2 .ext3 .ext4"
/// Return value indicates wether filter string was specified correctly.
bool CFFileList::SetFilters(LPTSTR pFilterStr)
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
    while(pToken)
    {
      // filter valid?
      if ((_tcslen(pToken) >= 2)&&(pToken[0] == '.')) {
        // add filter
        FILEFILTER *pTemp= new FILEFILTER;
        if (!pTemp) return false;
        pTemp->pszFilter= (TCHAR*)malloc((_tcslen(pToken) + 1) * sizeof(TCHAR));
        _tcscpy(pTemp->pszFilter, pToken);
        pTemp->pNext= NULL;
        // enqueue
        if (!m_pFilters)
        {
          m_pLastFilter= m_pFilters= pTemp;
        }
        else
        {
          m_pLastFilter->pNext= pTemp;
          m_pLastFilter= m_pLastFilter->pNext;
        }
      }
      // get next token
      pToken = _tcstok(NULL, pDel);
    }
    free(pszCopy);
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
/// Removes all filters that have been specified
void CFFileList::RemoveFilters()
{
  // clear file-filter datatstructure
  FILEFILTER* pTemp = m_pFilters;
  while (pTemp)
  {
    FILEFILTER* pOld = pTemp;
    pTemp= pTemp->pNext;
    delete(pOld->pszFilter);
    delete(pOld);
  }
}
