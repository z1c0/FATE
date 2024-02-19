#include "FNaviMap.h"
#include "../base/FateTypeDefs.h"
#include "../base/FateApp.h"

//--------------------------------------------------------------------------------
CFNaviMap::CFNaviMap(CFBitmap *bmpMap) : IFateComponent()
{
  m_bmpN= NULL;
  m_bmpS= NULL;
  m_bmpW= NULL;
  m_bmpE= NULL;
  m_colArrow= RGB(0, 0, 255);
  m_bmpMap= bmpMap;
  m_bmpUser= NULL;
  m_pBookMarkList= new CFLinkedList<BOOKMARKSTRUCT*>;
  m_bShow[0]= false; // N
  m_bShow[1]= true ; // S
  m_bShow[2]= false; // W
  m_bShow[3]= true;  // E
  m_iScrollStep= 4;
  m_iWidth = -1;
  m_iHeight= -1;
}

//--------------------------------------------------------------------------------
CFNaviMap::~CFNaviMap()
{
  SAFE_DELETE(m_bmpMap);
  SAFE_DELETE(m_bmpUser);
  SAFE_DELETE(m_bmpN);
  SAFE_DELETE(m_bmpS);
  SAFE_DELETE(m_bmpW);
  SAFE_DELETE(m_bmpE);
  
  m_pBookMarkList->Clear();
  delete(m_pBookMarkList);
}

//--------------------------------------------------------------------------------
void CFNaviMap::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
  
  // Default values.
  if (m_iWidth  == -1) {
    m_iWidth = m_app->GetWidth();
  }
  if (m_iHeight == -1) {
    m_iHeight= m_app->GetHeight();
  }

  CreateNaviBmps();

  // Create bitmap which indicates user postion.
  m_bmpUser= new CFBitmap(*m_bmpMap);
  m_bmpUser->Create(8, 8);
  m_bmpUser->SolidFill(RGB(255, 0, 0));
  m_bmpUser->SetX(-1);
  m_bmpUser->SetY(-1);
}

//--------------------------------------------------------------------------------
/// User can specify a bitmap which indicates user's current position on the map.
void CFNaviMap::SetUserBmp(CFBitmap *bmpUser)
{
  SAFE_DELETE(m_bmpUser);
  m_bmpUser= bmpUser;
}

//--------------------------------------------------------------------------------
void CFNaviMap::Draw()
{
  if (m_bmpUser->GetX() > 0) {
    m_bmpUser->Blit();
  }
  m_bmpMap->Blit(m_iPosX, m_iPosY, m_iWidth, m_iHeight, m_bmpMap->GetX(), m_bmpMap->GetY());

  if (m_bShow[0]) {
    m_bmpN->TransBlit(RGB(255, 0, 0));
  }
  if (m_bShow[1]) {
    m_bmpS->TransBlit(RGB(255, 0, 0));
  }
  if (m_bShow[2]) {
    m_bmpW->TransBlit(RGB(255, 0, 0));
  }
  if (m_bShow[3]) {
    m_bmpE->TransBlit(RGB(255, 0, 0));
  }
}

//--------------------------------------------------------------------------------
bool CFNaviMap::StylusDown(int xPos, int yPos)
{
  return(StylusMove(xPos, yPos));
}

