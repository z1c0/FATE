#include "Game.h"

#define COL_GREEN_BACK  RGB(70, 135, 60)

//--------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CGame());
}


//--------------------------------------------------------------------------------
CGame::CGame() : CFateApp()
{
  m_allcards[ 0] = new CCard(this, ColH, TypeUnder);
  m_allcards[ 1] = new CCard(this, ColH, TypeUpper);
  m_allcards[ 2] = new CCard(this, ColH, TypeKing);
  m_allcards[ 3] = new CCard(this, ColH, TypeTen);
  m_allcards[ 4] = new CCard(this, ColH, TypeAce);
  m_allcards[ 5] = new CCard(this, ColP, TypeUnder);
  m_allcards[ 6] = new CCard(this, ColP, TypeUpper);
  m_allcards[ 7] = new CCard(this, ColP, TypeKing);  
  m_allcards[ 8] = new CCard(this, ColP, TypeTen);
  m_allcards[ 9] = new CCard(this, ColP, TypeAce);  
  m_allcards[10] = new CCard(this, ColS, TypeUnder);
  m_allcards[11] = new CCard(this, ColS, TypeUpper);
  m_allcards[12] = new CCard(this, ColS, TypeKing);
  m_allcards[13] = new CCard(this, ColS, TypeTen);
  m_allcards[14] = new CCard(this, ColS, TypeAce);
  m_allcards[15] = new CCard(this, ColE, TypeUnder);
  m_allcards[16] = new CCard(this, ColE, TypeUpper);
  m_allcards[17] = new CCard(this, ColE, TypeKing);
  m_allcards[18] = new CCard(this, ColE, TypeTen);
  m_allcards[19] = new CCard(this, ColE, TypeAce);
}

//--------------------------------------------------------------------------------
CGame::~CGame()
{
  for (int i = 0; i < NR_OF_CARDS; i++) {
    delete m_allcards[i];
  }
}

//--------------------------------------------------------------------------------
void CGame::Draw()
{
  ClearScreen(COL_GREEN_BACK);

  int left1 = 10;
  int top1 = 70;
  int left2 = 10;
  int top2 = 200;

  for (int i = 0; i < NR_OF_CARDS; i++) {
    CCard *card = m_allcards[i];
    if (card->GetCardState() == StateHand1) {
      card->SetX(left1);
      card->SetY(top1);

      left1 += (card->GetWidth() + 10);
    }
    else if (card->GetCardState() == StateHand2) {
      card->SetX(left2);
      card->SetY(top2);

      left2 += (card->GetWidth() + 10);
    }
  }


  left1 = 10;
  top1 = 130;
  m_rectcardtarget.left = left1 + (CARD_WIDTH  + 10) * 2;
  m_rectcardtarget.top = top1;
  m_rectcardtarget.right = left1 + (CARD_WIDTH + 10) * 3;
  m_rectcardtarget.bottom = top1 + CARD_HEIGHT;
  m_pDblBuffer->SetColor(RGB(0,0,0));
  m_pDblBuffer->SetBackgroundColor(COL_GREEN_BACK);
  m_pDblBuffer->DrawFilledRect(m_rectcardtarget);
}

//--------------------------------------------------------------------------------
bool CGame::InitFateApp()
{
  for (int i = 0; i < NR_OF_CARDS; i++) {
    Add(*m_allcards[i]);
  }

  m_btndeal.SetCaption("Neu");
  m_btndeal.SetId(ID_BTN_NEW);
  Add(m_btndeal);

  DealCards();

  return true;
}

//--------------------------------------------------------------------------------
bool CGame::StylusDown(int xPos, int yPos)
{
  return false;
}

//--------------------------------------------------------------------------------
bool CGame::ButtonReleased(DWORD dwBtnID)
{
  if (dwBtnID == ID_BTN_NEW) {
    DealCards();
    return true;
  }

  return false;
}

//--------------------------------------------------------------------------------
void CGame::DealCards()
{
  for (int i = 0; i < NR_OF_CARDS; i++) {
    m_allcards[i]->SetVisible(false);
    m_allcards[i]->SetCardState(StateNone);
  }

  int count = 0;
  while (count < 5) {
    int index = CFSystem::GetRandomNumber(20);
    if (m_allcards[index]->GetCardState() == StateNone) {
      m_allcards[index]->SetCardState(StateHand1);
      m_allcards[index]->SetVisible(true);
      count++;
    }
  }
  count = 0;
  while (count < 5) {
    int index = CFSystem::GetRandomNumber(20);
    if (m_allcards[index]->GetCardState() == StateNone) {
      m_allcards[index]->SetCardState(StateHand2);
      m_allcards[index]->SetVisible(true);
      count++;
    }
  }
}

//--------------------------------------------------------------------------------
bool CGame::IsCardInTarget(CCard& card)
{
  CFRect& rect1 = card.GetBounds();
  return rect1.Intersects(m_rectcardtarget);
}
