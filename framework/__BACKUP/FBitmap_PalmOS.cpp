// FBitmap_PalmOS.cpp: implementation of the CFBitmap class.
//////////////////////////////////////////////////////////////////////
#include "FateApp.h"
#include "FBitmap_PalmOS.h"

//--------------------------------------------------------------------------------
CFBitmap::CFBitmap()
{
  Init();
  m_hWinDst= WinGetDrawWindow();
}

//--------------------------------------------------------------------------------
CFBitmap::CFBitmap(CFBitmap *pBmp)
{
  Init();
  m_hWinDst= pBmp->m_hWinSrc;
}

//--------------------------------------------------------------------------------
CFBitmap::~CFBitmap()
{
  CleanUp();
}

//------------------------------------------------------------------------------
void CFBitmap::CleanUp()
{
  if (m_pBmp) 
    BmpDelete(m_pBmp);
  if (m_hWinSrc)
    WinDeleteWindow(m_hWinSrc, false);
}


//--------------------------------------------------------------------------------
void CFBitmap::Init()
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
void CFBitmap::SetDestBitmap(CFBitmap *pBmp)
{
  m_hWinDst= pBmp->m_hWinSrc;
}


//--------------------------------------------------------------------------------
bool CFBitmap::Create(int iWidth, int iHeight)
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
bool CFBitmap::SolidFill(const COLORREF colFill)
{
  UInt16 *pPixel= (UInt16*)m_pBits;
  for (unsigned int i=0; i<m_iWidth*m_iHeight; i++) {    
    *pPixel++= colFill;
  }
  return(true);
}

//--------------------------------------------------------------------------------
bool CFBitmap::Blit()
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
bool CFBitmap::Blit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmap::ClipBlit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmap::StretchBlit(int iWidth, int iHeight)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmap::TransBlit(COLORREF colTrans)
{
  //TODO
  return(false);
}

//--------------------------------------------------------------------------------
bool CFBitmap::SaveToFile(const TCHAR *pszFileName)
{
  //TODO
  return(false);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetColor(COLORREF col)
{
  COLORREF colRet= m_colForeground;

  m_colForeground= col;

  if (m_hWinSrc) 
  {
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinSrc);
    RGBColorType colNew, colOld;
    colNew.r= GetRValue(col);
    colNew.g= GetGValue(col);
    colNew.b= GetBValue(col);
    
    WinSetForeColorRGB(&colNew, &colOld);
    WinSetDrawWindow(hPrevWin);
  }

  return(colRet);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetBackgroundColor(COLORREF col)
{
  COLORREF colRet= m_colBackground;

  m_colBackground= col;

  if (m_hWinSrc) 
  {
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinSrc);
    RGBColorType colNew, colOld;
    colNew.r= GetRValue(col);
    colNew.g= GetGValue(col);
    colNew.b= GetBValue(col);
    WinSetBackColorRGB(&colNew, &colOld);
    WinSetDrawWindow(hPrevWin);
  }

  return(colRet);
}

//------------------------------------------------------------------------------
COLORREF CFBitmap::SetTextColor(COLORREF col)
{
  COLORREF colRet= m_colText;

  m_colText= col;

  if (m_hWinSrc) 
  {
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinSrc);
    
    RGBColorType colNew, colOld;
    colNew.r= GetRValue(m_colText);
    colNew.g= GetGValue(m_colText);
    colNew.b= GetBValue(m_colText);
    
    WinSetTextColorRGB(&colNew, &colOld);
    
    WinSetDrawWindow(hPrevWin);
  }

  return(colRet);
}
//------------------------------------------------------------------------------
bool CFBitmap::CalcRectForText(TCHAR *pszText, RECT *pRect)
{
  pRect->bottom= pRect->top + FntCharHeight();
  pRect->right= pRect->left + FntCharsWidth(pszText, StrLen(pszText));  
  
  return(true);
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(RECT *pRect)
{
  return(DrawFilledRect(pRect->left, pRect->top, pRect->right, pRect->bottom));
}

//------------------------------------------------------------------------------
bool CFBitmap::DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight)
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
bool CFBitmap::DrawText(TCHAR *pszText, RECT *pRect)
{
  if (m_hWinSrc) {
    WinHandle hPrevWin= WinSetDrawWindow(m_hWinSrc);
    WinDrawChars(pszText, StrLen(pszText), pRect->left, pRect->top);
    WinSetDrawWindow(hPrevWin);
    return(true);
  }
  return(false);
}