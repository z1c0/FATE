#pragma once

#include "../../framework/include/fate.h"

#include "FPPTPanel.h"
#include "FFilePanel.h"

//--------------------------------------------------------------------------------
#define PPC_PORT       23523
#define POLL_TIMER    0xBABE 

class CPPTRC : public CFateApp  
{
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
  bool ReadConfigFile(LPCTSTR pszFileName);
  void Error(LPCTSTR pszErrMsg);
  bool SendToPPTHost(const char *pMsg);
  bool ReceiveFromPPTHost(CFSocket *sock);

private:
  bool ClosePresentation();
  bool OpenPresentation();
  CFInetAddr *m_addrPPT;
  CFBitmap *m_bmpTitle;
  CFButton *m_btnClose;
  CFPPTPanel *m_panelPPT;
  CFFilePanel *m_panelFiles;
  CFSocket *m_pSockRecv;
};

