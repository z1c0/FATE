#include "FMenu.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"


//--------------------------------------------------------------------------------
CFMenu::CFMenu()
{
  // inits
  m_iVerSpace= 2;  // space around menu text
  m_iHorSpace= 3;
  m_iMainNr= -1;
  m_pItems= NULL;
  m_pCurrOpen= NULL;
  m_bCreated= false;
  m_bSubMenuOpen= false;

  // default values for colors
  m_colText  = RGB(255, 255, 255);
  m_colBorder= RGB(0, 0, 125);
  m_colBack  = RGB(0, 0, 255);
 
  // Calculate necessary height of items
  RECT rect = {0};
  CFBitmap::CalcRectForText(TEXT("W"), rect);
  m_iItemHeight = (rect.bottom - rect.top) + 2 * m_iVerSpace;
}

//--------------------------------------------------------------------------------
CFMenu::~CFMenu()
{
  // safe-release any entries
  if (m_pItems) {
    ClearEntries();
  }
}

//--------------------------------------------------------------------------------
void CFMenu::Draw()
{
  MENUITEM *pTemp;
  
  // are the bitmaps already created?
  if (!m_bCreated) {
    if (Create()) m_bCreated= true;
    else return;
  }
    
  pTemp= m_pItems;
  while(pTemp) {      
    pTemp->pBmp->SaveUnder();
    pTemp->pBmp->Blit();
    pTemp= pTemp->pNext;
  }
}

//--------------------------------------------------------------------------------
/// Draws an expanded menu.
void CFMenu::OpenMenu(MENUITEM *pItem)
{
  MENUITEM *pTemp = pItem->pDown;

  // if there is no submenu, leave immediately
  if (!pTemp) return;
  
  while(pTemp) {      
    pTemp->pBmp->SaveUnder();
    pTemp->pBmp->Blit();
    pTemp= pTemp->pDown;
  }

  // save currently open menu
  m_app->m_pSysCapt= this;
  m_pCurrOpen= pItem;
}

//--------------------------------------------------------------------------------
/// Closes the currently opened menu.
void CFMenu::CloseMenu()
{
  MENUITEM *pTemp= NULL;
  
  pTemp= m_pCurrOpen->pDown;
  while(pTemp->pDown) {      
    pTemp= pTemp->pDown;
  }
  while (pTemp) {
    pTemp->pBmp->RestoreUnder();
    pTemp= pTemp->pUp;
  }

  // currently no menu is opened
  m_app->m_pSysCapt= NULL;
  m_pCurrOpen= NULL;
}

//--------------------------------------------------------------------------------
bool CFMenu::StylusDown(int xPos, int yPos)
{
  MENUITEM *pTemp= NULL;

  // was the main menu clicked?
  pTemp= m_pItems;
  while (pTemp) {
    if (pTemp->pBmp->PointInside(xPos, yPos)) {
      // is the menu already opened?
      if (m_pCurrOpen)
      {
        // the same menu was tapped
        if (m_pCurrOpen == pTemp)
        {
          CloseMenu();
        }
        else
        {
          m_pSystem->QueueEvent(WM_MENUSELECTION, m_ulID, (void*)MAKELPARAM(pTemp->iPosHor, pTemp->iPosVer));
          CloseMenu();
          OpenMenu(pTemp);
        }
      }
      else
      {
        m_pSystem->QueueEvent(WM_MENUSELECTION, m_ulID, (void*)MAKELPARAM(pTemp->iPosHor, pTemp->iPosVer));
        OpenMenu(pTemp);
      }
      return(true);
    }
    pTemp= pTemp->pNext;
  }  
  
  // was a sub-menu clicked?
  if (m_pCurrOpen) {
    pTemp= m_pCurrOpen->pDown;
    while(pTemp) {
      if (pTemp->pBmp->PointInside(xPos, yPos)) {
        m_pSystem->QueueEvent(WM_MENUSELECTION, m_ulID, (void*)MAKELPARAM(pTemp->iPosHor, pTemp->iPosVer));
        m_bSubMenuOpen= true;
        return(true);
      }
      pTemp= pTemp->pDown;
    }  
  }

  return(false); // event not handled
}

//--------------------------------------------------------------------------------
bool CFMenu::StylusUp(int xPos, int yPos)
{
  // submenu clicked
  if (m_bSubMenuOpen) {
    CloseMenu();
    m_bSubMenuOpen= false;
    return(true);
  }

  return(false); // event not handled
}

//--------------------------------------------------------------------------------
bool CFMenu::AddEntry(LPCTSTR pText)
{
  // create new menu-entry
  MENUITEM *pEntry= new MENUITEM;
  if (!pEntry) return(false);
  pEntry->pText= pText;
  pEntry->bEnabled= true;
  pEntry->pNext= NULL;
  pEntry->pDown= NULL;
  pEntry->iPosVer= 0;
  pEntry->iPosHor= ++m_iMainNr;
   
  // enqueue
  if (!m_pItems) {
    m_pItems= pEntry;
  } else {
    MENUITEM *pTemp= m_pItems;
    
    while(pTemp->pNext) {      
      pTemp= pTemp->pNext;
    }
    pTemp->pNext= pEntry;
  }
  return(true);
}

