#include "../base/FateApp.h"
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
  m_pBmpDefault = NULL;
  m_pBmpPress = NULL;
  m_pBmpDisabled = NULL;

  InitTextButton(pCaption, iWidth, iHeight);
}

//--------------------------------------------------------------------------------
/// Constructor for text button.
CFButton::CFButton(LPCTSTR pCaption)
{
  m_pBmpDefault = NULL;
  m_pBmpPress = NULL;
  m_pBmpDisabled = NULL;

  InitTextButton(pCaption, -1, -1);
}

//--------------------------------------------------------------------------------
/// Default Constructor.
CFButton::CFButton()
{
  m_pBmpDefault = NULL;
  m_pBmpPress = NULL;
  m_pBmpDisabled = NULL;
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
  _tcscpy(m_szCaption, pCaption);
  if (!m_pBmpDefault) {
    m_pBmpDefault = new CFBitmap();
  }
  if (!m_pBmpPress) {
    m_pBmpPress = new CFBitmap();
  }
  if (!m_pBmpDisabled) {
    m_pBmpDisabled = new CFBitmap();
  }
  m_bPressImg          = true;
  m_bDisImg            = true;
  m_bPressed = false;

  // set default colors
  m_colText        = RGB(0, 0, 0);
  m_colBack        = RGB(200, 200, 200);
  m_colBorder      = RGB(0, 0, 0);  
  m_colPressed     = RGB(255, 255, 255);
  m_colBackPressed = RGB(0, 0, 50);
  m_colDisabled    = RGB(150, 150, 150);
  m_colBackDisabled= RGB(200, 200, 200);
}

//--------------------------------------------------------------------------------
CFButton::~CFButton()
{
  if (m_pBmpDefault) delete(m_pBmpDefault);
  if (m_bPressImg) delete(m_pBmpPress);
  if (m_bDisImg) delete(m_pBmpDisabled);
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
void CFButton::SetCaption(LPCTSTR pCaption)
{
  InitTextButton(pCaption, -1, -1);
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
void CFButton::SetSystem(CFSystem *pSystem)
{
   m_pSystem = pSystem;

   if (!m_bBmpButton) {
     // width and height specified?
     if ((m_iWidth <= 0)||(m_iHeight <= 0)) {
       RECT rectText;
  
       // get needed space for text
       rectText.left  = 0;
       rectText.top   = 0;
       rectText.right = 0;
       rectText.bottom= 0;
       
       CFBitmap::CalcRectForText(m_szCaption, rectText);
       
       m_iWidth = rectText.right  + 4;
       m_iHeight= rectText.bottom + 4;
     }

     CFBitmap *pBmp = m_pSystem->GetDoubleBuffer();

     m_pBmpDefault->SetDestBitmap(*pBmp);
     m_pBmpDefault->Create(m_iWidth, m_iHeight);
     
     m_pBmpPress->SetDestBitmap(*pBmp);
     m_pBmpPress->Create(m_iWidth, m_iHeight);
     
     m_pBmpDisabled->SetDestBitmap(*pBmp);
     m_pBmpDisabled->Create(m_iWidth, m_iHeight);
     
     CreateButton();
   }
}

//--------------------------------------------------------------------------------
/// Creates the image for text buttons.
bool CFButton::CreateButton()
{
  if (!m_bBmpButton) {
    RECT rect;
    rect.left= 2; 
    rect.top= 2;
    rect.right= m_iWidth - 2;
    rect.bottom= m_iHeight - 2;
    
    // draw border and background for default image
    m_pBmpDefault->SetBackgroundColor(m_colBack);
    m_pBmpDefault->SetColor(m_colBorder);
    m_pBmpDefault->DrawFilledRect(0, 0, m_iWidth, m_iHeight);
    
    // draw text for default image
    m_pBmpDefault->SetTextColor(m_colText);
    m_pBmpDefault->DrawText(m_szCaption, rect);
    
    
    // draw border and background for pressed button image
    m_pBmpPress->SetBackgroundColor(m_colBackPressed);
    m_pBmpPress->SetColor(m_colBorder);
    m_pBmpPress->DrawFilledRect(0, 0, m_iWidth, m_iHeight);
    
    // draw text for pressed button image
    m_pBmpPress->SetTextColor(m_colPressed);
    m_pBmpPress->DrawText(m_szCaption, rect);
    
    
    // draw border and background for disabled button image
    m_pBmpDisabled->SetBackgroundColor(m_colBackDisabled);
    m_pBmpDisabled->SetColor(m_colDisabled);
    m_pBmpDisabled->DrawFilledRect(0, 0, m_iWidth, m_iHeight);

    // draw text for disabled button image
    m_pBmpDisabled->SetTextColor(m_colDisabled);
    m_pBmpDisabled->DrawText(m_szCaption, rect);

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


