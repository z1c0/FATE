// PPTRC.h: interface for the CPPTRC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPTRC_H__21F34F32_30B8_468D_95BA_27CC4E2DFACB__INCLUDED_)
#define AFX_PPTRC_H__21F34F32_30B8_468D_95BA_27CC4E2DFACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../framework/include/fate.h"

#include "FPPTPanel.h"
#include "FFilePanel.h"

//--------------------------------------------------------------------------------
#define PPC_PORT       23523
#define POLL_TIMER    0xBABE 


class CPPTRC : public CFateApp  
{
  friend DWORD WINAPI RecvThreadFunc(LPVOID pParam);
public:
	CPPTRC();
	virtual ~CPPTRC();
  virtual BOOL InitFateApp();
	virtual BOOL ActivateFateApp();
	virtual BOOL CloseFateApp();
  virtual BOOL StylusDown(int xPos, int yPos);
  virtual BOOL StylusMove(int xPos, int yPos);
	virtual BOOL StylusUp(int xPos, int yPos);
  virtual BOOL KeyDown(int vkKey);
  virtual BOOL KeyUp(int vkKey);
  virtual void Draw();
  virtual BOOL ButtonReleased(DWORD dwBtnID);
  void DisplayIntro();
  BOOL ReadConfigFile(LPCTSTR pszFileName);
  void Error(LPCTSTR pszErrMsg);
  BOOL SendToPPTHost(char *pMsg);
  BOOL ReceiveFromPPTHost(CFSocket *sock);
  virtual BOOL ExtraEventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
  BOOL ClosePresentation();
  BOOL OpenPresentation();
  CFInetAddr *m_addrPPT;
  BOOL m_bIntro;
  CFBitmap *m_bmpTitle;
  CFButton *m_btnClose;
  CFPPTPanel *m_panelPPT;
  CFFilePanel *m_panelFiles;
  CFSocket *m_pSockRecv;
};

#endif // !defined(AFX_PPTRC_H__21F34F32_30B8_468D_95BA_27CC4E2DFACB__INCLUDED_)
