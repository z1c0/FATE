#pragma once

#include "FItemList.h"

class CFDropList : public CFItemList
{
public:
	CFDropList(int iMaxVisItems, int iItemWidth, CFBitmap *bmpDropArrow,
            CFBitmap *bmpUpArrow, CFBitmap *bmpDownArrow);
	virtual ~CFDropList();
	
  virtual bool StylusDown(int xPos, int yPos);
	virtual bool StylusMove(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
	virtual void Draw();
  virtual void SetSystem(CFSystem *pSystem);
  void SetX(int x) { m_iPosX = x; UpdatePos(); }
  void SetY(int y) { m_iPosY = y; UpdatePos(); }
  
  void SetDropEnabled(bool bValue){ m_bDropEnabled = bValue;}
  bool SetSelectedItem(int iNr);

private:

  void DrawSelItem(LPCTSTR pSel);
  virtual void ItemSelected(ITEMLISTENTRY *pEntry);
	
  bool CreateBackGround();
  void UpdatePos();
  
  CFBitmap *m_bmpDropArrow;
  CFBitmap *m_bmpTop;
  
  void* m_pDropData;
  
  bool m_bDropState;
  bool m_bDropEnabled;
  
  LPCTSTR m_pszSelItem;
};

