// FSlider.cpp: implementation of the CFSlider class.
//////////////////////////////////////////////////////////////////////
#include "FSlider.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"


//--------------------------------------------------------------------------------
/// Background graphics decides wether slider is vertical or horizontal.
CFSlider::CFSlider(CFBitmap *bmpBack, CFBitmap *bmpSlide) : IFateComponent()
{
  m_pszClassName= TEXT("CFSlider");

  // vertical or horizontal?
  m_bVer= bmpBack->GetHeight() > bmpBack->GetWidth();
  m_bmpBack  = bmpBack;
  m_bmpSlide = bmpSlide;
  
  if (m_bVer)
  {  // vertical
    m_iHeight  = m_bmpBack->GetHeight();
    m_iWidth   = _max(m_bmpBack->GetWidth(), m_bmpSlide->GetWidth());
    m_iSlideMid= m_bmpSlide->GetHeight() / 2;
    m_iMaxValue= m_bmpBack->GetHeight() + 2 * m_iSlideMid - 1;
  
  }
  else
  {  // horizontal
    m_iHeight  = _max(m_bmpBack->GetHeight(), m_bmpSlide->GetHeight());
    m_iWidth   = m_bmpBack->GetWidth();
    m_iSlideMid= m_bmpSlide->GetWidth() / 2;
    m_iMaxValue= m_bmpBack->GetWidth() + 2 * m_iSlideMid - 1;
  }
  m_iValue   = 0;
  m_bValSet  = false;
}

//--------------------------------------------------------------------------------
CFSlider::~CFSlider()
{
  delete(m_bmpBack);
  delete(m_bmpSlide);
}

//--------------------------------------------------------------------------------
void CFSlider::Draw()
{
  m_bmpBack->Blit();
  m_bmpSlide->SaveUnder();
  m_bmpSlide->Blit();
}