//--------------------------------------------------------------------------------
/// Adds a sub-menu-entry for an existing main-menu-entry. "iPos" is the 0-based
/// index of the main-menu-entry, which must be valid.
/// If "iPos" is an invalid index, false is returned.
bool CFMenu::AddSubEntry(LPCTSTR pText, int iPos)
{
  if ((iPos >= 0)&&(iPos <= m_iMainNr)) {
    // go to main-entry specified by iPos
    int i= 0;
    MENUITEM *pTemp= m_pItems;    
    while(i++ < iPos) pTemp= pTemp->pNext;

    // create new menu-entry
    MENUITEM *pEntry= new MENUITEM;
    if (!pEntry) return(false);
    pEntry->pText= pText;
    pEntry->bEnabled= true;
    pEntry->pNext= NULL;
    pEntry->pDown= NULL;
    pEntry->pUp= NULL;
    pEntry->iPosHor= iPos;
    pEntry->iPosVer= 1;
    
    // enqueue
    if (!pTemp->pDown) {
      pTemp->pDown= pEntry;
    } else {
      MENUITEM *pTempDown= pTemp->pDown;
      pEntry->iPosVer++;
    
      while(pTempDown->pDown) {      
        pTempDown= pTempDown->pDown;
        pEntry->iPosVer++;
      }
      pEntry->pUp= pTempDown;
      pTempDown->pDown= pEntry;
    }  
  }
  return(false);
}

//--------------------------------------------------------------------------------
/// Menu entries are cleaned up.
bool CFMenu::ClearEntries()
{
  // delete itemstructures  
  MENUITEM *pTemp, *pOld, *pDown;

  pTemp= m_pItems;
  while(pTemp) {
    pDown= pTemp->pDown;
    while(pDown) {
      pOld= pDown;
      pDown= pDown->pDown;
      delete(pOld->pBmp);
      delete(pOld);
    }

    pOld= pTemp;
    pTemp= pTemp->pNext;
    delete(pOld->pBmp);
    delete(pOld);
  }

  m_pItems= NULL;
  m_bCreated= false;  
  m_iMainNr= -1;
  
  return(true);  
}

//--------------------------------------------------------------------------------
/// Method is called when menu has a valid HDC. Menu Bitmaps are created here.
bool CFMenu::Create()
{
  MENUITEM *pTemp= m_pItems;
  MENUITEM *pTempDown;  
  int iMaxWidth;
  int iOldX= 0;
  int iOldY= 0;
    
  while(pTemp) {      
    // create bitmap and set positions accordingly
    pTemp->pBmp= CreateMenuBmp(pTemp->pText);
    pTemp->pBmp->SetX(iOldX);
    iOldY= pTemp->pBmp->GetBottom() - 1;
    pTempDown= pTemp->pDown;
    // get maximum width for subentries
    iMaxWidth= GetMaxWidth(pTemp);
    while(pTempDown) {
      pTempDown->pBmp= CreateMenuBmp(pTempDown->pText, iMaxWidth);
      pTempDown->pBmp->SetX(iOldX);
      pTempDown->pBmp->SetY(iOldY);
      iOldY= pTempDown->pBmp->GetBottom() - 1;
      pTempDown= pTempDown->pDown;
    }
    iOldX= pTemp->pBmp->GetRight() - 1;
    pTemp= pTemp->pNext;
  }  
  return(true);
}

//--------------------------------------------------------------------------------
/// Creates a Bitmap for the specified menu entry using standard color settings and
/// the specified text.
CFBitmap* CFMenu::CreateMenuBmp(LPCTSTR pText, int iWidth /* = -1 */)
{
  CFBitmap *pBmp;
  RECT rectText;
  
  // get needed space for text
  rectText.left= m_iHorSpace + 1;
  rectText.top= m_iVerSpace;
  rectText.right= m_iHorSpace + 1;
  rectText.bottom= m_iVerSpace;

  CFBitmap::CalcRectForText(pText, rectText);
  
  // if a widht was specified, use it
  if (iWidth != -1) rectText.right= iWidth;

  // Create the bitmap.
  CFBitmap *pDestBmp = m_pSystem->GetDoubleBuffer();
  pBmp= new CFBitmap(*pDestBmp);
  pBmp->Create(rectText.right + 2 * m_iHorSpace, m_iItemHeight);
  
  // Draw the bitmap.
  pBmp->SetColor(m_colBorder);
  pBmp->SetBackgroundColor(m_colBack);
  pBmp->DrawFilledRect(0, 0, pBmp->GetWidth(), m_iItemHeight);

  pBmp->SetTextColor(m_colText);
  pBmp->DrawText(pText, rectText);
  
  return pBmp;
}

//--------------------------------------------------------------------------------
/// Calculates the maximum widht of menu entries for a sub-menu, which is then used
/// as the common widht.
int CFMenu::GetMaxWidth(MENUITEM *pItem)
{
  RECT rectText;
  MENUITEM *pTemp;
  int iMax = 0;

  pTemp = pItem;
  while(pTemp) {  
    // Get needed space for text.
    rectText.left= m_iHorSpace + 1;
    rectText.top= m_iVerSpace;
    rectText.right= m_iHorSpace + 1;
    rectText.bottom= m_iVerSpace;
    CFBitmap::CalcRectForText(pTemp->pText, rectText);
    iMax = max(iMax, rectText.right);
    pTemp = pTemp->pDown;
  }

  return iMax;
}
