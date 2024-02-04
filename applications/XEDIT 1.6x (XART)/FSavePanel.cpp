// FSavePanel.cpp: implementation of the CFSavePanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FSavePanel.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CFSavePanel::CFSavePanel(LPTSTR pDirectory)
{
  m_fileToSaveLabel= NULL;
  m_dirList= NULL;
  m_bCreated= FALSE;
  m_btnOK= NULL;
  m_btnCancel= NULL;
  _tcscpy(m_szDirectory, pDirectory); 
}

//--------------------------------------------------------------------------------
CFSavePanel::~CFSavePanel()
{
  SAFE_DELETE(m_fileToSaveLabel);
  SAFE_DELETE(m_dirList);
  SAFE_DELETE(m_btnOK);
  SAFE_DELETE(m_btnCancel);
}

CFSavePanel::Create()
{
  // create and add label for filename
  m_fileToSaveLabel= new CFLabel(TEXT(" "), 245, 17);
  Add(*m_fileToSaveLabel);
  m_fileToSaveLabel->SetVisible(TRUE);
  
  // load images for waveList
  //CFBitmap *cbUpArrow= new CFBitmap(m_hdc); // TODO
  CFBitmap *cbUpArrow= new CFBitmap();
  if (!cbUpArrow->Load(IDB_ARROWUP)) return(FALSE);
  //CFBitmap *cbDownArrow= new CFBitmap(m_hdc);  // TODO
  CFBitmap *cbDownArrow= new CFBitmap();
  if (!cbDownArrow->Load(IDB_ARROWDOWN)) return(FALSE);
  
  // create the item listbox
  m_dirList= new CFDirList(6, 150, cbUpArrow, cbDownArrow);
  Add(*m_dirList);
  m_dirList->SetVisible(TRUE);  
  m_dirList->SetCurrDir(m_szDirectory);
  m_dirList->SetId(ID_ITEM_SAVEDIR_SELECTED);

  // create buttons
  m_btnOK= new CFButton(TEXT("OK"), 60, 25);
  m_btnOK->SetId(ID_BTN_SAVEDIR_OK);
  Add(*m_btnOK);
  m_btnOK->SetVisible(TRUE);
  m_btnCancel= new CFButton(TEXT("Cancel"), 60, 25);
  m_btnCancel->SetId(ID_BTN_SAVEDIR_CANCEL);
  Add(*m_btnCancel);
  m_btnCancel->SetVisible(TRUE);
  
  m_bCreated= TRUE;
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFSavePanel::SetX(int iPosX)
{
  m_iPosX= iPosX;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFSavePanel::SetY(int iPosY)
{
  m_iPosY= iPosY;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFSavePanel::UpdatePos()
{
  m_fileToSaveLabel->SetX(m_iPosX + 10);
  m_fileToSaveLabel->SetY(m_iPosY);
  m_dirList->SetX(m_iPosX + 10);
  m_dirList->SetY(m_fileToSaveLabel->GetBottom() + 10);
  m_btnOK->SetX(m_fileToSaveLabel->GetRight() - m_btnOK->GetWidth());
  m_btnOK->SetY(m_iPosY + 45);
  m_btnCancel->SetX(m_fileToSaveLabel->GetRight() - m_btnCancel->GetWidth());
  m_btnCancel->SetY(m_iPosY + 90);
}

bool CFSavePanel::ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry)
{
  switch(dwListID) {
    case ID_ITEM_SAVEDIR_SELECTED:
      SetDirectory(m_dirList->GetCurrDir());
      break;

    default:
      return(FALSE); // event not handled
      break;
  }
  return(FALSE);  // event not handled
}

bool CFSavePanel::ButtonReleased(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

bool CFSavePanel::ButtonPressed(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

void CFSavePanel::Refresh()
{
  m_dirList->Refresh();
}

void CFSavePanel::SetDirectory(LPTSTR pDirectory) 
{
  TCHAR szFileName[MAX_PATH];
  
  _tcscpy(m_szDirectory, pDirectory); 

  // make new file path
  _stprintf(szFileName, TEXT("%s\\%s"), m_szDirectory, m_szFileName);
  _tcscpy(m_szFileToSaveFullPath, szFileName); 
  m_fileToSaveLabel->SetText(szFileName);
}

void CFSavePanel::SetFileToSave(LPCTSTR pFileToSave) 
{ 
  LPTSTR pFileName;  
  int iDirName;  
  
  _tcscpy(m_szFileToSaveFullPath, pFileToSave); 
  m_fileToSaveLabel->SetText(m_szFileToSaveFullPath);
  
  // file name and directory
  pFileName = (LPTSTR)_tcsrchr(m_szFileToSaveFullPath, '\\');
  
  if (pFileName) {
    _tcscpy(m_szFileName, pFileName + 1);
    iDirName = pFileName - m_szFileToSaveFullPath;
    _tcsncpy(m_szDirectory, m_szFileToSaveFullPath, iDirName);
    m_szDirectory[iDirName + 1] = 0;  
  } else {
    _tcscpy(m_szFileName, m_szFileToSaveFullPath);
    m_szDirectory[0] = 0;
  }
  m_dirList->SetCurrDir(m_szDirectory);
  m_dirList->Refresh();  
};