//--------------------------------------------------------------------------------
bool CFSlider::StylusDown(int xPos, int yPos)
{
  if (m_bmpBack->PointInside(xPos, yPos)||(m_bmpSlide->PointInside(xPos, yPos))) {
    m_bValSet= false;  // set flag that value was changed by hand

    if (!m_bmpSlide->PointInside(xPos, yPos)) {
      m_bmpSlide->RestoreUnder();
      
      // move slider under stylus
      if (m_bVer) {  // vertical
        int iPosY;
        int iHeight= m_bmpSlide->GetHeight();

        iPosY= yPos - iHeight / 2;
        // correction needed?
        if (iPosY < m_iTop) iPosY= m_iTop;
        else if (iPosY + iHeight > m_iBottom) iPosY= m_iBottom - iHeight;
      
        m_bmpSlide->SetY(iPosY);
      
      } else {  // horizontal
        int iPosX;
        int iWidth= m_bmpSlide->GetWidth();

        iPosX= xPos - iWidth / 2;
        // correction needed?
        if (iPosX < m_iBottom) 
          iPosX= m_iBottom;
        else if (iPosX + iWidth > m_iTop) 
          iPosX= m_iTop - iWidth;

        m_bmpSlide->SetX(iPosX);
      }      
      
      m_bmpSlide->SaveUnder();
      m_bmpSlide->Blit();
      m_app->DrawDoubleBuffer();
    }
    
    m_app->StartDragMode(m_bmpSlide, xPos, yPos);
    if (m_bVer) {  // vertical
      m_iDrag= xPos;     
    } else {  // horizontal
      m_iDrag= yPos; 
    }
    
    return(true);
  }
  
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFSlider::StylusMove(int xPos, int yPos)
{
  CFateApp::DRAGGINGINFO *pDI= NULL;

  if (m_app->IsDragMode()) {
    pDI= m_app->GetDragInfo();
    if (pDI->pBmpDrag != m_bmpSlide) return(false);

    // vertical?
    if ((m_bVer)&&(yPos - pDI->iOffsY >= m_iTop)&&(yPos - pDI->iOffsY + m_bmpSlide->GetHeight() <= m_iBottom))
    {
      m_app->DoDrag(m_iDrag, yPos);
      m_pSystem->QueueEvent(FATE_EVENT_ID_SLIDERCHANGE, m_ulID, reinterpret_cast<void*>(GetValue()));
    
    }
    else if ((xPos - pDI->iOffsX >= m_iBottom)&&(xPos - pDI->iOffsX + m_bmpSlide->GetWidth() <= m_iTop))
    {
      m_app->DoDrag(xPos, m_iDrag);
      m_pSystem->QueueEvent(FATE_EVENT_ID_SLIDERCHANGE, m_ulID, reinterpret_cast<void*>(GetValue()));
    }
    return true;
  }
  
  return false;  // event not handled
}

//--------------------------------------------------------------------------------
bool CFSlider::StylusUp(int xPos, int yPos)
{
  CFateApp::DRAGGINGINFO *pDI= NULL;

  if (m_app->IsDragMode()) {
    pDI= m_app->GetDragInfo();
    if (pDI->pBmpDrag != m_bmpSlide) return(false);

    // drag is over anyway
    m_app->StopDragMode();
    m_pSystem->QueueEvent(FATE_EVENT_ID_SLIDERCHANGE, m_ulID, reinterpret_cast<void*>(GetValue()));
    m_bmpSlide->Blit();
    return(true);
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
void CFSlider::SetX(int iPosX)
{
  m_iPosX= iPosX;

  m_bmpBack->SetX(iPosX);
  if (m_bVer) {
    // center the slide
    m_bmpSlide->SetX(iPosX + m_bmpBack->GetWidth() / 2 - m_bmpSlide->GetWidth() / 2);
  } else {
    m_bmpSlide->SetX(m_bmpBack->GetX() - m_iSlideMid + m_iValue);
    // save upper and lower level
    m_iBottom  = m_bmpBack->GetX() - m_iSlideMid;
    m_iTop     = m_bmpBack->GetRight() + m_iSlideMid;
  }
}

//--------------------------------------------------------------------------------
void CFSlider::SetY(int iPosY)
{
  m_iPosY= iPosY;
  m_bmpBack->SetY(iPosY);
  
  if (m_bVer) {
    m_bmpSlide->SetY(m_bmpBack->GetY() - m_iSlideMid + m_iValue);
    // save upper and lower level
    m_iTop     = m_bmpBack->GetY() - m_iSlideMid;
    m_iBottom  = m_bmpBack->GetBottom() + m_iSlideMid;
  
  } else {
    // center the slide
    m_bmpSlide->SetY(iPosY + m_bmpBack->GetHeight() / 2 - m_bmpSlide->GetHeight() / 2);
  }  
}

//--------------------------------------------------------------------------------
/// Returns a value between 0 and iMaxValue
int CFSlider::GetValue()
{
  // if last value was set by method call, return it
  if (m_bValSet) {
    return(m_iValue);
  } else {
    if (m_bVer) // vertical
      return((m_bmpSlide->GetY() + m_iSlideMid - m_bmpBack->GetY()) * m_iMaxValue / m_iHeight); 
    else
      return((m_bmpSlide->GetX() + m_iSlideMid - m_bmpBack->GetX()) * m_iMaxValue / m_iWidth);
  }
}
  
//--------------------------------------------------------------------------------
/// If iValue is between 0 and m_iMaxValue it set accordingly and the return value
/// is true. 
/// Otherwise, the call fails and the return value is false.
bool CFSlider::SetValue(int iValue)
{
  if ((iValue >= 0)&&(m_iValue <= m_iMaxValue)) {
    m_bValSet= true;  // set flag that value was set by method call
    
    if (m_bVer) {  // vertical
      m_iValue= m_iHeight * iValue / m_iMaxValue;
      m_bmpSlide->SetY(m_bmpBack->GetY() - m_iSlideMid + m_iValue);
    } else {  // horizontal
      m_iValue= m_iWidth * iValue / m_iMaxValue;
      m_bmpSlide->SetX(m_bmpBack->GetX() - m_iSlideMid + m_iValue);
    }
    
    if (m_pSystem)
    {
      Draw();    
    }
    return(true);
  }
  return(false);
}

