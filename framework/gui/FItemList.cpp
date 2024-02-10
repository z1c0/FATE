#include "../base/FateTypeDefs.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"
#include "FItemlist.h"


//--------------------------------------------------------------------------------
CFItemList::CFItemList(int iMaxVisItems, int iItemWidth,
                       CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow) : IFateComponent()
{
  m_colText = RGB(0, 0, 0);
  m_colBack = RGB(255, 255, 255);
  m_colBorder = RGB(0, 0, 0);
  m_colSlide = RGB(204, 204, 204);
  m_colHiBack = RGB(175, 175, 175);
  m_colHiText = RGB(255, 255, 255);

  // allocate memory for backup item
  m_saveEntry.pszItem   = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
  m_saveEntry.pszAddInfo= (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
  m_saveEntry.pNext= NULL;
  
  m_iItemCount= 0;
  m_iItemPos= 0;
  m_pLast= m_pEntries = NULL;
  m_iMaxVisItems= iMaxVisItems;
  m_iItemWidth = iItemWidth;
  m_iSelItem = -1;
  m_isFileList = false;
  m_iPosX= 0; 
  m_iPosY= 0;

  m_bmpUpArrow = bmpUpArrow;
  m_bmpDownArrow = bmpDownArrow;
  m_bmpBack = new CFBitmap();
  m_bmpSlider = NULL;    
}

//--------------------------------------------------------------------------------
CFItemList::~CFItemList()
{ 
  SAFE_DELETE(m_bmpBack);
  SAFE_DELETE(m_bmpUpArrow);
  SAFE_DELETE(m_bmpDownArrow);
  SAFE_DELETE(m_bmpSlider);

  delete(m_saveEntry.pszItem);
  delete(m_saveEntry.pszAddInfo);
  
  // delete itemstructures  
  ITEMLISTENTRY *pTempEntry, *pOldEntry;
  pTempEntry= m_pEntries;
  while(pTempEntry)
  {
    pOldEntry= pTempEntry;
    pTempEntry= pTempEntry->pNext;
    SAFE_DELETE(pOldEntry->pszItem);
    SAFE_DELETE(pOldEntry->pszAddInfo);
    SAFE_DELETE(pOldEntry->pBmp);
    delete(pOldEntry);
  }
}

//--------------------------------------------------------------------------------
/// Method is inherited from IFateComponent and overwritten because some additional
/// initialization using the HDC is necessary.
void CFItemList::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
   
  // Depending on height, calculate number of possible items.
  RECT rect = {0};
  CFBitmap::CalcRectForText(TEXT("W"), rect);
  m_iItemHeight = max(18, (rect.bottom - rect.top));
  
  // if no bitmaps were specified, they are created
  if (!m_bmpUpArrow)
  {
    POINT points[4];
    const int iWidth = 5;

    m_bmpUpArrow = new CFBitmap(*m_pSystem->GetDoubleBuffer());
    m_bmpUpArrow->Create(DEFAULT_ARROW_DIM, DEFAULT_ARROW_DIM);
    m_bmpUpArrow->SolidFill(RGB(200, 200, 200));
    
    points[0].x = m_bmpUpArrow->GetX() + iWidth;
    points[0].y = m_bmpUpArrow->GetBottom() - iWidth;
    points[1].x = m_bmpUpArrow->GetRight() - iWidth;
    points[1].y = m_bmpUpArrow->GetBottom() - iWidth;
    points[2].x = m_bmpUpArrow->GetWidth() / 2;
    points[2].y = m_bmpUpArrow->GetY() + iWidth;
    points[3].x = m_bmpUpArrow->GetX() + iWidth;
    points[3].y = m_bmpUpArrow->GetBottom() - iWidth;

    m_bmpUpArrow->SetColor(RGB(0, 0, 0));
    m_bmpUpArrow->SetBackgroundColor(RGB(200, 200, 200));
    m_bmpUpArrow->DrawFilledRect(0, 0, DEFAULT_ARROW_DIM, DEFAULT_ARROW_DIM);

    m_bmpUpArrow->SetBackgroundColor(RGB(0, 0, 0));
    m_bmpUpArrow->DrawPolygon(points, 4);
  }
  if (!m_bmpDownArrow)
  {
    POINT points[4];
    const int iWidth= 5;

    m_bmpDownArrow = new CFBitmap(*m_pSystem->GetDoubleBuffer());
    m_bmpDownArrow->Create(DEFAULT_ARROW_DIM, DEFAULT_ARROW_DIM);
    m_bmpDownArrow->SolidFill(RGB(200, 200, 200));

    points[0].x= m_bmpDownArrow->GetX() + iWidth;
    points[0].y= m_bmpDownArrow->GetY() + iWidth;
    points[1].x= m_bmpDownArrow->GetRight() - iWidth;
    points[1].y= m_bmpDownArrow->GetY() + iWidth;
    points[2].x= m_bmpDownArrow->GetWidth() / 2;
    points[2].y= m_bmpDownArrow->GetBottom() - iWidth;
    points[3].x= m_bmpDownArrow->GetX() + iWidth;
    points[3].y= m_bmpDownArrow->GetY() + iWidth;

    m_bmpDownArrow->SetColor(RGB(0, 0, 0));
    m_bmpDownArrow->SetBackgroundColor(RGB(200, 200, 200));
    m_bmpDownArrow->DrawFilledRect(0, 0, DEFAULT_ARROW_DIM, DEFAULT_ARROW_DIM);

    m_bmpDownArrow->SetBackgroundColor(RGB(0, 0, 0));
    m_bmpDownArrow->DrawPolygon(points, 4);
  }

  // Create background bitmap.
  m_bmpBack->SetDestBitmap(*m_pSystem->GetDoubleBuffer());
  m_bmpBack->Create(m_iItemWidth + m_bmpUpArrow->GetWidth(), m_iItemHeight * m_iMaxVisItems + 1);
  m_iWidth = m_bmpBack->GetWidth();
  m_iHeight = m_bmpBack->GetHeight();
  
  CreateBackGround();

  // calculate remaining space of scrollbar
  m_iScrollBarHeight = m_bmpBack->GetHeight() - m_bmpUpArrow->GetHeight() - m_bmpDownArrow->GetHeight();    
  
  // calculate step width for scrollbar-slider
  if (m_iItemCount > 0)
  {
    m_iSliderStep= (m_iScrollBarHeight / m_iItemCount); 
  }
  
  UpdatePos();  
}


