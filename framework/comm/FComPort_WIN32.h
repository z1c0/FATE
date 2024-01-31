// FComPort.h: interface for the CFComPort class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FCOMPORT_H__5608D25C_1518_4E8A_B252_321EE6A03B4B__INCLUDED_)
#define AFX_FCOMPORT_H__5608D25C_1518_4E8A_B252_321EE6A03B4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>


//--------------------------------------------------------------------------------
enum BaudRate 
{
  BD_110   = 110,
  BD_300   = 300,
  BD_600   = 600,
  BD_1200  = 1200,
  BD_2400  = 2400, 
  BD_4800  = 4800, 
  BD_9600  = 9600, 
  BD_14400 = 14400,
  BD_19200 = 19200, 
  BD_38400 = 38400, 
  BD_56000 = 56000, 
  BD_57600 = 57600, 
  BD_115200= 115200, 
  BD_128000= 128000, 
  BD_256000= 256000 
};


//--------------------------------------------------------------------------------
class CFComPort  
{
public:
	BOOL SetEventChar(char chEvtChar);
	BOOL SetEofChar(char chEofChar);
	BOOL SetErrorChar(char chErrorChar);
	BOOL SetXoffChar(char chXoffChar);
	BOOL SetXonChar(char chXonChar);
	BOOL SetStopBits(BYTE StopBits);
	BOOL SetParity(BYTE Parity);
	BOOL SetByteSize(BYTE ByteSize);
	BOOL SetXoffLim(WORD wXoffLim);
	BOOL SetXonLim(WORD wXonLim);
	BOOL EnableAbortOnError(BOOL bAbort);
	BOOL SetRtsControl(DWORD dwRtsControl);
	BOOL EnableNullBytes(BOOL bNull);
	BOOL EnableErrorChar(BOOL bErrorChar);
	BOOL EnableInX(BOOL bInX);
	BOOL EnableOutX(BOOL bOutX);
	BOOL EnableContinueOnXoff(BOOL bCont);
	BOOL EnableDsrSensitivity(BOOL bDsrSensitivity);
	BOOL SetDtrControl(DWORD dwDtrControl);
	BOOL EnableOutDsrFlow(BOOL bDSR);
	BOOL EnableOutCtsFlow(BOOL bCTS);
	BOOL EnableParityCheck(BOOL bParity);
	CFComPort();
	~CFComPort();
  BOOL Open(int iPort= 1, BaudRate brBaud= BD_9600);
  BOOL Close();
	int Read(char *pszBuffer, int iBuffSize);
	int Write(const char *pszBuffer, int iBuffSize);
  BOOL SetTimeOuts(LPCOMMTIMEOUTS lpCommTimeouts);
  LPCOMMTIMEOUTS GetTimeOuts();
  BOOL SetPortState(DCB *dcb);
  DCB *GetPortState();
  BOOL SetBaudRate(BaudRate brBaud);

private:
  
  COMMTIMEOUTS m_CommTimeouts;
	DCB m_dcb;
	HANDLE m_hPort;
};

#endif // !defined(AFX_FCOMPORT_H__5608D25C_1518_4E8A_B252_321EE6A03B4B__INCLUDED_)
