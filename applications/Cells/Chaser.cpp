#include "Game.h"
#include "Chaser.h"

//--------------------------------------------------------------------------------
CChaser::CChaser() : CCell()
{
}

//--------------------------------------------------------------------------------
CChaser::~CChaser()
{
}

//--------------------------------------------------------------------------------
bool CChaser::Create(const CFBitmap& destbmp)
{
  m_bmp= new CFBitmap(destbmp);
  m_bmp->Create(m_iWidth, m_iHeight);
  m_bmp->SolidFill(RGB(0, 0, 255));

  return(TRUE);
}

//--------------------------------------------------------------------------------
void CChaser::Move(int x, int y)
{
  POINT p0, p1, p2;
  int iRet;
  int iAux;

  // create auxiliary points
  p0.x= m_iPosX; p0.y= m_iPosY;
  p1.x= m_iPosX + m_iVec[0]; p1.y= m_iPosY + m_iVec[1];
  p2.x= x;  p2.y= y;
  
  // use CCW function to determine where we have to go
  iRet= CCW(p0, p1, p2);
  if (iRet == -1) {
    iAux= m_iVec[0];
    m_iVec[0]= -m_iVec[1];
    m_iVec[1]= iAux;
  } else if (iRet == 1) {
    iAux= m_iVec[0];
    m_iVec[0]= m_iVec[1];
    m_iVec[1]= -iAux;
  }

  // within bounds?
  if ((m_iPosX <= BOUND_L)||(m_iPosX >= BOUND_R)) m_iVec[0]*= -1;
  if ((m_iPosY <= BOUND_U)||(m_iPosY >= BOUND_D)) m_iVec[1]*= -1;
  
  m_iPosX+=  m_iVec[0];
  m_iPosY+=  m_iVec[1];
}

//--------------------------------------------------------------------------------
int CChaser::CCW(POINT p0, POINT p1, POINT p2) 
{
  int dx1= p1.x - p0.x;
  int dy1= p1.y - p0.y;
  int dx2= p2.x - p0.x;
  int dy2= p2.y - p0.y;
  
  if (dx1*dy2 > dy1*dx2) return(-1);  // COUNTERCLOCKWISE
  if (dx1*dy2 < dy1*dx2) return(1);  // CLOCKWISE
  if ((dx1*dx2 < 0)||(dy1*dy2 < 0)) return(1);  // CLOCKWISE
  if ((dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2)) return(-1);  // COUNTERCLOCKWISE
  
  return(0);  // COLLINEAR
}

