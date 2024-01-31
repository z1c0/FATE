#ifndef __FDROPLIST__H__
#define __FDROPLIST__H__

#include "FItemList.h"

//--------------------------------------------------------------------------------
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
  
  void SetDropEnabled(bool bValue){ m_bDropEnabled = bValue;}
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); }
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); }
  virtual void SetSystem(CFSystem *pSystem);
  bool SetSelectedItem(int iNr);

private:

  void DrawSelItem(LPCTSTR pSel);
  virtual void ItemSelected(ITEMLISTENTRY *pEntry);
	
  virtual bool CreateBackGround();
  void UpdatePos();
  
  CFBitmap *m_bmpDropArrow;
  CFBitmap *m_bmpTop;
  
  void* m_pDropData;
  
  bool m_bDropState;
  bool m_bDropEnabled;
  
  RECT m_rectSel;
  LPCTSTR m_pszSelItem;
};

#endif  // __FDROPLIST__H__