//--------------------------------------------------------------------------------
bool CFItemList::RemoveItem(int index)
{
  return false;
}

//--------------------------------------------------------------------------------
bool CFItemList::AddPicItem(CFBitmap *bmpPic, LPCTSTR pszItem, LPCTSTR pszAddInfo /* = NULL */)
{
  // allocate memory
  ITEMLISTENTRY *pTempEntry = new ITEMLISTENTRY;
  if (!pTempEntry) return(false);

  // zero-init the structure
  memset(pTempEntry, 0, sizeof(ITEMLISTENTRY));

  pTempEntry->pszItem= (TCHAR*)malloc((_tcslen(pszItem) + 1) * sizeof(TCHAR));
  if (!pTempEntry->pszItem) return(false);
  _tcscpy(pTempEntry->pszItem, pszItem);
  
  // if additional information was specified
  if (pszAddInfo)
  {
    pTempEntry->pszAddInfo= (TCHAR*)malloc((_tcslen(pszAddInfo) + 1)*sizeof(TCHAR));
    if (!pTempEntry->pszAddInfo) return(false);
    _tcscpy(pTempEntry->pszAddInfo, pszAddInfo);
  }
  else
  {
    pTempEntry->pszAddInfo = NULL;
  }

  // if picture was specified?
  pTempEntry->pBmp = bmpPic;
  
  pTempEntry->pNext = NULL;
  // enqueue
  if (m_pEntries == NULL)
  {
    m_pLast= m_pEntries= pTempEntry;
  }
  else
  {
    m_pLast->pNext= pTempEntry;
    m_pLast= m_pLast->pNext;
  }
  pTempEntry->ulIndex= m_iItemCount;

  // increase counter of items
  m_iItemCount++;
  
  // calculate new step width for scrollbar-slider
  m_iSliderStep= (m_iScrollBarHeight / m_iItemCount); 

  // slider bitmap also changes so delete and create a new one on demand
  if (m_bmpSlider)
  {
    delete(m_bmpSlider);
    m_bmpSlider = NULL;
  }

  return true;
}

