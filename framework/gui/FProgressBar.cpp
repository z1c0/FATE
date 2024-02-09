// FProgressBar.cpp: implementation of the CFProgressBar class.
//////////////////////////////////////////////////////////////////////
#include "FProgressBar.h"

//--------------------------------------------------------------------------------
CFProgressBar::CFProgressBar(int iWidth, int iHeight) : IFateComponent()
{
  m_bmpBuff= NULL;
  m_iWidth = iWidth;
  m_iHeight= iHeight;
  m_iBorderWidth= 2;
  m_iProgPerc= m_iProgGraph= 0;
  
  // init default color-settings
  m_colBack  = RGB(255, 255, 255);
  m_colBorder= RGB(125, 125, 125);
  m_colFront = RGB(255, 0, 0);
}

//--------------------------------------------------------------------------------
CFProgressBar::~CFProgressBar()
{
  if (m_bmpBuff) delete(m_bmpBuff);
}

//--------------------------------------------------------------------------------
/// Draw the buffered image of the progress bar.
void CFProgressBar::Draw()
{
  m_bmpBuff->Blit();
}

//--------------------------------------------------------------------------------
/// Draws the offscreen bitmap for the ProgessBar-Control with the current color
/// settings and progess value.
void CFProgressBar::DrawOffScreen()
{
  if (!m_pSystem) return;  
 
  // draw progess
  m_bmpBuff->SetColor(m_colBorder);
  m_bmpBuff->SetBackgroundColor(m_colFront);
  m_bmpBuff->DrawFilledRect(1, 1, m_iProgGraph - 1, m_iHeight - 1);

  // draw background
  m_bmpBuff->SetBackgroundColor(m_colBack);
  m_bmpBuff->DrawFilledRect(m_iProgGraph - 1, 1, m_iWidth - 1, m_iHeight - 1);
}

//-------------------------------------------------------------------------------- 
void CFProgressBar::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
  m_bmpBuff = new CFBitmap(*pSystem->GetDoubleBuffer());
  m_bmpBuff->Create(m_iWidth, m_iHeight);
  m_bmpBuff->SetX(m_iPosX);
  m_bmpBuff->SetY(m_iPosY);
  DrawOffScreen();
}

//--------------------------------------------------------------------------------
/// Sets the progess of the bar.
/// Parameter is specified in percent (must be between 0 and 100). Otherwhise
/// return value is false.
bool CFProgressBar::SetProgress(int iProgress)
{
  if ((iProgress < 0)||(iProgress > 100)) return(false);

  m_iProgPerc= iProgress;
  // calculate progess relative to size of control
  m_iProgGraph= iProgress * m_iWidth / 100;
  DrawOffScreen();
  Draw();

  return(true);
}