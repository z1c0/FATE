#include "FStartMenu.h"
#include "../base/FateTypeDefs.h"
#include "../base/FateApp.h"
#include "../base/FBitmap.h"

//--------------------------------------------------------------------------------
CFStartMenu::CFStartMenu() : IFateComponent()
{
  m_bArrL= false;
  m_bArrR= false;
  m_bApp = false;
  m_iCurrApp= 0;
  m_iPicHeight= 0;
  m_iPicWidth= 0;
  m_bmpBack= NULL;
  m_bmpArrL= NULL;
  m_bmpArrR= NULL;
  m_bmpCurr= NULL;
  m_colBack= RGB(90, 90, 200);
  m_colArr = RGB(255, 255, 255);
  m_pPicList= new CFLinkedList<CFBitmap*>();
}

//--------------------------------------------------------------------------------
CFStartMenu::~CFStartMenu()
{
  SAFE_DELETE(m_bmpBack);
  SAFE_DELETE(m_bmpArrL);
  SAFE_DELETE(m_bmpArrR);
  
  for (int i=0; i<m_pPicList->GetSize(); i++) {
    delete m_pPicList->Get(i);
  }
  delete m_pPicList;
}

//--------------------------------------------------------------------------------
bool CFStartMenu::StylusDown(int xPos, int yPos)
{
  if (m_bmpArrL->PointInside(xPos, yPos)) {
    m_bArrL= true;
    return(true);
  
  } else if (m_bmpArrR->PointInside(xPos, yPos)) {
    m_bArrR= true;
    return(true);  
  } else if (m_bmpCurr->PointInside(xPos, yPos)) {
    m_bApp= true;
    return(true);  
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFStartMenu::StylusUp(int xPos, int yPos)
{
  bool bRet= false;

  if ((m_bArrL)&&(m_bmpArrL->PointInside(xPos, yPos))) {
    m_iCurrApp= max(m_iCurrApp - 1, 0);
    Draw();
    bRet= true;
  
  } else if ((m_bArrR)&&(m_bmpArrR->PointInside(xPos, yPos))) {
    m_iCurrApp= min(m_iCurrApp + 1, m_pPicList->GetSize() - 1);
    Draw();
    bRet= true;  

  } else if ((m_bApp)&&(m_bmpCurr->PointInside(xPos, yPos))) {
    AppSelected(m_iCurrApp);
    bRet= true;  
  }

  m_bArrL= false;
  m_bArrR= false;
  m_bApp = false;
  return(bRet);
}

//--------------------------------------------------------------------------------
void CFStartMenu::Draw()
{
  if (m_pSystem) {
    m_bmpBack->Blit();
    if (m_iCurrApp > 0) m_bmpArrL->Blit();
    if (m_iCurrApp < m_pPicList->GetSize()-1) m_bmpArrR->Blit();
    if (m_pPicList->GetSize()) {
      m_bmpCurr= m_pPicList->Get(m_iCurrApp);
      if (m_bmpCurr) {
        m_bmpCurr->SetX(m_iPosX + m_iWidth / 2 - m_bmpCurr->GetWidth() / 2);
        m_bmpCurr->SetY(m_iPosY + m_iHeight / 2 - m_bmpCurr->GetHeight() / 2);
        m_bmpCurr->Blit();
      }
    }
  }
}

//--------------------------------------------------------------------------------
void CFStartMenu::SetSystem(CFSystem* pSystem)
{
  m_pSystem = pSystem;

  // Init with default values.
  m_iWidth = m_app->GetWidth();
  m_iHeight= 60;
  m_iPicHeight= m_iHeight - 20;
  m_iPicWidth= m_iPicHeight;

  for (int i=0; i<m_pPicList->GetSize(); i++) {
    if (m_pPicList->Get(i) == NULL) {
      m_pPicList->Delete(i);
      m_pPicList->Insert(i, CreateAppBmp(i + 1));
    }
  }

  CreateBmps();
}

//--------------------------------------------------------------------------------
void CFStartMenu::AddEntry()
{
  CFBitmap *bmp = NULL;

  if (m_pSystem) {
    bmp = CreateAppBmp(m_pPicList->GetSize() + 1);
    AddEntry(bmp);
  }
}

//--------------------------------------------------------------------------------
void CFStartMenu::AddEntry(CFBitmap *bmpEntry)
{
  m_pPicList->Append(bmpEntry);
}

//--------------------------------------------------------------------------------
/// Method is called by the framework, whenever an application is selected from
/// the menu.
/// By overriding this method, the user can react accordingly (e.g. launch a 
/// CAVE application).
void CFStartMenu::AppSelected(int iNr)
{
}

//--------------------------------------------------------------------------------
CFBitmap* CFStartMenu::CreateAppBmp(int iNr)
{
  CFBitmap *bmp = NULL;
  TCHAR szText[16];
  RECT rect;

  if (m_pSystem)
  {
    bmp = new CFBitmap(m_pSystem->GetDoubleBuffer());
    bmp->Create(m_iPicWidth, m_iPicHeight);
    bmp->SolidFill(m_colArr);

    // Draw text:
    rect.left  = 0;
    rect.top   = 0;
    rect.right = bmp->GetWidth();
    rect.bottom= bmp->GetHeight();
    _itot(iNr, szText, 10);

    bmp->SetBackgroundColor(m_colBack);
    bmp->SetColor(m_colBack);
    bmp->DrawText(szText, rect);
  }
  return bmp;
}

//--------------------------------------------------------------------------------
void CFStartMenu::CreateBmps()
{
  if (m_pSystem) {
    POINT points[4];
    const int iWidth = 4;

    // create background bitmap
    SAFE_DELETE(m_bmpBack);
    m_bmpBack = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpBack->Create(m_iWidth, m_iHeight);
    m_bmpBack->SolidFill(m_colBack);

    // create left arrow bmp
    SAFE_DELETE(m_bmpArrL);
    m_bmpArrL = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpArrL->Create(m_iWidth / 10, m_iPicHeight);
    m_bmpArrL->SolidFill(m_colBack);
    points[0].x= m_bmpArrL->GetWidth() - iWidth; 
    points[0].y= m_bmpArrL->GetY() + iWidth;
    points[1].x= m_bmpArrL->GetX() + iWidth;
    points[1].y= m_bmpArrL->GetHeight() / 2;
    points[2].x= m_bmpArrL->GetWidth() - iWidth; 
    points[2].y= m_bmpArrL->GetBottom() - iWidth;
    points[3].x= m_bmpArrL->GetWidth() - iWidth; 
    points[3].y= m_bmpArrL->GetY() + iWidth;

    m_bmpArrL->SetBackgroundColor(m_colArr);
    m_bmpArrL->SetColor(m_colArr);
    m_bmpArrL->DrawPolygon(points, 4);    
    
    // create right arrow bmp
    SAFE_DELETE(m_bmpArrR);
    m_bmpArrR = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpArrR->Create(m_bmpArrL->GetWidth(), m_bmpArrL->GetHeight());
    m_bmpArrR->SolidFill(m_colBack);
    points[0].x= m_bmpArrR->GetX() + iWidth;
    points[0].y= m_bmpArrR->GetY() + iWidth;
    points[1].x= m_bmpArrR->GetWidth() - iWidth;
    points[1].y= m_bmpArrR->GetHeight() / 2;
    points[2].x= m_bmpArrR->GetX() + iWidth;
    points[2].y= m_bmpArrR->GetBottom() - iWidth;
    points[3].x= m_bmpArrR->GetX() + iWidth;
    points[3].y= m_bmpArrR->GetY() + iWidth;
    
    m_bmpArrR->SetBackgroundColor(m_colArr);
    m_bmpArrR->SetColor(m_colArr);
    m_bmpArrR->DrawPolygon(points, 4);    

    UpdatePos();
  }
}

//--------------------------------------------------------------------------------
void CFStartMenu::UpdatePos()
{
  if (m_pSystem) {
    m_bmpBack->SetX(m_iPosX);
    m_bmpBack->SetY(m_iPosY);
    
    m_bmpArrL->SetX(m_iPosX + 10);
    m_bmpArrL->SetY(m_iPosY + m_iHeight / 2 - m_iPicHeight / 2);
    
    m_bmpArrR->SetX(m_iWidth - m_bmpArrR->GetWidth() - 10);
    m_bmpArrR->SetY(m_bmpArrL->GetY());
  }
}