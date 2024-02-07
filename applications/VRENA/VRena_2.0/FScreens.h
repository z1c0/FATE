// FScreens.h: interface for the CFScreens class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FSCREENS_H__6F9B31F6_C75E_4B78_A616_57BFED4FB8DB__INCLUDED_)
#define AFX_FSCREENS_H__6F9B31F6_C75E_4B78_A616_57BFED4FB8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../framework/include/fate.h"
#include "FPPTPanel.h"

#define VRENA_INACTIVE   0x0    // screen is inactive
#define VRENA_SELECTED   0x1    // screen has running application and input focus
#define VRENA_RUNNING    0x2    // screen has running application, NO focus

class CFVRena;

class CFScreens : public CFPanel  
{
public:
  CFScreens(CFPPTPanel **ppPanelsPPT);
	virtual ~CFScreens();
  virtual void Draw();
  virtual void SetX(int iPosX) { m_iPosX= iPosX; UpdatePos(); };
  virtual void SetY(int iPosY) { m_iPosY= iPosY; UpdatePos(); };
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
  virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(PdaKey key) { return(FALSE); };
  virtual bool KeyUp(PdaKey key) { return(FALSE); };
  bool Create();
  int GetActiveScreen() { return(m_iActiveScreen); };
  BOOL StartVR();
  BOOL CloseMovie(int iScreen);
  BOOL IsScreenActive(int iScreen) { return(m_bActive[iScreen]); };
  BOOL ScreenChange(int iSource, int iTarget);
private:
  BOOL ScreenUnload(int iScreen, BOOL bSendToMatrix= TRUE);
  BOOL ScreenActivated(int iScreen, int iMediaType, char *pszFileName);
  BOOL ScreenTapped(int iScreen);
  BOOL StartPPT(int iScreen, char *pszFileName);
  BOOL ReturnToPPT(int iScreen);
  BOOL OpenMovie(int iScreen, char *pszFileName, BOOL bMono);
  BOOL PlayPauseMovie(int iScreen);
  BOOL Start3D();
  BOOL ReturnTo3D();
  BOOL ReturnToVR();
  void UpdatePos();
  CFBitmap *m_bmpScreens[3][3];
  CFBitmap *m_bmpPlay;
  CFBitmap *m_bmpPause;
  CFBitmap *m_bmpBack;
  CFVRena *m_theApp;
  CFPPTPanel **m_aPanelsPPT;
  BOOL m_bCreated;
  BOOL m_bVideoPaused[3];
  int m_iStates[3];
  int m_iMedia[3];
  int m_iActiveScreen;
  BOOL m_bActive[3];
  int m_iNrOfScreensUsed;
};

#endif // !defined(AFX_FSCREENS_H__6F9B31F6_C75E_4B78_A616_57BFED4FB8DB__INCLUDED_)
