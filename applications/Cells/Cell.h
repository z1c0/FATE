#pragma once
#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
class CCell  
{
public:
	CCell();
	virtual ~CCell();
  virtual bool Create(const CFBitmap& destbmp);
  virtual void Move(int x, int y);
  virtual void Draw();
  void SetX(int iPosX) { m_iPosX= iPosX; };
  int GetX() { return(m_iPosX); };
  void SetY(int iPosY) { m_iPosY= iPosY; };
  int GetY() { return(m_iPosY); };
  bool Touches(const CCell &cell);
  bool PointInside(int iPosX, int iPosY)
  {
    return(m_bmp->PointInside(iPosX, iPosY));
  };
protected:
  CFBitmap *m_bmp;
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;
  int m_iVec[2];
};

