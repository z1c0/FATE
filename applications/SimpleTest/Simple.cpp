#include "Simple.h"

//------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CSimple());
}


//------------------------------------------------------------------------------
CSimple::CSimple()
{
  m_pBtnQuit = NULL;
  m_pBtnAction = NULL;
}

//------------------------------------------------------------------------------
CSimple::~CSimple()
{
  if (m_pBtnQuit) delete(m_pBtnQuit);
  if (m_pBtnAction) delete(m_pBtnAction);
}

//------------------------------------------------------------------------------
bool CSimple::InitFateApp()
{
  ClearScreen(RGB(58, 110, 165));
  
  m_pBtnQuit= new CFButton("Exit");
  m_pBtnQuit->SetId(1234);
  Add(*m_pBtnQuit);

  m_pBtnAction= new CFButton("Action");
  m_pBtnAction->SetId(2345);
  m_pBtnAction->SetY(m_pBtnQuit->GetBottom() + 20);
  Add(*m_pBtnAction);

  m_pDblBuffer->SetBackgroundColor(RGB(255, 0, 0));

  return true;
}

//------------------------------------------------------------------------------
void CSimple::Draw()
{
  ClearScreen(RGB(58, 110, 165));
}

//------------------------------------------------------------------------------
bool CSimple::StylusDown(int xPos, int yPos)
{
  const int iSize= 4;
  m_pDblBuffer->DrawFilledRect(xPos, yPos, iSize, iSize);
  return false;
}

//------------------------------------------------------------------------------
bool CSimple::StylusMove(int xPos, int yPos)
{
  const int iSize= 4;
  m_pDblBuffer->DrawFilledRect(xPos, yPos, iSize, iSize);
  return false;
}

//------------------------------------------------------------------------------
bool CSimple::ButtonReleased(unsigned long ulBtnID)
{
  if (ulBtnID == 1234)
    Exit();
  else {  
  }
  return true;
}

//------------------------------------------------------------------------------
bool CSimple::KeyDown(PdaKey key)
{
  char szMsg[32];
  _stprintf(szMsg, "Key: %d", key);
  Message(szMsg);

  return(true);
}