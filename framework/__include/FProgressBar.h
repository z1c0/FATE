// FProgressBar.h: interface for the CFProgressBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FPROGRESSBAR_H__7A5CC938_CF84_41EF_BA2C_19981A4EFD77__INCLUDED_)
#define AFX_FPROGRESSBAR_H__7A5CC938_CF84_41EF_BA2C_19981A4EFD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFateComponent.h"
#include "FBitmap.h"


//--------------------------------------------------------------------------------
class CFProgressBar : public IFateComponent  
{
public:
	CFProgressBar(int iWidth, int iHeight);
	virtual ~CFProgressBar();
  
  virtual bool StylusDown(int xPos, int yPos) { return(false); };
	virtual bool StylusMove(int xPos, int yPos) { return(false); };
	virtual bool StylusUp(int xPos, int yPos) { return(false); };
  virtual bool KeyDown(PdaKey key) { return(false); }  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); }  // event not handled
	virtual void Draw();
  virtual void SetDC(HDC hdc);
  
  void SetX(int iPosX) { m_iPosX= iPosX; if (m_hdc) m_bmpBuff->SetX(m_iPosX); };
  void SetY(int iPosY) { m_iPosY= iPosY; if (m_hdc) m_bmpBuff->SetY(m_iPosY); };
  
  bool SetProgress(int iProgress);
  int  GetProgress(int iProgress) { return(m_iProgPerc); };

private:

  void DrawOffScreen();
  CFBitmap *m_bmpBuff;
  COLORREF m_colBorder;
  COLORREF m_colBack;
  COLORREF m_colFront;
  HBRUSH m_hBrushBack;
  HBRUSH m_hBrushFront;
  HPEN m_hPenBorder;
  int m_iBorderWidth;
  int m_iProgPerc;
  int m_iProgGraph;
};

#endif // !defined(AFX_FPROGRESSBAR_H__7A5CC938_CF84_41EF_BA2C_19981A4EFD77__INCLUDED_)
