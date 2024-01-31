#ifndef __GAME__H__
#define __GAME__H__

#include "../../framework/include/fate.h"
#include "Card.h"

//--------------------------------------------------------------------------------

#define ID_BTN_NEW 100

//--------------------------------------------------------------------------------

class CGame : public CFateApp  
{

public:
	CGame();
	virtual ~CGame();

  virtual bool InitFateApp();
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual void Draw();

  bool IsCardInTarget(CCard& card);

private:

  void DealCards();

  CCard *m_allcards[NR_OF_CARDS];
  CFButton m_btndeal;
  RECT m_rectcardtarget;
};

#endif // __GAME__H__
