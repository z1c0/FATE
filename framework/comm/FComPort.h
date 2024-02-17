#pragma once

class CFComPortImpl;

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

class CFComPort
{
public:
  CFComPort();
  ~CFComPort();

  bool Open(int iPort = 1, BaudRate brBaud = BD_9600);
  int Write(const char *pszBuffer, int iBuffSize);

private:
  CFComPortImpl* m_pImpl;
};
