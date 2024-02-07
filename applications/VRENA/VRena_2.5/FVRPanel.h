// FExtPanel.h: interface for the CFExtPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEXTPANEL_H__92800F86_F345_4376_B97F_42BABFB29AFF__INCLUDED_)
#define AFX_FEXTPANEL_H__92800F86_F345_4376_B97F_42BABFB29AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../framework/include/fate.h"


//--------------------------------------------------------------------------------
class CFVRena;

//--------------------------------------------------------------------------------
class CFVRPanel : public CFPanel  
{
public:
	CFVRPanel();
	virtual ~CFVRPanel();
  virtual void Draw();
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  virtual bool StylusDown(int xPos, int yPos);
  
  bool Create();

private:

  bool ButtonReleased(DWORD dwBtnID);
  bool PointInRect(const RECT *rect, int iPosX, int iPosY);
  void UpdatePos();
  void DrawIcons();
  int GetNumberOfApps();
  CFBitmap *m_bmpTitle;
  CFBitmap *m_bmpTab[3];
  CFBitmap **m_bmpIcons;
  CFBitmap **m_bmpPics;
  CFBitmap *m_bmpReset;
  CFButton *m_btnClose;
  CFButton *m_btnLeft;
  CFButton *m_btnRight;
  CFVRena* m_theApp;
  RECT m_rectTab[3];
  int m_iTab;
  int m_iLeftApp;
  int m_iNrOfApps;
  int m_iIconY;
};

#endif // !defined(AFX_FEXTPANEL_H__92800F86_F345_4376_B97F_42BABFB29AFF__INCLUDED_)
