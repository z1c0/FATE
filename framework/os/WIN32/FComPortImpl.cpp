#include "FComPortImpl.h"

//-------------------------------------------------------------------------------- 
CFComPortImpl::CFComPortImpl()
{
  m_hPort= NULL;
}

//-------------------------------------------------------------------------------- 
CFComPortImpl::~CFComPortImpl()
{
  if (m_hPort) CloseHandle(m_hPort);
}

//-------------------------------------------------------------------------------- 
/// Open specified COM port at specified baudrate.
bool CFComPortImpl::Open(int iPort, BaudRate brBaud)
{
  TCHAR szPort[8];
  wsprintf(szPort, TEXT("COM%d:"), iPort);
  m_hPort= CreateFile(szPort, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

  if (m_hPort == INVALID_HANDLE_VALUE)
    return(FALSE);

  m_dcb.DCBlength= sizeof (DCB);       // initialize the DCBlength member. 
  // get the default port setting information.
  GetCommState(m_hPort, &m_dcb);

  // start with default settings    
  m_dcb.fBinary          = TRUE;               // must be enabled
  m_dcb.fParity          = TRUE;               // enable parity checking 
  m_dcb.fOutxCtsFlow     = FALSE;              // no CTS output flow control 
  m_dcb.fOutxDsrFlow     = FALSE;              // no DSR output flow control 
  m_dcb.fDtrControl      = DTR_CONTROL_ENABLE; // DTR flow control type 
  m_dcb.fDsrSensitivity  = FALSE;              // DSR sensitivity 
  m_dcb.fTXContinueOnXoff= TRUE;               // XOFF continues Tx 
  m_dcb.fOutX            = FALSE;              // no XON/XOFF out flow control 
  m_dcb.fInX             = FALSE;              // no XON/XOFF in flow control 
  m_dcb.fErrorChar       = FALSE;              // disable error replacement 
  m_dcb.fNull            = FALSE;              // disable null stripping 
  m_dcb.fRtsControl      = RTS_CONTROL_ENABLE; // RTS flow control 
  m_dcb.fAbortOnError    = FALSE;              // do not abort reads/writes on error
  m_dcb.ByteSize         = 8;                  // number of bits/byte, 4-8 
  m_dcb.Parity           = NOPARITY;           // 0-4=no,odd,even,mark,space 
  m_dcb.StopBits         = ONESTOPBIT;         // 0,1,2 = 1, 1.5, 2 
  
  // configure the port according to the specifications of the DCB structure.
  return SetCommState(m_hPort, &m_dcb) == TRUE;
}

//-------------------------------------------------------------------------------- 
/// Closes handle to COM port.
bool CFComPortImpl::Close()
{
  if (m_hPort)
  {
    return CloseHandle(m_hPort) != 0;
  }
  return false;
}

//-------------------------------------------------------------------------------- 
/// Reads characters from COM port into "pszBuffer. Maximum of characters that will
/// be read is specified in "iBuffSize".
int CFComPortImpl::Read(char *pszBuffer, int iBuffSize)
{
  BYTE  Byte;
  DWORD dwBytesRead;
  DWORD dwCommModemStatus;
  int i= 0;

  // specify a set of events to be monitored for the port.
  SetCommMask(m_hPort, EV_RXCHAR|EV_CTS|EV_DSR|EV_RLSD|EV_RING);
  while (m_hPort != INVALID_HANDLE_VALUE) 
  {
    // wait for an event to occur for the port.
    WaitCommEvent(m_hPort, &dwCommModemStatus, 0);

    // re-specify the set of events to be monitored for the port.
    SetCommMask(m_hPort, EV_RXCHAR|EV_CTS| EV_DSR| EV_RING);

    if (dwCommModemStatus & EV_RXCHAR) {
      // loop for waiting for the data.
      do {
        // read the data from the serial port.
        ReadFile(m_hPort, &Byte, 1, &dwBytesRead, 0);

        // display the data read.
        if (dwBytesRead == 1)
          pszBuffer[i++]= Byte;
        else 
          return(i);

      } while ((dwBytesRead == 1)&&(i < iBuffSize));      
    } 
  }
  return(i);
}

//-------------------------------------------------------------------------------- 
/// Writes characters in "pszBuffer" to COM port. The numbers of characters that
/// will be written is specified in "iBuffSize".
int CFComPortImpl::Write(const char *pszBuffer, int iBuffSize)
{
  DWORD dwBytesWritten;
  int i = 0;
  for (i = 0; i < iBuffSize; i++) {
    WriteFile (m_hPort, pszBuffer + i, 1, &dwBytesWritten, NULL);
    if (!dwBytesWritten) break;
  } 
  
  return i;
}

//-------------------------------------------------------------------------------- 
/// Sets a new COMMTIMEOUTS structure for COM port.
inline BOOL CFComPortImpl::SetTimeOuts(LPCOMMTIMEOUTS lpCommTimeouts)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);

  return(SetCommTimeouts (m_hPort, lpCommTimeouts));
}
  
//-------------------------------------------------------------------------------- 
/// Retrieves COMMTIMEOUTS structure of COM port.
inline LPCOMMTIMEOUTS CFComPortImpl::GetTimeOuts()
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(NULL);

  GetCommTimeouts(m_hPort, &m_CommTimeouts);
  return(&m_CommTimeouts);
}

//-------------------------------------------------------------------------------- 
/// Sets a new DCB structure for COM port.
inline BOOL CFComPortImpl::SetPortState(DCB *dcb)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);

  // get the default port setting information.
  return(SetCommState(m_hPort, dcb));

}

