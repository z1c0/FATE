#pragma once

class CFComPortImpl
{
public:
  bool Open(int iPort, BaudRate brBaud);
  int Write(const char *pszBuffer, int iBuffSize);
};
