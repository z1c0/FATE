#ifndef __FBUTTON__H__
#define __FBUTTON__H__

//#include "../base/FateTypeDefs.h"
#include "../base/IFateComponent.h"
#include "../base/FBitmap.h"


//------------------------------------------------------------------------------
class CFButton : public IFateComponent  
{
public:

  CFButton() FATE_SECTION;
	CFButton(CFBitmap *pBmpDefault, CFBitmap *pBmpPress= NULL, CFBitmap *pBmpDisabled= NULL) FATE_SECTION;
  CFButton(LPCTSTR pCaption, int iWidth, int iHeight) FATE_SECTION;
  CFButton(LPCTSTR pCaption) FATE_SECTION;
	
  virtual ~CFButton() FATE_SECTION;

  virtual bool StylusDown(int xPos, int yPos) FATE_SECTION;
	virtual bool StylusUp(int xPos, int yPos) FATE_SECTION;
  
  virtual void Draw() FATE_SECTION;
  
  virtual void SetSystem(CFSystem *pSystem) FATE_SECTION;
  
  virtual void SetX(int iPosX) FATE_SECTION;
  virtual void SetY(int iPosY) FATE_SECTION;

  virtual void SetCaption(LPCTSTR pCaption) FATE_SECTION;
  
  virtual void SetEnabled(bool bEnabled) FATE_SECTION;
  
  void SetTransparency(COLORREF colTrans) { m_colTrans= colTrans; m_bTrans= true; };
  
  void SetDelay(unsigned long ulDelay) { m_ulDelay= ulDelay; };

protected:

  void InitTextButton(LPCTSTR pCaption, int iWidth, int iHeight) FATE_SECTION;
  bool CreateButton() FATE_SECTION;

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
  
  bool m_bBmpButton;
  bool m_bPressImg;
  bool m_bDisImg;
  bool m_bTrans;
  
  COLORREF m_colTrans;
  
  unsigned long m_ulDelay;
  
  bool m_bPressed;
};

#endif // __FBUTTON__H__
