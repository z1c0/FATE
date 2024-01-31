#ifndef __FBUTTON__H__
#define __FBUTTON__H__

#include "FATE.h"

//------------------------------------------------------------------------------
class CFButton : public IFateComponent  
{
public:
	CFButton(CFBitmap *pBmpDefault, CFBitmap *pBmpPress= NULL, CFBitmap *pBmpDisabled= NULL);
  CFButton(LPCTSTR pCaption, int iWidth, int iHeight);
  CFButton(LPCTSTR pCaption);
	
  virtual ~CFButton();

  virtual bool StylusDown(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  
  virtual bool KeyDown(PdaKey key) { return(false); };  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); };  // event not handled
	
  virtual void Draw();
  
  virtual void SetDC(HDC hdc);
  
  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);
  
  virtual void SetEnabled(bool bEnabled);
  
  void SetTransparency(COLORREF colTrans) { m_colTrans= colTrans; m_bTrans= true; };
  
  void SetDelay(unsigned long ulDelay) { m_ulDelay= ulDelay; };

protected:

  void InitTextButton(LPCTSTR pCaption, int iWidth, int iHeight);
  bool CreateButton();

  CFBitmap *m_pBmpDefault;
  CFBitmap *m_pBmpPress;
  CFBitmap *m_pBmpDisabled;
  
  TCHAR m_szCaption[128];
  
  COLORREF m_colBorder;
  COLORREF m_colBack;
  COLORREF m_colText;
  COLORREF m_colPressed;
  COLORREF m_colBackPressed;
  COLORREF m_colDisabled;
  COLORREF m_colBackDisabled;
  
  HPEN m_hPenBorder;
  HPEN m_hPenPressed;
  HPEN m_hPenDisabled;
  HBRUSH m_hBrushBack;
  HBRUSH m_hBrushBackPressed;
  HBRUSH m_hBrushBackDisabled;
  
  bool m_bBmpButton;
  bool m_bPressImg;
  bool m_bDisImg;
  bool m_bTrans;
  
  COLORREF m_colTrans;
  
  unsigned long m_ulDelay;
  
  bool m_bPressed;
};

#endif // __FBUTTON__H__
