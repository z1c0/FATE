#ifndef __FSTARTMENU__H__
#define __FSTARTMENU__H__

#include "../base/IFateComponent.h"
#include "../util/FLinkedList.h"

//------------------------------------------------------------------------------
class CFBitmap;

//------------------------------------------------------------------------------
class CFStartMenu : public IFateComponent  
{
public:
	CFStartMenu();
	virtual ~CFStartMenu();
  
  virtual bool StylusDown(int xPos, int yPos);
	virtual bool StylusMove(int xPos, int yPos) { return(false); };
	virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(PdaKey key) { return(false); }  // event not handled
  virtual bool KeyUp(PdaKey key)   { return(false); }  // event not handled
	virtual void Draw();
  virtual void SetSystem(CFSystem *pSystem);
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  
  virtual void AddEntry();
  virtual void AddEntry(CFBitmap *bmpEntry);
  virtual void AppSelected(int iNr);
  
  void SetBackCol(COLORREF col) { m_colBack = col; if (m_pSystem) CreateBmps(); };
  void SetArrowCol(COLORREF col) { m_colArr = col; if (m_pSystem) CreateBmps(); };

protected:
  
  CFLinkedList<CFBitmap*> *m_pPicList;
  CFBitmap *m_bmpBack;
  CFBitmap *m_bmpArrL;
  CFBitmap *m_bmpArrR;
  COLORREF m_colBack;
  COLORREF m_colArr;
  int m_iCurrApp;
  int m_iPicHeight;
  int m_iPicWidth;

private:
  
  bool m_bArrL;
  bool m_bArrR;
  bool m_bApp;
  CFBitmap* m_bmpCurr;
  CFBitmap* CreateAppBmp(int iNr);
  void CreateBmps();
  void UpdatePos();
};

#endif  // __FSTARTMENU__H__
