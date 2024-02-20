#pragma once

#include "../../framework/include/fate.h"

class CPPTRC;

class CFPPTPanel : public CFPanel  
{
public:
	CFPPTPanel();
	virtual ~CFPPTPanel();
  virtual void Draw();
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool KeyDown(int vkKey);
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool DropListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  bool Create();
  bool StartPPT(char *pszFileName);
  bool ClosePPT();
  void SetSlideCount(int iSlideCount);
  bool StorePreview(char *pBuff, DWORD dwSize);
  void SetCurrSlide(int iNr) { m_iCurrSlide= iNr; };
private:
  void NextSlide();
  void PrevSlide();
  void GotoSlide(DWORD dwNr);
  void NextLink();
  void PrevLink();
  void ActivateLink();
  void BlackScreen();
  void WhiteScreen();
  void DrawCurrent();
  void DrawNext();
  void DeletePreviews();
  void UpdatePos();
  CFBitmap *m_bmpCurr;
  CFBitmap *m_bmpNext;
  CFBitmap *m_bmpSlide;
  CFBitmap **m_bmpPreviews;
  CFDropList *m_dropList;
  CFButton *m_btnClose;
  CFButton *m_btnNextSlide;
  CFButton *m_btnPrevSlide;
  CFButton *m_btnNextLink;
  CFButton *m_btnPrevLink;
  CFButton *m_btnActLink;
  CFButton *m_btnBlack;
  CFButton *m_btnWhite;
  CPPTRC *m_theApp;
  CFSocket m_sockListen;
  int m_iMaxItems;
  int m_iItemWidth;
  int m_iSlideCount;
  int m_iCurrSlide;
  int m_iPrevWidth; 
  int m_iPrevHeight;
  bool m_bShowCurr;
  RECT m_rectTab[2];
};

