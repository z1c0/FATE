// FMarker.cpp: implementation of the CFMarker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FMarker.h"

//--------------------------------------------------------------------------------
CFMarker::CFMarker(CFWavePanel *parent)
{
  m_parent= parent;
  m_bmpImg= NULL;
  m_iMargin= 4;  // extra pixels for stylus sensivity
  m_bSet= FALSE;
}

//--------------------------------------------------------------------------------
CFMarker::~CFMarker()
{
  SAFE_RELEASE(m_bmpImg);
}

//--------------------------------------------------------------------------------
BOOL CFMarker::Create(CFBitmap *bmpImg)
{
  // create images representing the marker
  m_bmpImg = bmpImg;
  m_iWidth = bmpImg->GetWidth();
  m_iHeight= bmpImg->GetHeight();
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFMarker::PointInside(int iPosX, int iPosY)
{
 return((iPosX >= m_iPosX - m_iMargin)&&(iPosX <= m_iPosX + m_iWidth + m_iMargin)&&
        (iPosY >= m_iPosY)&&(iPosY < m_iPosY + m_iHeight));
}

//--------------------------------------------------------------------------------
// Puts the marker to specified position on screen. According to screen postion
// the values for the sample position and time are calculated.
void CFMarker::SetPosX(int iPosX)
{
  // set values and move
  m_iPosX= iPosX;
  m_bmpImg->SetX(m_iPosX);  

  // calculate block position
  m_dwBlockPos= m_parent->GetZoomBlockStart() + ((m_parent->GetZoomBlockCount() * m_iPosX) / m_parent->GetWidth());
  
  // check block position
  if (m_dwBlockPos > m_parent->GetBlockCount())
    m_dwBlockPos = m_parent->GetBlockCount();
  if (m_dwBlockPos < 0)
    m_dwBlockPos = 0;
}


//--------------------------------------------------------------------------------
void CFMarker::SetBlockPos(DWORD dwBlockPos)
{
  if (m_parent->GetZoomBlockCount()) { // make sure we are not dividing by zero
    // set block value
    m_dwBlockPos= dwBlockPos;

      // check block position
    if (m_dwBlockPos > m_parent->GetBlockCount())
      m_dwBlockPos = m_parent->GetBlockCount();
    if (m_dwBlockPos < 0)
      m_dwBlockPos = 0;

    // calculate screen position
    m_iPosX= (m_dwBlockPos - m_parent->GetZoomBlockStart()) * m_parent->GetWidth()  / m_parent->GetZoomBlockCount();
    m_bmpImg->SetX(m_iPosX);
  }
}

//--------------------------------------------------------------------------------
DWORD CFMarker::GetTimePos() 
{ 
  if (m_parent->GetWave()->GetBytesPerSec()) { // make sure we are not dividing by zero
    return (m_dwBlockPos * m_parent->GetWave()->GetBlockAlign()) / (m_parent->GetWave()->GetBytesPerSec()/1000);
  } else {
    return 0;
  }
};  

//--------------------------------------------------------------------------------
void  CFMarker::Set() 
{ 
  m_bSet= TRUE; 
  SetBlockPos(0); 
};