//--------------------------------------------------------------------------------
bool CFNaviMap::StylusMove(int xPos, int yPos)
{
  if ((m_bShow[0])&&(m_bmpN->PointInside(xPos, yPos))) {
    m_bmpMap->SetY(max(m_bmpMap->GetY() - m_iScrollStep, 0));
    m_bShow[0]= m_bmpMap->GetY() > 0;
    m_bShow[1]= m_bmpMap->GetY() < m_iHeight;
    Draw();
    return(true);
  
  } else if ((m_bShow[1])&&(m_bmpS->PointInside(xPos, yPos))) {
    m_bmpMap->SetY(min(m_bmpMap->GetY() + m_iScrollStep, 
                       m_bmpMap->GetHeight() - m_iHeight));
    m_bShow[0]= m_bmpMap->GetY() > 0;
    m_bShow[1]= m_bmpMap->GetY() < m_iHeight;
    Draw();
    return(true);

  } else if ((m_bShow[2])&&(m_bmpW->PointInside(xPos, yPos))) {
    m_bmpMap->SetX(max(m_bmpMap->GetX() - m_iScrollStep, 0));
    m_bShow[2]= m_bmpMap->GetX() > 0;
    m_bShow[3]= m_bmpMap->GetX() < m_iWidth;
    Draw();
    return(true);
  
  } else if ((m_bShow[3])&&(m_bmpE->PointInside(xPos, yPos))) {
    m_bmpMap->SetX(min(m_bmpMap->GetX() + m_iScrollStep,
                       m_bmpMap->GetWidth() - m_iWidth));
    m_bShow[2]= m_bmpMap->GetX() > 0;
    m_bShow[3]= m_bmpMap->GetX() < m_iWidth;
    Draw();
    return(true);    
  }

  return(false);
}

//--------------------------------------------------------------------------------
bool CFNaviMap::StylusUp(int xPos, int yPos)
{
  if ((xPos > m_iPosX)&&(xPos < m_iWidth)&&(yPos > m_iPosY)&&(yPos < m_iHeight)) {
    if ((m_bmpN->PointInside(xPos, yPos))&&(m_bShow[0])) return(false);
    else if ((m_bmpS->PointInside(xPos, yPos))&&(m_bShow[1])) return(false);
    else if ((m_bmpW->PointInside(xPos, yPos))&&(m_bShow[2])) return(false);
    else if ((m_bmpE->PointInside(xPos, yPos))&&(m_bShow[3])) return(false);
    else {
      SetUserPos(abs(m_bmpMap->GetX()) + xPos - m_iPosX - m_bmpUser->GetWidth() / 2,
                 abs(m_bmpMap->GetY()) + yPos - m_iPosY - m_bmpUser->GetHeight() / 2);
      return(true);
    }
  }

  return(false);
}


//--------------------------------------------------------------------------------
/// Set user's position on the map.
void CFNaviMap::SetUserPos(int xPos, int yPos)
{
  if (m_pSystem) {
    // else set user bitmap
    m_bmpUser->RestoreUnder();
    m_bmpUser->SetX(xPos);
    m_bmpUser->SetY(yPos);
    m_bmpUser->SaveUnder();
    Draw();
    // Call framework method.
    NavigateTo(m_bmpUser->GetX(), m_bmpUser->GetY());
  }
}