//-------------------------------------------------------------------------------- 
/// Retrieves current DCB structure of port settings.
inline DCB *CFComPortImpl::GetPortState()
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(NULL);
  
  GetCommState(m_hPort, &m_dcb);
  return(&m_dcb);
}

//-------------------------------------------------------------------------------- 
/// Sets the baudrate for the COM-port.
BOOL CFComPortImpl::SetBaudRate(BaudRate brBaud)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.BaudRate= brBaud;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Enables/Disables parity check
BOOL CFComPortImpl::EnableParityCheck(BOOL bParity)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fParity= bParity;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}


//-------------------------------------------------------------------------------- 
/// Enables/Disables monitoring of CTS signal for output flow-control. If set to 
/// TRUE and CTS is turned off, output is suspended until CTS is sent again.
BOOL CFComPortImpl::EnableOutCtsFlow(BOOL bCTS)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fOutxCtsFlow= bCTS;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}


//-------------------------------------------------------------------------------- 
/// Enables/Disables monitoring of DSR signal for output flow-control. If set to 
/// TRUE and DSR is turned off, output is suspended until DSR is sent again.
BOOL CFComPortImpl::EnableOutDsrFlow(BOOL bDSR)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fOutxDsrFlow= bDSR;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Sets the DTR flow-control. Possible values are:
/// DTR_CONTROL_DISABLE, DTR_CONTROL_ENABLE Enables & DTR_CONTROL_HANDSHAKE
BOOL CFComPortImpl::SetDtrControl(DWORD dwDtrControl)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fDtrControl= dwDtrControl;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if the communications driver is sensitive to the state of the DSR 
/// signal. If this member is TRUE, the driver ignores any bytes received, unless 
/// the DSR modem input line is high.
BOOL CFComPortImpl::EnableDsrSensitivity(BOOL bDsrSensitivity)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fDsrSensitivity= bDsrSensitivity;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if transmission stops when the input buffer is full and the driver 
/// has transmitted the XoffChar character. 
BOOL CFComPortImpl::EnableContinueOnXoff(BOOL bCont)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fTXContinueOnXoff= bCont;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if XON/XOFF flow control is used when writing to COM port.
BOOL CFComPortImpl::EnableOutX(BOOL bOutX)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fOutX= bOutX;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if XON/XOFF flow control is used when receiving from COM port.
BOOL CFComPortImpl::EnableInX(BOOL bInX)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fInX= bInX;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}


//-------------------------------------------------------------------------------- 
/// Specifies if bytes received with parity errors are replaced with the character
/// specified by the ErrorChar member.
BOOL CFComPortImpl::EnableErrorChar(BOOL bErrorChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fErrorChar= bErrorChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if null bytes are discarded when received.
BOOL CFComPortImpl::EnableNullBytes(BOOL bNull)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fNull= bNull;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the RTS (request-to-send) flow control. Possible values are:
/// RTS_CONTROL_DISABLE, RTS_CONTROL_ENABLE, 
/// RTS_CONTROL_HANDSHAKE & RTS_CONTROL_TOGGLE 
BOOL CFComPortImpl::SetRtsControl(DWORD dwRtsControl)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fRtsControl= dwRtsControl;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies if read and write operations are terminated if an error occurs.
BOOL CFComPortImpl::EnableAbortOnError(BOOL bAbort)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.fAbortOnError= bAbort;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the minimum number of bytes accepted in the input buffer before the 
/// XON character is sent.
BOOL CFComPortImpl::SetXonLim(WORD wXonLim)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.XonLim= wXonLim;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState (m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the maximum number of bytes accepted in the input buffer before the
/// XOFF character is sent.
BOOL CFComPortImpl::SetXoffLim(WORD wXoffLim)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.XoffLim= wXoffLim;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the number of bits in the bytes transmitted and received. 
BOOL CFComPortImpl::SetByteSize(BYTE ByteSize)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.ByteSize= ByteSize;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the parity scheme to be used. It is one of the following values:
/// EVENPARITY,MARKPARITY, NOPARITY, ODDPARITY, SPACEPARITY
BOOL CFComPortImpl::SetParity(BYTE Parity)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.Parity= Parity;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the number of stop bits to be used. It is one of these values:
/// ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
BOOL CFComPortImpl::SetStopBits(BYTE StopBits)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.StopBits= StopBits;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the value of the XON character for both transmission and reception. 
BOOL CFComPortImpl::SetXonChar(char chXonChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.XonChar= chXonChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the value of the XOFF character for both transmission and reception. 
BOOL CFComPortImpl::SetXoffChar(char chXoffChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.XoffChar= chXoffChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the value of the character used to replace bytes received 
/// with a parity error. 
BOOL CFComPortImpl::SetErrorChar(char chErrorChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.ErrorChar= chErrorChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the value of the character used to signal the end of data. 
BOOL CFComPortImpl::SetEofChar(char chEofChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.EofChar= chEofChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}

//-------------------------------------------------------------------------------- 
/// Specifies the value of the character used to signal an event. 
BOOL CFComPortImpl::SetEventChar(char chEvtChar)
{
  // is port handle valid?
  if ((m_hPort)||(m_hPort == INVALID_HANDLE_VALUE)) return(FALSE);  

  // get the default port setting information.
  GetCommState (m_hPort, &m_dcb);

  // change the DCB structure settings.
  m_dcb.EvtChar= chEvtChar;

  // configure the port according to the specifications of the DCB structure.
  return(SetCommState(m_hPort, &m_dcb));
}
