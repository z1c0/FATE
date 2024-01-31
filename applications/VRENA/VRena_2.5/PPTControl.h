#ifndef __PPTCONTROL___H__
#define __PPTCONTROL___H__

#include "stdafx.h"
#include "msppt.h"


//--------------------------------------------------------------------------------
class CPPTControl
{
public:
  CPPTControl(HINSTANCE hInstance);
  ~CPPTControl();

  void SetDimensions(int iPosX, int iPosY, int iWidth, int iHeight);

  bool RunSlideShow(const char *pszFileName);
  bool StopSlideShow();

  bool IsThisFileAlreadyPlaying(const char *pszFileName);

  void DrawMask(bool bDraw);

private:
  ATOM RegisterWindowClass(HINSTANCE hInstance);
 
  bool RunPPT();
  bool IsPPTRunning();

  bool ClosePPT();
  bool KillAllInstancesOfPPT();

  bool OpenSlideShowExists();

  bool CreateMask(const RECT* pRects, int iNrOfRects);

  _Application m_appPPT;
  _Presentation m_pres;
  SlideShowView m_view;

  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;

  bool m_bFirst;
  
  int m_iNrOfWnds;
  HWND m_hWnd[4];
  HWND m_hWndFullMask;
  HINSTANCE m_hInstance;
  char m_szWndClassName[32];

  char m_szFileName[MAX_PATH];
};

#endif  // __PPTCONTROL___H__