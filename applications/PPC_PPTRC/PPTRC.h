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
  virtual bool InitFateApp();
	virtual bool ActivateFateApp();
	virtual bool CloseFateApp();
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
	virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(int vkKey);
  virtual bool KeyUp(int vkKey);
  virtual void Draw();
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool Timer(unsigned long id);
  void DisplayIntro();
  BOOL ReadConfigFile(LPCTSTR pszFileName);
  void Error(LPCTSTR pszErrMsg);
  BOOL SendToPPTHost(char *pMsg);
  BOOL ReceiveFromPPTHost(CFSocket *sock);

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
