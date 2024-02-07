// PPTPanel.h: interface for the CPPTPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPTPANEL_H__5F4B30F7_44F9_417B_A449_9A32CA71AC5A__INCLUDED_)
#define AFX_PPTPANEL_H__5F4B30F7_44F9_417B_A449_9A32CA71AC5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../framework/include/fate.h"

class CFPPTPanel : public CFPanel  
{
public:
	CFPPTPanel();
	virtual ~CFPPTPanel();
  virtual void Draw();
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool KeyDown(PdaKey key);
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool DropListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  BOOL Create();
  BOOL StartPPT(char *pszFileName);
  BOOL ClosePPT();
  void SetAddrPPT(CFInetAddr *addrPPT) { m_addrPPT= addrPPT; };
  void SetScreenID(int iScreenID) { m_iScreenID= iScreenID; };
  BOOL SendToPPTHost(char *pMsg);

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
  BOOL ReceiveFromPPTHost(CFSocket *sock);
  void UpdatePos();
  void SetSlideCount(int iSlideCount);
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
  CFInetAddr *m_addrPPT;
  int m_iMaxItems;
  int m_iItemWidth;
  int m_iScreenID;
  int m_iSlideCount;
  int m_iCurrSlide;
  int m_iPrevWidth; 
  int m_iPrevHeight;
  char  *m_pPrevData;
  DWORD m_dwPrevSize;
  BOOL m_bShowCurr;
  RECT m_rectTab[2];
};

#endif // !defined(AFX_PPTPANEL_H__5F4B30F7_44F9_417B_A449_9A32CA71AC5A__INCLUDED_)
