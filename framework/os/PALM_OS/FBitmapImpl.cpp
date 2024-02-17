#include "FBitmapImpl.h"

//--------------------------------------------------------------------------------
CFBitmapImpl::CFBitmapImpl()
{
  Init();
  m_hWinDst= WinGetDrawWindow();
}

//--------------------------------------------------------------------------------
CFBitmapImpl::~CFBitmapImpl()
{
  CleanUp();
}

//------------------------------------------------------------------------------
void CFBitmapImpl::CleanUp()
{
  if (m_pBmp) 
    BmpDelete(m_pBmp);
  if (m_hWinSrc)
    WinDeleteWindow(m_hWinSrc, false);
}

void CFBitmapImpl::InitFrom(const CFBitmapImpl& other)
{
}

//--------------------------------------------------------------------------------
void CFBitmapImpl::Init()
{
  m_pBmp   = NULL;
  m_hWinSrc= NULL;
  m_pBits  = NULL;
  m_iPosX  = 0;
  m_iPosY  = 0;
  m_iWidth = 0;
  m_iHeight= 0;

  m_iBorderWith= 1;
}

//------------------------------------------------------------------------------
void CFBitmapImpl::SetDestBitmap(const CFBitmapImpl& bmp)
{
  m_hWinDst = bmp.m_hWinSrc;
}


//--------------------------------------------------------------------------------
bool CFBitmapImpl::Create(int iWidth, int iHeight)
{
  Err err;
  UInt16 uiErr= 0;
  m_pBmp= BmpCreate(iWidth, iHeight, 16, NULL, &uiErr);
  if (!m_pBmp) {
    return(false);
  }

  m_hWinSrc= WinCreateBitmapWindow(m_pBmp, &err);
  if (!m_hWinSrc) {
    BmpDelete(m_pBmp);
    return(false);
  }

  // get a pointer to the bitmap's bits
  m_pBits= (char*)BmpGetBits(m_pBmp);
  
  m_iWidth = iWidth;
  m_iHeight= iHeight;

  return(true);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::SolidFill(const COLORREF colFill)
{
  UInt16 *pPixel= (UInt16*)m_pBits;
  for (unsigned int i=0; i<m_iWidth*m_iHeight; i++) {    
    *pPixel++= colFill;
  }
  return(true);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::Blit()
{
  if (m_hWinDst) {
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinDst);
    WinDrawBitmap(m_pBmp, m_iPosX, m_iPosY);
    WinSetDrawWindow(hPrevWin);
    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::Blit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::ClipBlit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::StretchBlit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::TransBlit(COLORREF colTrans)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmapImpl::SaveToFile(const TCHAR *pszFileName)
{
  //TODO
  return(false);
}

//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetColor(COLORREF col)
{
  COLORREF colRet= m_colForeground;

  m_colForeground= col;

  return(colRet);
}

//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetBackgroundColor(COLORREF col)
{
  COLORREF colRet= m_colBackground;

  m_colBackground= col;

  return(colRet);
}

//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetTextColor(COLORREF col)
{
  COLORREF colRet= m_colText;

  m_colText= col;

  return(colRet);
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::CalcRectForText(const TCHAR *pszText, RECT& rect)
{
  rect.bottom = rect.top + FntCharHeight();
  rect.right = rect.left + FntCharsWidth(pszText, StrLen(pszText));  
  return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight)
{
  if (m_hWinSrc) {
    RectangleType rect;

    rect.topLeft.x= iLeft + m_iBorderWith;
    rect.topLeft.y= iTop + m_iBorderWith;
    rect.extent.x= iWidth - iLeft - 2*m_iBorderWith;
    rect.extent.y= iHeight - iTop - 2*m_iBorderWith;
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinSrc);
    
    // Switch color (use background color for drawing).
    RGBColorType col, colOld;
    col.r= GetRValue(m_colBackground);
    col.g= GetGValue(m_colBackground);
    col.b= GetBValue(m_colBackground);    
    WinSetForeColorRGB(&col, &colOld);    
    
    // First draw background.
    WinDrawRectangle(&rect, 0);
    
    // Switch to foreground color again.
    WinSetForeColorRGB(&colOld, &col);    

    // Draw the frame.
    WinDrawRectangleFrame(simpleFrame, &rect);
    
    WinSetDrawWindow(hPrevWin);
    
    return(true);
  }
  return(false);
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawText(const TCHAR *pszText, RECT& rect)
{
  if (m_hWinSrc)
  {
    WinHandle hPrevWin = WinSetDrawWindow(m_hWinSrc);

    // set text color
    RGBColorType col;
    col.r = GetRValue(m_colText);
    col.g = GetGValue(m_colText);
    col.b = GetBValue(m_colText);    
    WinSetTextColorRGB(&col, NULL);    

    // set background color
    col.r = GetRValue(m_colBackground);
    col.g = GetGValue(m_colBackground);
    col.b = GetBValue(m_colBackground);
    WinSetBackColorRGB(&col, NULL);
   
    WinDrawChars(pszText, StrLen(pszText), rect.left, rect.top);

    WinSetDrawWindow(hPrevWin);

    return(true);
  }
  return(false);
}