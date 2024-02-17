#pragma once

#include "../../base/FateTypeDefs.h"
#include "../../comm/FComPort.h"

//--------------------------------------------------------------------------------
class CFComPortImpl
{
public:
  CFComPortImpl();
  ~CFComPortImpl();

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
  bool Open(int iPort, BaudRate brBaud);
  bool Close();
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