//--------------------------------------------------------------------------------
bool CFItemList::ClearItems()
{
  m_iItemCount= 0;
  m_iItemPos= 0;
  m_iSelItem= -1;
  if (m_pSystem)
  {
    m_iSliderPos = m_bmpUpArrow->GetBottom();
  }

  // delete itemstructures  
  ITEMLISTENTRY *pTempEntry, *pOldEntry;
  pTempEntry= m_pEntries;
  while(pTempEntry)
  {
    pOldEntry= pTempEntry;
    pTempEntry= pTempEntry->pNext;
    SAFE_DELETE(pOldEntry->pszItem);
    SAFE_DELETE(pOldEntry->pszAddInfo);
    SAFE_DELETE(pOldEntry->pBmp);
    delete(pOldEntry);
  }
  m_pEntries= m_pLast= NULL;
  
  return true;
}

//--------------------------------------------------------------------------------
bool CFItemList::StylusDown(int xPos, int yPos)
{
  if (m_bmpUpArrow->PointInside(xPos, yPos)) {
    // move up in listbox
    if (m_iItemPos > 0) MoveUp();
    return(true);

  } else if (m_bmpDownArrow->PointInside(xPos, yPos)) {
    // move down in listbox
    if (m_iItemPos < m_iItemCount - m_iMaxVisItems) MoveDown();
    return(true);

  } else if ((xPos >= m_rScrollBar.left)&&(xPos <= m_rScrollBar.right)&&
             (yPos >= m_rScrollBar.top)&&(yPos <= m_rScrollBar.bottom)) {
    // if scrollbar was clicked
    if (yPos <= m_bmpSlider->GetY()) {
      MoveUp();
      return(true);
    } else if (yPos >= m_bmpSlider->GetBottom()) {
      MoveDown();
      return(true);
    } else if (m_bmpSlider->PointInside(xPos, yPos)) {
      m_iOldY= yPos;
      return(true);
    }
  
  } else if (m_bmpBack->PointInside(xPos, yPos)) {

    // one of the items was tapped
    int iItemPos= m_iItemPos + (yPos - m_bmpBack->GetY()) / m_iItemHeight - 1;
    ITEMLISTENTRY *pAuxEntry= m_pEntries;
    int i= 0;
  
    // get selected item
    while ((pAuxEntry)&&(i++ <= iItemPos)) pAuxEntry= pAuxEntry->pNext;
    m_iSelItem= --i;
    
    if (pAuxEntry) {
      // save entry in member variable to avoid alias effects with pointer variable!
      m_saveEntry.ulIndex= pAuxEntry->ulIndex;
      m_saveEntry.ulUser = pAuxEntry->ulUser;
      _tcscpy(m_saveEntry.pszItem, pAuxEntry->pszItem);
      if (pAuxEntry->pszAddInfo) _tcscpy(m_saveEntry.pszAddInfo, pAuxEntry->pszAddInfo);
      ItemSelected(&m_saveEntry);
    }
    return(true);
  }
  
  return(false); 
}

//--------------------------------------------------------------------------------
void CFItemList::ItemSelected(ITEMLISTENTRY *pEntry)
{
  Draw();
  m_pSystem->QueueEvent(WM_ITEMLISTSELECT, m_ulID, (void*)pEntry);
}

//--------------------------------------------------------------------------------
bool CFItemList::StylusMove(int xPos, int yPos)
{
 if ((xPos >= m_rScrollBar.left)&&(xPos <= m_rScrollBar.right)&&
     (yPos >= m_rScrollBar.top)&&(yPos <= m_rScrollBar.bottom)) {
    int iDiffY= yPos - m_iOldY;

    if (iDiffY >= m_iSliderStep) {
      m_iOldY= yPos;
      MoveDown();
      return(true);
    } else if (iDiffY <= m_iSliderStep * -1) {
      m_iOldY= yPos;
      MoveUp();
      return(true);
    }    
  }  
  return(false);
}

