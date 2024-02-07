#pragma once

#include "../base/IFateComponent.h"

//------------------------------------------------------------------------------
class CFBitmap;

//--------------------------------------------------------------------------------
class CFToggleButton : public IFateComponent  
{
public:
	CFToggleButton(CFBitmap *bmpState1, CFBitmap *bmpState2);
	virtual ~CFToggleButton();
  
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(PdaKey key) { return(false); }  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); }  // event not handled
	virtual void Draw();
  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);
  
  void SetTransparency(COLORREF colTrans) { m_colTrans= colTrans; m_bTrans= TRUE; };
  int GetState() { return(m_bState1 ? 0 : 1); };
  void SetState(int iState);

private:
  
  CFBitmap *m_bmpState1;
  CFBitmap *m_bmpState2;
  bool m_bState1;
  COLORREF m_colTrans;
  bool m_bTrans;
};

