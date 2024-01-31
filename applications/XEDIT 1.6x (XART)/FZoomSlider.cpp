// FZoomSlider.cpp: implementation of the CFZoomSlider class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FZoomSlider.h"


//--------------------------------------------------------------------------------
CFZoomSlider::CFZoomSlider(CFWavePanel *parent, int iWindowWidth)
{
  m_parent= parent;
  m_iMargin= 2;  // extra pixels for stylus sensivity
  m_iWindowWidth = iWindowWidth;
  m_dwZoomBlockCount = 0;
}

//--------------------------------------------------------------------------------
CFZoomSlider::~CFZoomSlider()
{
  delete(m_bmpImg);
}

//--------------------------------------------------------------------------------
BOOL CFZoomSlider::Create(CFBitmap *bmpImg)
{
  // create images representing the marker
  m_bmpImg= bmpImg;
  m_iWidth= bmpImg->GetWidth();
  m_iHeight= bmpImg->GetHeight(); 
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFZoomSlider::PointInside(int iPosX, int iPosY)
{
 return((iPosX >= m_iPosX - m_iMargin)&&(iPosX <= m_iPosX + m_iWidth + m_iMargin)&&
        (iPosY >= m_iPosY)&&(iPosY < m_iPosY + m_iHeight));
}

//--------------------------------------------------------------------------------
// Puts the marker to specified position on screen. According to screen position
// the values for the sample position and time are calculated.
void CFZoomSlider::SetPosX(int iPosX)
{
  
  if (iPosX < 0) {
    m_iPosX= 0;
  } else if ((iPosX + m_iWidth) >= m_iWindowWidth) {
    m_iPosX= m_iWindowWidth - m_iWidth;
  } else {
    m_iPosX= iPosX;
  }
  m_bmpImg->SetX(m_iPosX);  

  //calculate samples
  m_dwZoomStartBlock = (m_iPosX *  m_dwBlockCount) /  m_iWindowWidth;
  m_dwZoomEndBlock = m_dwZoomStartBlock + m_dwZoomBlockCount ;
  if (m_dwZoomEndBlock > m_dwBlockCount)
    m_dwZoomEndBlock = m_dwBlockCount;
}

void CFZoomSlider::Reset(DWORD dwBlockCount)
{
  m_iPosX= 0;
  m_bmpImg->SetX(0);
  m_dwBlockCount = dwBlockCount;
  ChangeZoom(0,dwBlockCount);

  //m_iWaveOffset = 0;
}

void CFZoomSlider::ChangeZoom(DWORD dwZoomStart, DWORD dwZoomEnd)
{
  if ((dwZoomEnd - dwZoomStart) > 5) {
    m_dwZoomStartBlock = dwZoomStart;
    m_dwZoomEndBlock = dwZoomEnd;
    m_dwZoomBlockCount = dwZoomEnd - dwZoomStart;
    
    //calculate width
    if ((m_dwZoomBlockCount * m_iWindowWidth) / m_dwBlockCount > 1 ) {
      m_iWidth = (m_dwZoomBlockCount * m_iWindowWidth) / m_dwBlockCount;
      m_bmpImg->ClipBlit(m_iWidth, m_iHeight);
    }

    m_iPosX = (dwZoomStart * m_iWindowWidth) / m_dwBlockCount;//calculate PosX
    m_bmpImg->SetX(m_iPosX);
  }
}

void CFZoomSlider::ZoomIn(DWORD dwPos)
{
  DWORD dwZoomStep, dwZoomStart, dwZoomEnd;
  // set some variables
  dwZoomStep= (GetZoomBlockCount()) / 4;
  // set zoom borders
  if (dwPos < dwZoomStep ) {    
    dwZoomStart= 0; 
    dwZoomEnd= 2 * dwZoomStep;
  } else if ((dwPos + dwZoomStep) > m_dwBlockCount) {
    dwZoomEnd= m_dwBlockCount;
    dwZoomStart= m_dwBlockCount - 2 * dwZoomStep;
  } else {
    dwZoomStart= dwPos - dwZoomStep;  
    dwZoomEnd  = dwPos + dwZoomStep;
  }
  // resize the zoom-area bitmap
  ChangeZoom(dwZoomStart, dwZoomEnd);
  // redraw

}

void CFZoomSlider::ZoomOut(DWORD dwPos)
{
  DWORD dwZoomStep, dwZoomStart, dwZoomEnd;

  dwZoomStep= GetZoomBlockCount();
  m_dwBlockCount;
    // set zoom borders
  if ((dwPos <= dwZoomStep)&&(dwPos + dwZoomStep >= m_dwBlockCount)){
    dwZoomStart= 0; 
    dwZoomEnd= m_dwBlockCount;
  } else if (dwPos < dwZoomStep) {    
    dwZoomStart= 0; 
    dwZoomEnd= 2 * dwZoomStep;
  } else if (dwPos + dwZoomStep > m_dwBlockCount) {
    dwZoomEnd= m_dwBlockCount;
    dwZoomStart= m_dwBlockCount - 2 * dwZoomStep;
  } else {
    dwZoomStart= dwPos - dwZoomStep;  
    dwZoomEnd  = dwPos + dwZoomStep;
  }
  // resize the zoom-area bitmap
  ChangeZoom(dwZoomStart, dwZoomEnd);
}
