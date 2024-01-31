// FButton.cpp: implementation of the CFButton class.
//////////////////////////////////////////////////////////////////////
#include "FateTypeDefs.h"
#include "FateApp.h"
#include "FButton.h"


//--------------------------------------------------------------------------------
/// Constructor for bitmap button.
CFButton::CFButton(CFBitmap *pBmpDefault, CFBitmap *pBmpPress /*= NULL */, CFBitmap *pBmpDisabled /* = NULL */)
{
  m_pszClassName= TEXT("CFButton");
  m_iWidth= pBmpDefault->GetWidth();
  m_ulDelay= 1;
  m_iHeight= pBmpDefault->GetHeight();
  m_bTrans= false;
  m_bBmpButton= true;
  m_pBmpDefault= pBmpDefault;
  if (!pBmpPress) {
    m_pBmpPress= pBmpDefault;
    m_bPressImg= false;
  }
  else {
    m_pBmpPress= pBmpPress;
    m_bPressImg= true;
  }
  m_pBmpDisabled= pBmpDisabled;
  m_bDisImg= (m_pBmpDisabled != NULL);

  m_bPressed= false;
}

//--------------------------------------------------------------------------------
/// Constructor for text button.
CFButton::CFButton(LPCTSTR pCaption, int iWidth, int iHeight)
{
  InitTextButton(pCaption, iWidth, iHeight);
}

//--------------------------------------------------------------------------------
/// Constructor for text button.
CFButton::CFButton(LPCTSTR pCaption)
{
  InitTextButton(pCaption, -1, -1);
}

//--------------------------------------------------------------------------------
void CFButton::InitTextButton(LPCTSTR pCaption, int iWidth, int iHeight)
{
  m_pszClassName       = TEXT("CFButton");
  m_iWidth             = iWidth;
  m_iHeight            = iHeight;
  m_ulDelay            = 1;
  m_bBmpButton         = false;
  m_bTrans             = false;
  m_pBmpDefault        = NULL;
  _tcscpy(m_szCaption, pCaption);
  m_pBmpDefault          = new CFBitmap();
  m_pBmpPress            = new CFBitmap();
  m_pBmpDisabled         = new CFBitmap();
  m_bPressImg          = true;
  m_bDisImg            = true;
  m_bPressed= false;

  // set default colors
  m_colText        = RGB(0, 0, 0);
  m_colBack        = RGB(200, 200, 200);
  m_colBorder      = RGB(0, 0, 0);  
  m_colPressed     = RGB(255, 255, 255);
  m_colBackPressed = RGB(0, 0, 50);
  m_colDisabled    = RGB(150, 150, 150);
  m_colBackDisabled= RGB(200, 200, 200);

  /*
  m_hPenBorder        = CreatePen(PS_SOLID, 1, m_colBorder);
  m_hPenPressed       = CreatePen(PS_SOLID, 1, m_colPressed);
  m_hPenDisabled      = CreatePen(PS_SOLID, 1, m_colDisabled);
  m_hBrushBack        = CreateSolidBrush(m_colBack);
  m_hBrushBackPressed = CreateSolidBrush(m_colBackPressed);
  m_hBrushBackDisabled= CreateSolidBrush(m_colBackDisabled);
  */
}

//--------------------------------------------------------------------------------
CFButton::~CFButton()
{
  delete(m_pBmpDefault);
  if (m_bPressImg) delete(m_pBmpPress);
  if (m_bDisImg) delete(m_pBmpDisabled);
  if (!m_bBmpButton) {
    DeleteObject(m_hPenBorder);
    DeleteObject(m_hPenPressed);
    DeleteObject(m_hPenDisabled);
    DeleteObject(m_hBrushBack);
    DeleteObject(m_hBrushBackPressed);
    DeleteObject(m_hBrushBackDisabled);
  }
}

//--------------------------------------------------------------------------------
void CFButton::SetX(int iPosX)
{
  m_iPosX= iPosX;
  m_pBmpDefault->SetX(iPosX);
  m_pBmpPress->SetX(iPosX);
  if (m_bDisImg) m_pBmpDisabled->SetX(iPosX);
}

//--------------------------------------------------------------------------------
void CFButton::SetY(int iPosY)
{
  m_iPosY= iPosY;
  m_pBmpDefault->SetY(iPosY);
  m_pBmpPress->SetY(iPosY);
  if (m_bDisImg) m_pBmpDisabled->SetY(iPosY);
}

