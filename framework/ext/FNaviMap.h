#ifndef __FNAVIMAP__H__
#define __FNAVIMAP__H__

#include "../base/IFateComponent.h"
#include "../base/FBitmap.h"
#include "../util/FLinkedList.h"

//--------------------------------------------------------------------------------
class CFNaviMap : public IFateComponent  
{
  struct BOOKMARKSTRUCT
  {
    int iPosX;  
    int iPosY;  
    TCHAR szRemark[256];
  };

public:

	CFNaviMap(CFBitmap *bmpMap);
	virtual ~CFNaviMap();

  virtual bool StylusDown(int xPos, int yPos);
	virtual bool StylusMove(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  
  virtual void Draw();
  
  virtual void AddBookMark(int iPosX, int iPosY, LPCTSTR pszRemark= NULL);
  //--------------------------------------------------------------------------------
  /// Add bookmark at current navigation position.
  virtual void AddBookMark(LPCTSTR pszRemark= NULL) 
  {
    if ((m_bmpUser)&&(m_bmpUser->GetX() != -1))
      AddBookMark(m_bmpUser->GetX(), m_bmpUser->GetY(), pszRemark); 
  };
  //--------------------------------------------------------------------------------
  /// Retrieve bookmark at specified position.
  /// If position is < 0 or > number of bookmarks - 1, NULL is returned.
  BOOKMARKSTRUCT* GetBookMark(int iNr)
  {
    return(m_pBookMarkList->Get(iNr));
  }
  //--------------------------------------------------------------------------------
  /// Get Number of bookmarks.
  int GetBookMarkCount()
  {
    return(m_pBookMarkList->GetSize());
  }
  virtual void SetSystem(CFSystem *pSystem);
  virtual void SetX(int iPosX) { m_iPosX = iPosX; CreateNaviBmps(); };
  virtual void SetY(int iPosY) { m_iPosY = iPosY; CreateNaviBmps(); };
  virtual void SetWidth(int iWidth)   { m_iWidth = iWidth;  CreateNaviBmps(); };
  virtual void SetHeight(int iHeight) { m_iHeight= iHeight; CreateNaviBmps(); };
  virtual void SetUserBmp(CFBitmap *bmpUser);
  virtual void SetUserPos(int xPos, int yPos);
  //--------------------------------------------------------------------------------
  /// IMPORTANT: This method is called when user interacts with map.
  virtual void NavigateTo(int iNaviX, int iNaviY) {};  
  void SetArrowCol(COLORREF colArrow) { m_colArrow= colArrow; CreateNaviBmps(); };
  //--------------------------------------------------------------------------------
  /// Sets the scrollstep (in pixels).
  void SetScrollStep(int iStep) { m_iScrollStep= iStep; };  

protected:
  
  CFLinkedList<BOOKMARKSTRUCT*> *m_pBookMarkList;
  CFBitmap *m_bmpMap;
  CFBitmap *m_bmpUser;
  CFBitmap *m_bmpN;
  CFBitmap *m_bmpS;
  CFBitmap *m_bmpW;
  CFBitmap *m_bmpE;
  COLORREF m_colArrow;
  bool m_bShow[4];
  int m_iScrollStep;

private:

  void CreateNaviBmps();
};

#endif  // __FNAVIMAP__H__
