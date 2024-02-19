#pragma once
#include "../base/IFateComponent.h"

//------------------------------------------------------------------------------

class CFInetAddr;
class CFUDPSocket;

//--------------------------------------------------------------------------------
class CFController : public IFateComponent  
{
  friend DWORD WINAPI SendThreadFunc(LPVOID pParam);
public:

	CFController();
	CFController(CFInetAddr *pAddr);
	CFController(char *pszHost, USHORT usPort);
	virtual ~CFController();
  
  bool ReadConfigFile(LPCTSTR pszFileName);
  bool ReadConfigData(char *pszData);
  
  virtual bool StylusDown(int xPos, int yPos) { return(false); };
  virtual bool StylusMove(int xPos, int yPos) { return(false); };
	virtual bool StylusUp(int xPos, int yPos) { return(false); };
  virtual bool KeyDown(PdaKey key);
  virtual bool KeyUp(PdaKey key);
  virtual void Draw() {};
  
  bool SendContInfo();
  void ResetValues();
  void SetDestAddr(CFInetAddr *pAddr);
  bool StartSending(DWORD dwMillis= 500);
  bool StopSending();
  virtual void ProcessData();

protected:

  int m_iDirs[2];
  int m_iBtns[4];

private:
  
  void Init(CFInetAddr *addr);
  CFUDPSocket *m_sockCont;
  CFInetAddr *m_addrCont;
  int m_iCounter;
  int m_iByte8;
  int m_iByte9;
  int m_iByte10;
  int m_iByte11;
  HANDLE m_hThread;
  bool m_bRun;
  DWORD m_dwSleep;
};
