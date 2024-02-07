#include "FToggleButton.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"

//--------------------------------------------------------------------------------
CFToggleButton::CFToggleButton(CFBitmap *bmpState1, CFBitmap *bmpState2) : IFateComponent()
{
  m_pszClassName= TEXT("CFToggleButton");
  m_bmpState1   = bmpState1;
  m_bmpState2   = bmpState2;
  m_iWidth      = bmpState1->GetWidth();
  m_iHeight     = bmpState1->GetHeight();
  m_bState1     = true;
  m_bTrans      = false;
}

//--------------------------------------------------------------------------------
CFToggleButton::~CFToggleButton()
{
  delete(m_bmpState1);
  delete(m_bmpState2);
}

//--------------------------------------------------------------------------------
void CFToggleButton::Draw()
{
  if (m_bState1) {
    m_bmpState1->SaveUnder();
    // transparency set?
    if (m_bTrans) {
      m_bmpState1->TransBlit(m_colTrans);
    } else {
      m_bmpState1->Blit();
    }   
  } else {
    m_bmpState2->SaveUnder();
    // transparency set?
    if (m_bTrans) {
      m_bmpState2->TransBlit(m_colTrans);
    } else {
      m_bmpState2->Blit();
    }   
  }
}

//--------------------------------------------------------------------------------
bool CFToggleButton::StylusDown(int xPos, int yPos)
{
  if (m_bmpState1->PointInside(xPos, yPos)) {
    if (m_bState1) {
      m_bmpState1->RestoreUnder();
    } else {
      m_bmpState2->RestoreUnder();
    }
    m_bState1= !m_bState1;
    Draw();
    m_pSystem->QueueEvent(WM_BUTTONPRESS, m_ulID, NULL);
    
    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFToggleButton::StylusMove(int xPos, int yPos)
{
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFToggleButton::StylusUp(int xPos, int yPos)
{
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
void CFToggleButton::SetX(int iPosX)
{
  m_iPosX= iPosX;
  m_bmpState1->SetX(m_iPosX);
  m_bmpState2->SetX(m_iPosX);
}

//--------------------------------------------------------------------------------
void CFToggleButton::SetY(int iPosY)
{
  m_iPosY= iPosY;
  m_bmpState1->SetY(m_iPosY);
  m_bmpState2->SetY(m_iPosY);
}

//--------------------------------------------------------------------------------
/// Possible values are 0 and 1.
void CFToggleButton::SetState(int iState) 
{
  if ((!m_bState1)&&(iState == 0))
  {
    m_bState1= true;
    if (m_pSystem)
    {
      m_bmpState2->RestoreUnder();
      Draw();
    }
  
  }
  else if ((m_bState1)&&(iState == 1))
  {
    m_bState1= false;
    if (m_pSystem)
    {
      m_bmpState1->RestoreUnder();
      Draw();
    }  
  }
}