//--------------------------------------------------------------------------------
bool CFButton::StylusDown(int xPos, int yPos)
{
  if (m_pBmpDefault->PointInside(xPos, yPos)) {
    m_bPressed= true;
    m_app->m_pSysCapt= this;
    // transparency set?
    if (m_bTrans) {
      m_pBmpPress->TransBlit(m_colTrans);
    } else {
      m_pBmpPress->Blit();
    } 
    m_app->DrawDoubleBuffer();
    
    m_pSystem->QueueEvent(WM_BUTTONPRESS, m_ulID, NULL);
    
    return(true);
  }
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFButton::StylusUp(int xPos, int yPos)
{
  if (m_app->m_pSysCapt == this) {
    m_bPressed= false;
    m_app->m_pSysCapt= NULL;
    
    //Sleep(m_dwDelay);
    
    // transparency set?
    if (m_bTrans) {
      m_pBmpDefault->TransBlit(m_colTrans);
    } else {
      m_pBmpDefault->Blit();
    }
    
    m_pSystem->QueueEvent(WM_BUTTONRELEASE, m_ulID, NULL);
    return(true);
  }
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
void CFButton::Draw()
{  
  if (m_bEnabled) {
    if (m_bTrans) {
      m_pBmpDefault->TransBlit(m_colTrans);
    } else {
      m_pBmpDefault->Blit();
    }
  } else if (m_bDisImg) {
    if (m_bTrans) {
      m_pBmpDisabled->TransBlit(m_colTrans);
    } else {
      m_pBmpDisabled->Blit();
    }
  }
}

//--------------------------------------------------------------------------------
/// Image is created for non bitmap-buttons here.
void CFButton::SetDC(HDC hdc)
{
   m_hdc= hdc; 
   if (!m_bBmpButton) {
     // width and height specified?
     if ((m_iWidth <= 0)||(m_iHeight <= 0)) {
       RECT rectText;
  
       // get needed space for text
       rectText.left  = 0;
       rectText.top   = 0;
       rectText.right = 0;
       rectText.bottom= 0;
       DrawText(m_hdc, m_szCaption, -1, &rectText, DT_CALCRECT);
       m_iWidth = rectText.right  + 4;
       m_iHeight= rectText.bottom + 2;
     }
     m_pBmpDefault->SetDestDC(hdc);
     m_pBmpDefault->Create(m_iWidth, m_iHeight);
     m_pBmpPress->SetDestDC(hdc);
     m_pBmpPress->Create(m_iWidth, m_iHeight);
     m_pBmpDisabled->SetDestDC(hdc);
     m_pBmpDisabled->Create(m_iWidth, m_iHeight);
     CreateButton();
   }
}

//--------------------------------------------------------------------------------
/// Creates the image for text buttons.
bool CFButton::CreateButton()
{
  if (!m_bBmpButton) {
    HPEN hOldPen;
    HBRUSH hOldBrush;
    RECT rect;
    HDC hdc;
    
    // draw border and background for default image
    hdc= m_pBmpDefault->GetSourceDC();
    hOldBrush= (HBRUSH)SelectObject(hdc, m_hBrushBack);
    hOldPen= (HPEN)SelectObject(hdc, m_hPenBorder);
    Rectangle(hdc, 0, 0, m_pBmpDefault->GetWidth(), m_pBmpDefault->GetHeight());
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    
    // draw text for default image
    rect.left= 2; 
    rect.top= 2;
    rect.right= m_pBmpDefault->GetWidth() - 2;
    rect.bottom= m_pBmpDefault->GetHeight() - 2;
    SetBkColor(hdc, m_colBack);
    SetTextColor(hdc, m_colText);
    DrawText(hdc, m_szCaption, _tcslen(m_szCaption), &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 

    // draw border and background for pressed button image
    hdc= m_pBmpPress->GetSourceDC();
    hOldBrush= (HBRUSH)SelectObject(hdc, m_hBrushBackPressed);
    hOldPen= (HPEN)SelectObject(hdc, m_hPenPressed);
    Rectangle(hdc, 0, 0, m_pBmpPress->GetWidth(), m_pBmpPress->GetHeight());
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    
    // draw text for pressed button image
    rect.left= 1; 
    rect.top= 1;
    rect.right= m_pBmpPress->GetWidth() - 1;
    rect.bottom= m_pBmpPress->GetHeight() - 1;
    SetBkColor(hdc, m_colBackPressed);
    SetTextColor(hdc, m_colPressed);
    DrawText(hdc, m_szCaption, _tcslen(m_szCaption), &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
    
    // draw border and background for disabled button image
    hdc= m_pBmpDisabled->GetSourceDC();
    hOldBrush= (HBRUSH)SelectObject(hdc, m_hBrushBackDisabled);
    hOldPen= (HPEN)SelectObject(hdc, m_hPenDisabled);
    Rectangle(hdc, 0, 0, m_pBmpDisabled->GetWidth(), m_pBmpDisabled->GetHeight());
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    
    // draw text for disabled button image
    rect.left= 1; 
    rect.top= 1;
    rect.right= m_pBmpDisabled->GetWidth() - 1;
    rect.bottom= m_pBmpDisabled->GetHeight() - 1;
    SetBkColor(hdc, m_colBackDisabled);
    SetTextColor(hdc, m_colDisabled);
    DrawText(hdc, m_szCaption, _tcslen(m_szCaption), &rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 

    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
/// Enables or disables the button. If a bitmap is associated with the disabled 
/// state, it will be displayed.
void CFButton::SetEnabled(bool bEnabled)
{
  m_bEnabled= bEnabled;

  // bitmap has a "disabled-image"?
  if (m_bDisImg) Draw();
}