//--------------------------------------------------------------------------------
/// Adds a bookmark at the specified navigation position.
/// A remark can be added for textual description of the bookmark.
void CFNaviMap::AddBookMark(int iPosX, int iPosY, LPCTSTR pszRemark /* = NULL */)
{
  BOOKMARKSTRUCT* pBookMark= new BOOKMARKSTRUCT;

  pBookMark->iPosX= iPosX;
  pBookMark->iPosY= iPosY;
  _tcsncpy(pBookMark->szRemark, pszRemark, 255);
  pBookMark->szRemark[255]= 0;
  m_pBookMarkList->Append(pBookMark);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which creates the navigation bitmaps.
void CFNaviMap::CreateNaviBmps()
{
  if (m_pSystem) {
    const int iWidth = 4;

    SAFE_DELETE(m_bmpN);
    SAFE_DELETE(m_bmpS);
    SAFE_DELETE(m_bmpW);
    SAFE_DELETE(m_bmpE);
    POINT points[4];
  
    // Adaption needed?
    m_iWidth = min(m_iWidth , m_app->GetWidth() - m_iPosX);
    m_iHeight= min(m_iHeight, m_app->GetHeight() - m_iPosX);

    // Create north navigation bitmap.
    m_bmpN = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpN->Create(m_iWidth / 4, m_iHeight / 12);
    m_bmpN->SolidFill(RGB(255, 0, 0));
    points[0].x= m_bmpN->GetX() + iWidth;
    points[0].y= m_bmpN->GetBottom() - iWidth;
    points[1].x= m_bmpN->GetRight() - iWidth;
    points[1].y= m_bmpN->GetBottom() - iWidth;
    points[2].x= m_bmpN->GetWidth() / 2;
    points[2].y= m_bmpN->GetY() + iWidth;
    points[3].x= m_bmpN->GetX() + iWidth;
    points[3].y= m_bmpN->GetBottom() - iWidth;
    m_bmpN->SetColor(m_colArrow);
    m_bmpN->DrawPolygon(points, 4);
    m_bmpN->SetX(m_iPosX + m_iWidth / 2 - m_bmpN->GetWidth() / 2);
    m_bmpN->SetY(m_iPosY + 5);
  
    // create south navigation bitmap
    m_bmpS = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpS->Create(m_bmpN->GetWidth(), m_bmpN->GetHeight());
    m_bmpS->SolidFill(RGB(255, 0, 0));
    points[0].x= m_bmpS->GetX() + iWidth;
    points[0].y= m_bmpS->GetY() + iWidth;
    points[1].x= m_bmpS->GetRight() - iWidth;
    points[1].y= m_bmpS->GetY() + iWidth;
    points[2].x= m_bmpS->GetWidth() / 2;
    points[2].y= m_bmpS->GetBottom() - iWidth;
    points[3].x= m_bmpS->GetX() + iWidth;
    points[3].y= m_bmpS->GetY() + iWidth;
    m_bmpS->SetColor(m_colArrow);
    m_bmpS->DrawPolygon(points, 4);
    m_bmpS->SetX(m_bmpN->GetX());
    m_bmpS->SetY(m_iPosY + m_iHeight - m_bmpS->GetHeight() - 5);
  
    // Create west navigation bitmap.
    m_bmpW = new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpW->Create(m_iWidth / 12, m_iHeight / 4);
    m_bmpW->SolidFill(RGB(255, 0, 0));
    points[0].x= m_bmpW->GetWidth() - iWidth; 
    points[0].y= m_bmpW->GetY() + iWidth;
    points[1].x= m_bmpW->GetX() + iWidth;
    points[1].y= m_bmpW->GetHeight() / 2;
    points[2].x= m_bmpW->GetWidth() - iWidth; 
    points[2].y= m_bmpW->GetBottom() - iWidth;
    points[3].x= m_bmpW->GetWidth() - iWidth; 
    points[3].y= m_bmpW->GetY() + iWidth;
    m_bmpW->SetColor(m_colArrow);
    m_bmpW->DrawPolygon(points, 4);
    m_bmpW->SetX(m_iPosX + 5);
    m_bmpW->SetY(m_iPosY + m_iHeight / 2 - m_bmpW->GetHeight() / 2);
  
    // Create east navigation bitmap.
    m_bmpE= new CFBitmap(m_pSystem->GetDoubleBuffer());
    m_bmpE->Create(m_bmpW->GetWidth(), m_bmpW->GetHeight());
    m_bmpE->SolidFill(RGB(255, 0, 0));
    points[0].x= m_bmpE->GetX() + iWidth;
    points[0].y= m_bmpE->GetY() + iWidth;
    points[1].x= m_bmpE->GetWidth() - iWidth;
    points[1].y= m_bmpE->GetHeight() / 2;
    points[2].x= m_bmpE->GetX() + iWidth;
    points[2].y= m_bmpE->GetBottom() - iWidth;
    points[3].x= m_bmpE->GetX() + iWidth;
    points[3].y= m_bmpE->GetY() + iWidth;
    m_bmpE->SetColor(m_colArrow);
    m_bmpE->DrawPolygon(points, 4);
    m_bmpE->SetX(m_iPosX + m_iWidth - m_bmpW->GetWidth() - 5);
    m_bmpE->SetY(m_bmpW->GetY());
  }
}

