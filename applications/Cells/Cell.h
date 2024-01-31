// Cell.h: interface for the CCell class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CELL_H__0EDAD1B3_2550_458F_A23D_AB3B8B16B709__INCLUDED_)
#define AFX_CELL_H__0EDAD1B3_2550_458F_A23D_AB3B8B16B709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
class CCell  
{
public:
	CCell();
	virtual ~CCell();
  virtual bool Create(const CFBitmap& destbmp);
  virtual void Move(int x, int y);  // TODO: ugly!!!
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

#endif // !defined(AFX_CELL_H__0EDAD1B3_2550_458F_A23D_AB3B8B16B709__INCLUDED_)