//--------------------------------------------------------------------------------
void CFItemList::Draw()
{
  int iSliderHeight = 0;

  DrawItems();

  m_bmpBack->Blit();
  m_bmpUpArrow->Blit();
  m_bmpDownArrow->Blit();

  // Check if we have to create slider new (when size has changed!)
  if (!m_bmpSlider)
  {
    // calculate height of slider
    if (m_iItemCount < m_iMaxVisItems)
    {
      iSliderHeight = m_iScrollBarHeight;
    }
    else
    {
      iSliderHeight = max(2, (m_iScrollBarHeight * m_iMaxVisItems) / m_iItemCount);
    }

    m_bmpSlider = new CFBitmap(*(m_pSystem->GetDoubleBuffer()));
    m_bmpSlider->Create(m_bmpUpArrow->GetWidth(), iSliderHeight);    
   
    m_bmpSlider->SetColor(m_colBorder);
    m_bmpSlider->SetBackgroundColor(m_colSlide);
    m_bmpSlider->DrawFilledRect(0, 0, m_bmpUpArrow->GetWidth(), iSliderHeight);
   
    m_bmpSlider->SetX(m_bmpUpArrow->GetX());
  }
  m_bmpSlider->SetY(m_iSliderPos);
  m_bmpSlider->SaveUnder();
  m_bmpSlider->Blit();
}

//--------------------------------------------------------------------------------
void CFItemList::DrawItems()
{
  BeforeDrawItems();

  // Clear background/items.
  m_bmpBack->SetColor(m_colBack);
  m_bmpBack->SetBackgroundColor(m_colBack);
  m_bmpBack->DrawFilledRect(1, 1, m_bmpBack->GetWidth() - m_bmpUpArrow->GetWidth() - 1, m_bmpBack->GetHeight() - 2);

  int iPicSpace = 0;
  // rectangle in which an item is displayed
  RECT itemRect;
  itemRect.top   = 1;
  itemRect.right = m_iItemWidth;
  itemRect.bottom= m_iItemHeight;

  // display list entries
  ITEMLISTENTRY *pAuxEntry = m_pEntries;
  int pos = 0;
  int index = 0;  
  
  while (pAuxEntry)
  {
    if (pos >= m_iItemPos && pos < (m_iItemPos + m_iMaxVisItems))
    {
      // Draw Icon/Image?
      if (m_isFileList || pAuxEntry->pBmp)
      {
        if (pAuxEntry->pBmp != NULL)
        {
          pAuxEntry->pBmp->SetDestBitmap(*m_bmpBack);
          pAuxEntry->pBmp->SetX(1);
          pAuxEntry->pBmp->SetY(1 + m_iItemHeight * index);
          pAuxEntry->pBmp->Blit();
        }
        iPicSpace = 17;
      }
      else
      {
        iPicSpace = 1;
      }
      itemRect.left = iPicSpace;

      COLORREF colBack = m_colBack;
      COLORREF colText = m_colText;
      bool normal = true;

      // Item selected?
      if (m_iSelItem != -1 && m_iSelItem - m_iItemPos == index)
      {
        colBack = m_colHiBack;
        colText = m_colHiText;
        normal = false;
      }

      m_bmpBack->SetColor(colBack);
      m_bmpBack->SetBackgroundColor(colBack);
      m_bmpBack->DrawFilledRect(itemRect);

      m_bmpBack->SetTextColor(colText);             
      RECT textRect = itemRect;
      textRect.left += 2;
      m_bmpBack->DrawText(pAuxEntry->pszItem, textRect);

      if (m_isFileList)
      {
        m_pSystem->DrawFileIcon(*m_bmpBack, pAuxEntry->pszAddInfo, 1, m_iItemHeight * index + 1, normal);
      }

      index++;
      itemRect.top += m_iItemHeight;
      itemRect.bottom += m_iItemHeight;
    }
    pos++;
    pAuxEntry= pAuxEntry->pNext;
  }
}

