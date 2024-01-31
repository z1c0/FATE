#include "SnakeGame.h"
#include "ids.h"

//------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return new CSnakeGame();
}


//------------------------------------------------------------------------------
CSnakeGame::CSnakeGame()
{
}

//------------------------------------------------------------------------------
CSnakeGame::~CSnakeGame()
{
}

//------------------------------------------------------------------------------
bool CSnakeGame::InitFateApp()
{
  ClearScreen(RGB(58, 110, 165));

  m_pDblBuffer->SetBackgroundColor(RGB(255, 0, 0));

  m_levelcont.SetX(0);
  m_levelcont.SetY(0);
  m_levelcont.SetWidth(GetWidth());
  m_levelcont.SetHeight(GetHeight() - 40);
  Add(m_levelcont);
  m_levelcont.Create();

  m_btnquit.SetCaption(TEXT("X"));
  m_btnquit.SetX(0);
  m_btnquit.SetY(m_levelcont.GetBottom());
  m_btnquit.SetId(ID_BTN_QUIT);
  Add(m_btnquit);

  m_levelcont.SetEditMode(true);

  return true;
}

//------------------------------------------------------------------------------
void CSnakeGame::Idle()
{
  m_levelcont.Animate();
}

//------------------------------------------------------------------------------
bool CSnakeGame::ButtonReleased(unsigned long ulBtnID)
{
  bool ret = false;
  switch (ulBtnID) {
  case ID_BTN_QUIT:
    Exit();
    ret = true;
    break;
  }

  return ret;
}