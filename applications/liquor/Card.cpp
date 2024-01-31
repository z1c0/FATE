#include "Card.h"
#include "Game.h"


//--------------------------------------------------------------------------------
CCard::CCard(CGame* pGame, CardColor col, CardType type) 
{
  m_pGame = pGame;
  m_state = StateNone;
  m_col = col;
  m_type = type;
}

//--------------------------------------------------------------------------------
CCard::~CCard()
{
}

//--------------------------------------------------------------------------------
void CCard::Draw()
{
  m_bmp.SaveUnder();
  m_bmp.Blit();
}

//--------------------------------------------------------------------------------
void CCard::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
  CFBitmap *pBmp = m_pSystem->GetDoubleBuffer();
  m_bmp.SetDestBitmap(*pBmp);

  m_bmp.Create(CARD_WIDTH, CARD_HEIGHT);
  COLORREF col;
  switch (m_col) {
    case ColH:
      col = RGB(200, 0, 0);
      break;
    case ColP:
      col = RGB(200, 200, 0);
      break;
    case ColS:
      col = RGB(0, 200, 200);
      break;
    case ColE:
      col = RGB(200, 0, 200);
      break;
  }
  m_bmp.SolidFill(col);
  m_bmp.SetBackgroundColor(col);

  RECT rect;
  rect.left = 1;
  rect.top = 1;
  rect.right = CARD_WIDTH;
  rect.bottom = CARD_HEIGHT;
  switch (m_type) {
    case TypeUnder:
      m_bmp.DrawText("Unter", rect);
      break;
    case TypeUpper:
      m_bmp.DrawText("Ober", rect);
      break;
    case TypeKing:
      m_bmp.DrawText("König", rect);
      break;
    case TypeTen:
      m_bmp.DrawText("10", rect);
      break;
    case TypeAce:
      m_bmp.DrawText("As", rect);
      break;
  }
}  

//--------------------------------------------------------------------------------
bool CCard::StylusDown(int xPos, int yPos)
{
  bool ret = false;
  if (m_bmp.PointInside(xPos, yPos)) {    
    m_app->StartDragMode(&m_bmp, xPos, yPos);
    ret = true;
  }
  return ret;
}

//--------------------------------------------------------------------------------
bool CCard::StylusMove(int xPos, int yPos)
{
  bool ret = false;

  if (m_app->IsDragMode()) {
    m_app->DoDrag(xPos, yPos);
  }
  return ret;
}

//--------------------------------------------------------------------------------
bool CCard::StylusUp(int xPos, int yPos)
{
  bool ret = false;
  if (m_app->IsDragMode()) {
    m_app->StopDragMode();
    if (m_pGame->IsCardInTarget(*this)) {
      m_state = StateDealt1;      
    }
    m_app->GetSystem()->ForceRedraw();
  }
  return ret;
}