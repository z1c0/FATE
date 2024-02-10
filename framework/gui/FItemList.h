#pragma once

#include "../base/IFateComponent.h"

//------------------------------------------------------------------------------
class CFBitmap;

#define DEFAULT_ARROW_DIM  20

//--------------------------------------------------------------------------------
class CFItemList : public IFateComponent  
{
public:

	CFItemList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow);
	virtual ~CFItemList();
 
  virtual bool StylusDown(int iPosX, int iPosY);
  virtual bool StylusMove(int iPosX, int iPosY);
  virtual bool StylusUp(int iPosX, int iPosY) { return false; };
  virtual bool KeyDown(PdaKey key) { return false; };
  virtual bool KeyUp(PdaKey key) { return false; };
  virtual void Draw();
  virtual void SetSystem(CFSystem *pSystem);
  
  bool RemoveItem(int index);
  bool AddItem(LPCTSTR pszItem, LPCTSTR pszAddInfo = NULL) { return AddPicItem(NULL, pszItem, pszAddInfo); };
  bool AddPicItem(CFBitmap *bmpPic, LPCTSTR pszItem, LPCTSTR pszAddInfo= NULL);
  bool ClearItems();
  void SetX(int x) { m_iPosX = x; UpdatePos(); }
  void SetY(int y) { m_iPosY = y; UpdatePos(); }
  void SetItemTextColor(COLORREF colText) { m_colText= colText; };
  void SetBackColor(COLORREF colBack);
  void SetItemTextHighlightColor(COLORREF colHiText) { m_colHiText= colHiText; };
  void SetBackHighlightColor(COLORREF colHiBack);
  void SetBorderColor(COLORREF colBorder);
  void SetSliderColor(COLORREF colSlide);
  int GetItemHeight() { return(m_iItemHeight); };
  int GetItemWidth() { return(m_iItemWidth); };
  int GetItemPos() { return(m_iItemPos); };
  int GetItemCount() { return(m_iItemCount); };
  void SetSelItem(int iSelItem) { m_iSelItem= iSelItem; };

protected:  
  virtual void BeforeDrawItems() {};
  virtual void ItemSelected(ITEMLISTENTRY *pEntry);
	
  void DrawItems();
	bool CreateBackGround();
  void UpdatePos(int offsetY = 0);
  
  CFBitmap *m_bmpBack;
  CFBitmap *m_bmpUpArrow;
  CFBitmap *m_bmpDownArrow;
  CFBitmap *m_bmpSlider;  

  bool m_isFileList;
  int m_iItemWidth;
  ITEMLISTENTRY *m_pEntries;
  COLORREF m_colBack;
  COLORREF m_colBorder;
  COLORREF m_colText;

private:
  void MoveUp();
	void MoveDown();

  int m_iMaxVisItems;
  int m_iItemHeight;
  int m_iItemCount;
  int m_iItemPos;
  int m_iScrollBarHeight;
  int m_iSliderStep;
  int m_iSliderPos;
  int m_iOldY;
  int m_iSelItem;
  RECT m_rScrollBar;
  ITEMLISTENTRY *m_pLast;  // for faster enqueueing
  ITEMLISTENTRY m_saveEntry;
  COLORREF m_colSlide;
  COLORREF m_colHiBack;
  COLORREF m_colHiText;
};

