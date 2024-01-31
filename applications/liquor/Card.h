#ifndef __CARD__H__
#define __CARD__H__

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------

#define NR_OF_CARDS  20
#define CARD_WIDTH   35
#define CARD_HEIGHT  50

//--------------------------------------------------------------------------------

enum CardType
{
  TypeUnder,
  TypeUpper,
  TypeKing,
  TypeTen,
  TypeAce
};

//--------------------------------------------------------------------------------
enum CardColor
{
  ColH,
  ColP,
  ColS,
  ColE
};

//--------------------------------------------------------------------------------
enum CardState
{
  StateNone,
  StateFree,
  StateHand1,
  StateHand2,
  StateDealt1,
  StateDealt2,
  StateDown1,
  StateDown2
};

//--------------------------------------------------------------------------------

class CGame;

//--------------------------------------------------------------------------------

class CCard : public IFateComponent
{

public:

	CCard(CGame *pGame, CardColor col, CardType type);
	virtual ~CCard();

  virtual void SetSystem(CFSystem *pSystem);

  virtual void SetX(int iPosX) { m_iPosX = iPosX; m_bmp.SetX(iPosX); };
  virtual void SetY(int iPosY) { m_iPosY = iPosY; m_bmp.SetY(iPosY); };

  virtual int GetWidth() { return m_bmp.GetWidth(); };
  virtual int GetHeight() { return m_bmp.GetHeight(); };

  virtual bool StylusDown(int xPos, int yPos); 
  virtual bool StylusMove(int xPos, int yPos);
  virtual bool StylusUp(int xPos, int yPos);

  CardState GetCardState() { return m_state; };
  void SetCardState(CardState state) { m_state = state; };

  CFRect& GetBounds()
  {
    m_rect.SetLeft(m_bmp.GetX());
    m_rect.SetTop(m_bmp.GetY());
    m_rect.SetWidth(m_bmp.GetWidth());
    m_rect.SetHeight(m_bmp.GetHeight());
    return m_rect;
  }

private:

  virtual void Draw();

  CGame *m_pGame;

  CardState m_state;
  CardColor m_col;
  CardType m_type;
  CFBitmap m_bmp;
  CFRect m_rect;

};

#endif // __CARD__H__
