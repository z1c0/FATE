#pragma once

#include "../base/IFateComponent.h"
#include "../base/FBitmap.h"

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
  virtual void SetSystem(CFSystem *pSystem);
  
  void SetX(int iPosX) { m_iPosX= iPosX; if (m_bmpBuff) m_bmpBuff->SetX(m_iPosX); };
  void SetY(int iPosY) { m_iPosY= iPosY; if (m_bmpBuff) m_bmpBuff->SetY(m_iPosY); };
  
  bool SetProgress(int iProgress);
  int  GetProgress(int iProgress) { return(m_iProgPerc); };

private:
  void DrawOffScreen();
  CFBitmap *m_bmpBuff;
  COLORREF m_colBorder;
  COLORREF m_colBack;
  COLORREF m_colFront;
  int m_iBorderWidth;
  int m_iProgPerc;
  int m_iProgGraph;
};