//--------------------------------------------------------------------------------
void CFItemList::UpdatePos(int offsetY /* = 0 */)
{
  if (m_pSystem)
  {
    m_bmpBack->SetX(m_iPosX);
    m_bmpBack->SetY(m_iPosY + offsetY);
    m_bmpUpArrow->SetX(m_bmpBack->GetRight() - m_bmpUpArrow->GetWidth());
    m_bmpUpArrow->SetY(m_bmpBack->GetY());
    m_bmpDownArrow->SetX(m_bmpUpArrow->GetX());
    m_bmpDownArrow->SetY(m_bmpBack->GetY() + m_bmpBack->GetHeight() -  m_bmpDownArrow->GetHeight());

    m_iSliderPos= m_bmpUpArrow->GetBottom();
    m_rScrollBar.left= m_bmpUpArrow->GetX();
    m_rScrollBar.top= m_bmpUpArrow->GetBottom();
    m_rScrollBar.right= m_bmpUpArrow->GetRight();
    m_rScrollBar.bottom= m_bmpDownArrow->GetY();
  }
}

//--------------------------------------------------------------------------------
void CFItemList::MoveUp()
{
  if (m_iItemPos > 0) {
    m_iItemPos--;
    // move slider 
    m_bmpSlider->RestoreUnder();
    if (m_iItemPos == 0) {
      m_iSliderPos= m_bmpUpArrow->GetBottom();
    } else {
      m_iSliderPos-= m_iSliderStep;
    }

    m_bmpSlider->SetY(m_iSliderPos);
    m_bmpSlider->SaveUnder();      
    m_bmpSlider->Blit();
    if ((m_iSelItem != -1)&&(m_iSelItem > (m_iItemPos + m_iMaxVisItems - 1))) {
      m_iSelItem= m_iItemPos + m_iMaxVisItems - 1;
    }
    Draw();
  }
}

//--------------------------------------------------------------------------------
void CFItemList::MoveDown()
{
  if (m_iItemPos < (m_iItemCount - m_iMaxVisItems)) {
    m_iItemPos++;
    // move slider
    m_bmpSlider->RestoreUnder();
    if (m_iItemPos == m_iItemCount - m_iMaxVisItems) {
      m_iSliderPos= m_bmpDownArrow->GetY() - m_bmpSlider->GetHeight();
    } else 
      m_iSliderPos+= m_iSliderStep;

    m_bmpSlider->SetY(m_iSliderPos);
    m_bmpSlider->SaveUnder();      
    m_bmpSlider->Blit();
    if ((m_iSelItem != -1)&&(m_iSelItem < m_iItemPos))
    {
      m_iSelItem = m_iItemPos;
    }
    Draw();
  }
}

//--------------------------------------------------------------------------------
/// If color-settings have changed the background is adapted in this method.
bool CFItemList::CreateBackGround()
{
  // Safety check if bitmap is valid.
  if (m_bmpBack->IsValid())
  {
    m_bmpBack->SetBackgroundColor(m_colBack);
    m_bmpBack->SetColor(m_colBorder);
    // Draw the main rectangle.
    m_bmpBack->DrawFilledRect(0, 0, m_bmpBack->GetWidth(), m_bmpBack->GetHeight());
    // Draw rectangle for scrollbar.
    m_bmpBack->DrawFilledRect(m_bmpBack->GetWidth() - m_bmpUpArrow->GetWidth(), 0, m_bmpUpArrow->GetWidth(), m_bmpBack->GetHeight());
    return true;
  }
  return false;
}

//--------------------------------------------------------------------------------
void CFItemList::SetBackColor(COLORREF colBack)
{
  m_colBack = colBack;
  CreateBackGround();
}

//--------------------------------------------------------------------------------
void CFItemList::SetBorderColor(COLORREF colBorder)
{
  m_colBorder = colBorder;
  CreateBackGround();
}

//--------------------------------------------------------------------------------
void CFItemList::SetSliderColor(COLORREF colSlide)
{
  m_colText = colSlide;
  CreateBackGround();
}

//--------------------------------------------------------------------------------
void CFItemList::SetBackHighlightColor(COLORREF colHiBack)
{
  m_colHiBack = colHiBack; 
  CreateBackGround();
}

