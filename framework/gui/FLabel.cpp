#include "../base/FateTypeDefs.h"
#include "../base/FBitmap.h"
#include "FLabel.h"



//-------------------------------------------------------------------------------- 
CFLabel::CFLabel(LPCTSTR pText /* = NULL */)
{
  m_iWidth = 0;
  m_iHeight= 0;  
  m_pszText= NULL;
  SetText(pText);
  Init();
}

//-------------------------------------------------------------------------------- 
CFLabel::CFLabel(int iWidth, int iHeight)
{
  m_pszText= NULL;
  m_iWidth = iWidth;
  m_iHeight= iHeight;

  Init();
}

//-------------------------------------------------------------------------------- 
CFLabel::CFLabel(LPCTSTR pText, int iWidth, int iHeight)
{
  m_pszText= NULL;
  if (pText) SetText(pText);

  m_iWidth = iWidth;
  m_iHeight= iHeight;

  Init();
}

//-------------------------------------------------------------------------------- 
CFLabel::~CFLabel()
{
  // safe-release text string
  SAFE_DELETE(m_pszText);
  SAFE_DELETE(m_pBmpImg);
}

//-------------------------------------------------------------------------------- 
void CFLabel::Init()
{
  m_pBmpImg= NULL;

  m_colBack  = RGB(255, 255, 255);
  m_colBorder= RGB(0, 0, 0);
  m_colText  = RGB(0, 0, 0);
}

//-------------------------------------------------------------------------------- 
void CFLabel::Draw()
{
  if (m_pBmpImg) m_pBmpImg->Blit();
}

//-------------------------------------------------------------------------------- 
void CFLabel::SetSystem(CFSystem *pSystem)
{
  if (!m_pSystem) {
    m_pSystem= pSystem;
    CreateBufferBitmap();
    RedrawBufferBitmap();
  }
}

//-------------------------------------------------------------------------------- 
void CFLabel::SetText(LPCTSTR pText)
{
  // safe-release text string
  SAFE_DELETE(m_pszText);

  if (pText) {
    m_pszText= new TCHAR[_tcslen(pText) + 1];
    _tcscpy(m_pszText, pText);
  }
  
  if (m_pSystem) {
    RedrawBufferBitmap();
    m_pSystem->ForceRedraw();
  }
}

//-------------------------------------------------------------------------------- 
bool CFLabel::CreateBufferBitmap()
{
  m_rectText.left  = 0;
  m_rectText.top   = 0;
  m_rectText.right = 0;
  m_rectText.bottom= 0;

  if ((!m_iWidth)||(!m_iHeight)) {
    // at least text must be specified
    if (!m_pszText) return(false);

    CFBitmap::CalcRectForText(m_pszText, m_rectText);
    m_iWidth = m_rectText.right - m_rectText.left + 4;
    m_iHeight= m_rectText.bottom - m_rectText.top + 4;
  }   
  
  m_rectText.top= 2; m_rectText.left= 2;
  m_rectText.bottom= m_iHeight - 2; 
  m_rectText.right= m_iWidth - 2;

  // safe-release old bitmap
  SAFE_DELETE(m_pBmpImg);
  CFBitmap& bmp = m_pSystem->GetDoubleBuffer();
  m_pBmpImg = new CFBitmap(bmp);
  if (!m_pBmpImg->Create(m_iWidth, m_iHeight)) 
  {
    return(false);
  }

  m_pBmpImg->SetX(m_iPosX);
  m_pBmpImg->SetY(m_iPosY);

  return(true);
}

//-------------------------------------------------------------------------------- 
bool CFLabel::RedrawBufferBitmap()
{
  if (m_pBmpImg) {
    // draw background
    m_pBmpImg->SetBackgroundColor(m_colBack);
    m_pBmpImg->SetColor(m_colBorder);
    m_pBmpImg->DrawFilledRect(0, 0, m_iWidth, m_iHeight);
  
    // draw text
    if (m_pszText) {
      m_pBmpImg->SetColor(m_colText);
      m_pBmpImg->DrawText(m_pszText, m_rectText);
    }
    return(true);
  }
  
  return(false);
}

//-------------------------------------------------------------------------------- 
void CFLabel::SetX(int iPosX)
{
  m_iPosX= iPosX;
  if (m_pBmpImg) m_pBmpImg->SetX(iPosX);
}

//-------------------------------------------------------------------------------- 
void CFLabel::SetY(int iPosY)
{
  m_iPosY= iPosY;
  if (m_pBmpImg) m_pBmpImg->SetY(iPosY);
}

//-------------------------------------------------------------------------------- 
void CFLabel::SetColBack(COLORREF col)
{
  m_colBack= col;
  if (m_pBmpImg) {
    RedrawBufferBitmap();
  }
}
 
//--------------------------------------------------------------------------------
void CFLabel::SetColText(COLORREF col)
{
  m_colText= col;
  if (m_pBmpImg) {
    RedrawBufferBitmap();
  }
}

//--------------------------------------------------------------------------------
void CFLabel::SetColBorder(COLORREF col)
{
  m_colBorder= col;
  if (m_pBmpImg) {
    RedrawBufferBitmap();
  }
}
