//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FDirPanel.h"
#include "Resource.h"
#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
CFDirPanel::CFDirPanel(LPTSTR pDirectory)
{
  m_dirList   = NULL;
  m_btnOK     = NULL;
  m_btnCancel = NULL;
  m_bCreated  = FALSE;
  m_pDirectory= pDirectory;
}

//--------------------------------------------------------------------------------
CFDirPanel::~CFDirPanel()
{
  SAFE_DELETE(m_dirList);
  SAFE_DELETE(m_btnOK);
  SAFE_DELETE(m_btnCancel);
}

//--------------------------------------------------------------------------------
CFDirPanel::Create()
{
  // load images for waveList
  CFBitmap *cbUpArrow= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbUpArrow->Load(IDB_ARROWUP)) return(FALSE);
  CFBitmap *cbDownArrow= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDownArrow->Load(IDB_ARROWDOWN)) return(FALSE);
  
  // create the item listbox
  m_dirList= new CFDirList(8, 150, cbUpArrow, cbDownArrow);
  Add(*m_dirList);
  m_dirList->SetVisible(TRUE);  
  m_dirList->SetCurrDir(m_pDirectory);
  m_dirList->SetId(ID_ITEM_DIR_SELECTED);

  // create buttons
  m_btnOK= new CFButton(TEXT("OK"), 60, 25);
  m_btnOK->SetId(ID_BTN_DIR_OK);
  Add(*m_btnOK);
  m_btnOK->SetVisible(TRUE);
  m_btnCancel= new CFButton(TEXT("Cancel"), 60, 25);
  m_btnCancel->SetId(ID_BTN_DIR_CANCEL);
  Add(*m_btnCancel);
  m_btnCancel->SetVisible(TRUE);
  
  m_bCreated= TRUE;
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFDirPanel::SetX(int iPosX)
{
  m_iPosX= iPosX;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFDirPanel::SetY(int iPosY)
{
  m_iPosY= iPosY;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFDirPanel::UpdatePos()
{
  m_dirList->SetX(m_iPosX + 10);
  m_dirList->SetY(m_iPosY + 10);
  m_btnOK->SetX(m_iPosX + 200);
  m_btnOK->SetY(m_iPosY + 50);
  m_btnCancel->SetX(m_iPosX + 200);
  m_btnCancel->SetY(m_iPosY + 100);
}

bool CFDirPanel::ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry)
{
  switch(dwListID) {
    case ID_ITEM_DIR_SELECTED:
      SetDirectory(m_dirList->GetCurrDir());
      break;

    default:
      return(FALSE); // event not handled
      break;
  }
  return(FALSE);  // event not handled
}

bool CFDirPanel::ButtonReleased(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

bool CFDirPanel::ButtonPressed(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

void CFDirPanel::Refresh()
{
  m_dirList->Refresh();
}
