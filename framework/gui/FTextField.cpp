#include "../base/FateTypeDefs.h"
#include "FTextField.h"
#include "../base/FBitmap.h"
#include "../base/FateApp.h"

//--------------------------------------------------------------------------------
CFTextField::CFTextField(int iWidth, int iHeight) : IFateComponent()
{
  m_pszClassName= TEXT("CFTextField");
  Init();
  m_iWidth = iWidth;
  m_iHeight= iHeight;
}

//--------------------------------------------------------------------------------
CFTextField::CFTextField(TCHAR *pszText, int iWidth, int iHeight) : IFateComponent()
{
  m_pszClassName= TEXT("CFTextField");
  Init();
  m_iWidth = iWidth;
  m_iHeight= iHeight;
}

//--------------------------------------------------------------------------------
CFTextField::~CFTextField()
{
  Cleanup();
}
  
//--------------------------------------------------------------------------------
void CFTextField::Init()
{
  m_bmpImg  = NULL;
  m_pszText = (TCHAR*)malloc(TEXT_BUFFER_LEN);
  memset(m_pszText, 0, TEXT_BUFFER_LEN);
  m_iBuffPos= 0;
  
  m_colBack  = RGB(255, 255, 255);
  m_colBorder= RGB(0, 0, 0);
  m_colText  = RGB(0, 0, 0);
}

//--------------------------------------------------------------------------------
void CFTextField::Cleanup()
{
  SAFE_DELETE(m_pszText);
  SAFE_DELETE(m_bmpImg);
}

//--------------------------------------------------------------------------------
void CFTextField::Draw()
{
  if (m_bmpImg)
  {
    m_bmpImg->Blit();
  }
}

//--------------------------------------------------------------------------------
void CFTextField::DrawBitmap()
{
  // draw background
  m_bmpImg->SetColor(m_colBorder);
  m_bmpImg->SetBackgroundColor(m_colBack);
  m_bmpImg->DrawFilledRect(0, 0, m_iWidth, m_iHeight);
  
  // draw text
  if (m_pszText)
  {
    m_bmpImg->SetTextColor(m_colText);
    m_bmpImg->DrawText(m_pszText, m_rect); 
 }
}

//--------------------------------------------------------------------------------
void CFTextField::SetSystem(CFSystem* pSystem)
{
  m_pSystem = pSystem;
  CreateBitmap();
  UpdatePos();
}

//-------------------------------------------------------------------------------- 
bool CFTextField::CreateBitmap()
{
  m_rect.top= 2; m_rect.left= 2;
  m_rect.bottom= m_iHeight - 2; m_rect.right= m_iWidth - 2;

  if (!m_iWidth || !m_iHeight)
  {
    // at least text must be specified
    if (!m_pszText) return(false);

    m_bmpImg->CalcRectForText(m_pszText, m_rect);
    m_iWidth = m_rect.right - m_rect.left - 2;
    m_iHeight= m_rect.bottom - m_rect.top - 2;
  }
  
  // safe-release old bitmap
  SAFE_DELETE(m_bmpImg);
  m_bmpImg = new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!m_bmpImg->Create(m_iWidth, m_iHeight))
  {
    return(false);
  }
  m_bmpImg->SetX(m_iPosX);
  m_bmpImg->SetY(m_iPosY);

  DrawBitmap();

  return true;
}

//--------------------------------------------------------------------------------
void CFTextField::UpdatePos()
{
  if (m_bmpImg) {
    m_bmpImg->SetX(m_iPosX);
    m_bmpImg->SetY(m_iPosY);
  }
}

//--------------------------------------------------------------------------------
void CFTextField::SetText(TCHAR *pszText)
{
  Cleanup();
  Init();
  _tcsncpy(m_pszText, pszText, TEXT_BUFFER_LEN - 1);
  Draw();
}

//--------------------------------------------------------------------------------
bool CFTextField::Char(TCHAR chChar)
{
  m_pszText[m_iBuffPos++]= chChar;
  DrawBitmap();
  Draw();
  return(true);
}