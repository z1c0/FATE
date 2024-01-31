#ifndef __FLABEL__H__
#define __FLABEL__H__

#include "../base/IFateComponent.h"

//------------------------------------------------------------------------------
class CFBitmap;

//------------------------------------------------------------------------------
class CFLabel : public IFateComponent  
{
public:
  CFLabel(LPCTSTR pText= NULL);
  CFLabel(int iWidth, int iHeight);
  CFLabel(LPCTSTR pText, int iWidth, int iHeight);
  
  virtual ~CFLabel();
  
  virtual void Draw();
  
  virtual void SetSystem(CFSystem *pSystem);
  
  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);
  
  TCHAR* GetText() { return(m_pszText); };
  void SetText(const TCHAR* pszText);
  
  void SetColBack(COLORREF col);
  void SetColText(COLORREF col);
  void SetColBorder(COLORREF col);

private:
  
  void Init();
  bool CreateBufferBitmap();
  bool RedrawBufferBitmap();
  
  CFBitmap *m_pBmpImg;
  
  TCHAR *m_pszText;

  RECT m_rectText;
  
  COLORREF m_colBack;
  COLORREF m_colBorder;
  COLORREF m_colText;
};

#endif  // __FLABEL__H__
