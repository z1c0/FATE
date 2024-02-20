#include "../base/FateTypeDefs.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"
#include "FDropList.h"

//--------------------------------------------------------------------------------
CFDropList::CFDropList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpDropArrow,
                     CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow) :
                     CFItemList(iMaxVisItems, iItemWidth, bmpUpArrow, bmpDownArrow)
{
  m_bmpDropArrow = bmpDropArrow;
  m_bDropState = false;
  m_bDropEnabled = true;
  m_bmpTop = new CFBitmap();
  m_pszSelItem = NULL;
}

//--------------------------------------------------------------------------------
CFDropList::~CFDropList() 
{
  SAFE_DELETE(m_bmpDropArrow);
  SAFE_DELETE(m_bmpTop);
}

//--------------------------------------------------------------------------------
bool CFDropList::StylusDown(int xPos, int yPos) 
{
  if (!m_bDropEnabled)
    return(false);
 
  // expand / collapse droplist?
  if (m_bmpDropArrow->PointInside(xPos, yPos))
  {
    m_bDropState= !m_bDropState;
    if (m_bDropState)
    {
      m_app->m_pSysCapt= this;
      m_bmpBack->SaveUnder();
      Draw();
      m_pSystem->QueueEvent(FATE_EVENT_ID_DROPLISTEXPAND, m_ulID, NULL);    
    }
    else
    {
      if (m_app->m_pSysCapt == this) m_app->m_pSysCapt= NULL;
      m_bmpBack->RestoreUnder();
      m_pSystem->QueueEvent(FATE_EVENT_ID_DROPLISTCOLLAPSE, m_ulID, NULL);
    }    
    return(true);
  
  } else if (m_bDropState) {  // if list is dropped down ...
    CFItemList::StylusDown(xPos, yPos);
    return(true);
  }  
    
  return(false); 
}

//--------------------------------------------------------------------------------
bool CFDropList::StylusMove(int xPos, int yPos)
{
  if (!m_bDropEnabled)
    return(false);

  if (m_bDropState) {  // if list is dropped down ...
    CFItemList::StylusMove(xPos, yPos);
    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFDropList::StylusUp(int xPos, int yPos)
{
  if (!m_bDropEnabled)
    return(false);
  
  return(m_bDropState);
}

//--------------------------------------------------------------------------------
void CFDropList::SetSystem(CFSystem *pSystem)
{
  CFItemList::SetSystem(pSystem);
  
  m_bmpTop->SetDestBitmap(m_pSystem->GetDoubleBuffer());
  // If no bitmap wase specified, it is created.
  if (!m_bmpDropArrow)
  {
    m_bmpDropArrow = new CFBitmap();
    m_bmpDropArrow->operator=(*m_bmpDownArrow);
    m_bmpDropArrow->SetDestBitmap(m_pSystem->GetDoubleBuffer());
  }
  m_bmpTop->Create(m_bmpBack->GetWidth(), m_bmpDropArrow->GetHeight());

  CreateBackGround();

  UpdatePos();
}

//--------------------------------------------------------------------------------
void CFDropList::Draw()
{
  LPCTSTR pSel= TEXT(" ");
  if ((!m_pszSelItem)&&(m_pEntries)&&(m_pEntries->pszItem))
  {
    pSel = m_pEntries->pszItem;
  }
  else if ((m_pszSelItem)&&(!m_pEntries))
  {
    pSel= TEXT(" ");
  }
  else
  {
    pSel = m_pszSelItem;
  }
  DrawSelItem(pSel);
  
  if (m_bDropState)
  {  
    // List is dropped down.
    CFItemList::Draw();
  }
}

//--------------------------------------------------------------------------------
void CFDropList::DrawSelItem(LPCTSTR pSel)
{  
  if (!pSel)
  {
    pSel = TEXT(" ");
  }
  m_pszSelItem = pSel;  
  // Draw the rectangle for selected item.
  m_bmpTop->SetBackgroundColor(m_colBack);
  m_bmpTop->SetColor(m_colBorder);
  m_bmpTop->DrawFilledRect(0, 0, m_bmpTop->GetWidth(), m_bmpTop->GetHeight());
  // Draw selected item.
  RECT rect;
  rect.left= 1;
  rect.top= 1;
  rect.right = m_iItemWidth - 2;
  rect.bottom= m_bmpTop->GetHeight() - 2;
  m_bmpTop->DrawText(pSel, rect);
  m_bmpTop->Blit();
  m_bmpDropArrow->Blit();
}

//--------------------------------------------------------------------------------
void CFDropList::UpdatePos()
{
  if (m_pSystem)
  {
    m_bmpTop->SetX(m_iPosX);
    m_bmpTop->SetY(m_iPosY);
    m_bmpDropArrow->SetX(m_bmpTop->GetRight() - m_bmpDropArrow->GetWidth());
    m_bmpDropArrow->SetY(m_iPosY);

    CFItemList::UpdatePos(m_bmpTop->GetHeight());
  } 
}

//--------------------------------------------------------------------------------
/// Method is called when an items was selected.
void CFDropList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  if (m_app->m_pSysCapt == this) m_app->m_pSysCapt= NULL;
  m_bDropState= false;
  DrawSelItem(pEntry->pszItem);
  m_app->DrawDoubleBuffer();
  m_bmpBack->RestoreUnder();
  m_pSystem->QueueEvent(FATE_EVENT_ID_DROPLISTSELECT, m_ulID, (void*)pEntry);
  m_pSystem->QueueEvent(FATE_EVENT_ID_DROPLISTCOLLAPSE, m_ulID, NULL);  
}

//--------------------------------------------------------------------------------
/// If color-settings have changed the background is adapted in this method.
bool CFDropList::CreateBackGround()
{
  if (!CFItemList::CreateBackGround())
  {
    return false;
  }
  
  // Safety check if bitmap is valid.
  if (m_bmpTop->IsValid())
  {
    // Set colors appropriately
    m_bmpTop->SetBackgroundColor(m_colBack);
    m_bmpTop->SetColor(m_colText);
    m_bmpTop->SetTextColor(m_colText);
    // Draw the rectangle for selected item.
    m_bmpTop->DrawFilledRect(m_bmpTop->GetX(), m_bmpTop->GetY(), m_bmpTop->GetRight(), m_bmpTop->GetBottom());
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
/// Selects an item by specifiying its index in the list, starting with 0.
bool CFDropList::SetSelectedItem(int iNr)
{
  ITEMLISTENTRY *pAuxEntry = m_pEntries;
  int i = 0;
  
  while ((pAuxEntry)&&(i++ < iNr)) {
    pAuxEntry = pAuxEntry->pNext;
  }
    
  if (!pAuxEntry) {
    return false;
  }
  
  m_pszSelItem = pAuxEntry->pszItem;
  
  if (m_pSystem) {
    DrawSelItem(m_pszSelItem);
  }
  
  return true;
}
