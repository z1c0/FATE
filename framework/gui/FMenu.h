#ifndef __FMENU__H__
#define __FMENU__H__

#include "../base/IFateComponent.h"

//------------------------------------------------------------------------------
class CFBitmap;


//------------------------------------------------------------------------------
class CFMenu : public IFateComponent  
{
public:
  typedef struct MENUITEM
  {
    LPCTSTR pText;
    CFBitmap *pBmp;
    bool bEnabled;
    int iPosHor;
    int iPosVer;
    struct MENUITEM *pNext;
    struct MENUITEM *pDown;
    struct MENUITEM *pUp;
  };

	CFMenu();
	virtual ~CFMenu();

  virtual bool StylusDown(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  
  virtual void Draw();

  virtual void SetX(int iPosX) {};  // Implemented empty. Menu is always placed at 0/0.
  virtual void SetY(int iPosX) {};  // Implemented empty. Menu is always placed at 0/0.
  
  bool AddEntry(LPCTSTR pText);
  bool AddSubEntry(LPCTSTR pText, int iPos);
  bool ClearEntries();

private:

  bool Create();
  
  void OpenMenu(MENUITEM *pItem);
  void CloseMenu();
  
  int GetMaxWidth(MENUITEM *pItem);
  CFBitmap *CreateMenuBmp(LPCTSTR pText, int iWidth= -1);

  int m_iMainNr;
  int m_iVerSpace;
  int m_iHorSpace;
  
  MENUITEM *m_pItems;
  int m_iItemHeight;
  COLORREF m_colText;
  COLORREF m_colBorder;
  COLORREF m_colBack;
  bool m_bCreated;
  MENUITEM *m_pCurrOpen;
  bool m_bSubMenuOpen;
};

#endif  // __FMENU__H__
