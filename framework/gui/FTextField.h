#pragma once

#include "../base/IFateComponent.h"
#include "../base/IFCharListener.h"

//------------------------------------------------------------------------------
class CFBitmap;

//------------------------------------------------------------------------------
#define TEXT_BUFFER_LEN   512

//------------------------------------------------------------------------------
class CFTextField : public IFateComponent, public IFCharListener
{
public:
	CFTextField(int iWidth, int iHeight);
	CFTextField(TCHAR *pszText, int iWidth, int iHeight);
	virtual ~CFTextField();

  virtual bool StylusDown(int xPos, int yPos) { return(false); };
  virtual bool StylusMove(int xPos, int yPos) { return(false); };
  virtual bool StylusUp(int xPos, int yPos) { return(false); };
  virtual bool KeyDown(PdaKey key) { return(false); }  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); }  // event not handled
  virtual bool Char(TCHAR chChar);
  virtual void Draw();
  virtual void SetSystem(CFSystem* pSystem);
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  
  void SetText(TCHAR *pszText);
  TCHAR* GetText() { return(m_pszText); };

protected:

  bool CreateBitmap();
  void Init();
  void Cleanup();
  void UpdatePos();
  void DrawBitmap();
  CFBitmap *m_bmpImg;
  TCHAR *m_pszText;
  RECT m_rect;
  int m_iBuffPos;
  COLORREF m_colBack;
  COLORREF m_colBorder;
  COLORREF m_colText;
  HPEN m_hPenBorder;
  HBRUSH m_hBrushBack;
};

