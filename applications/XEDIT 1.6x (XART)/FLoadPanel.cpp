// FLoadPanel.cpp: implementation of the CFLoadPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FLoadPanel.h"
#include "Resource.h"

//--------------------------------------------------------------------------------
CFLoadPanel::CFLoadPanel(LPCTSTR pFilePath)
{
  m_waveList= NULL;
  m_btnOK= NULL;
  m_btnCancel= NULL;
  m_bCreated= FALSE;
  m_pFileToLoad= NULL;
  _tcscpy(m_szFilePath, pFilePath);
}

//--------------------------------------------------------------------------------
CFLoadPanel::~CFLoadPanel()
{
  SAFE_RELEASE(m_waveList);
  SAFE_RELEASE(m_btnOK);
  SAFE_RELEASE(m_btnCancel);
}

//--------------------------------------------------------------------------------
CFLoadPanel::Create()
{
  // load images for waveList
  CFBitmap *cbUpArrow= new CFBitmap(m_hdc);
  if (!cbUpArrow->Load(IDB_ARROWUP)) return(FALSE);
  CFBitmap *cbDownArrow= new CFBitmap(m_hdc);
  if (!cbDownArrow->Load(IDB_ARROWDOWN)) return(FALSE);
  // create the item listbox
  m_waveList= new CFFileList(8, 150, cbUpArrow, cbDownArrow);
  Add(m_waveList);
  m_waveList->SetVisible(TRUE);  
  m_waveList->SetFilters(TEXT(".wav .WAV .Wav .WAv .WaV . wAV . waV"));
  m_waveList->SetCurrDir(m_szFilePath);
  m_waveList->SetID(ID_ITEM_LOAD_SELECTED);

  // create buttons
  m_btnOK= new CFButton(TEXT("OK"), 60, 25);
  m_btnOK->SetID(ID_BTN_LOAD_OK);
  Add(m_btnOK);
  m_btnOK->SetVisible(TRUE);
  m_btnCancel= new CFButton(TEXT("Cancel"), 60, 25);
  m_btnCancel->SetID(ID_BTN_LOAD_CANCEL);
  Add(m_btnCancel);
  m_btnCancel->SetVisible(TRUE);
  
  m_bCreated= TRUE;
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFLoadPanel::SetX(int iPosX)
{
  m_iPosX= iPosX;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFLoadPanel::SetY(int iPosY)
{
  m_iPosY= iPosY;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFLoadPanel::UpdatePos()
{
  m_waveList->SetX(m_iPosX + 10);
  m_waveList->SetY(m_iPosY + 10);
  m_btnOK->SetX(m_iPosX + 200);
  m_btnOK->SetY(m_iPosY + 50);
  m_btnCancel->SetX(m_iPosX + 200);
  m_btnCancel->SetY(m_iPosY + 100);
}


BOOL CFLoadPanel::ItemListSelected(DWORD dwListID, LPITEMLISTENTRY pEntry)
{
  switch(dwListID) {
    case ID_ITEM_LOAD_SELECTED:
      SetFileToLoad(pEntry->pszAddInfo);
      break;

    default:
      return(FALSE); // event not handled
      break;
  }
  return(FALSE);  // event not handled
}


BOOL CFLoadPanel::ButtonReleased(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

BOOL CFLoadPanel::ButtonPressed(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

void CFLoadPanel::Refresh()
{
  m_waveList->SetCurrDir(m_szFilePath);
}

void CFLoadPanel::SetFilePath(LPCTSTR pFilePath) 
{ 
  _tcscpy(m_szFilePath, pFilePath);
  m_waveList->SetCurrDir(m_szFilePath);
}
