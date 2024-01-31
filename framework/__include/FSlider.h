#ifndef __FSLIDER__H__
#define __FSLIDER__H__

#include "IFateComponent.h"

//------------------------------------------------------------------------------
class CFBitmap;

//------------------------------------------------------------------------------
class CFSlider : public IFateComponent  
{
public:
	CFSlider(CFBitmap *bmpBack, CFBitmap *bmpSlide);
	virtual ~CFSlider();

  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(PdaKey key) { return(false); }  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); }  // event not handled
	virtual void Draw();
  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);
  
  int GetValue();
  bool SetValue(int iValue);
  int GetMaxValue() { return(m_iMaxValue); };
  void SetMaxValue(int iMaxValue) { m_iMaxValue= iMaxValue; };

private:
  
  CFBitmap *m_bmpBack;
  CFBitmap *m_bmpSlide;
  int m_iDrag;
  int m_iTop;
  int m_iBottom;
  int m_iMaxValue;
  int m_iSlideMid;
  int m_iValue;
  bool m_bValSet;  /// Determines whether the slider's value was set via a method call or not.
  bool m_bVer;     /// Vertical or horizontal slider?
};

#endif  // __FSLIDER__H__