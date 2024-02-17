#pragma once

#include "../base/FateTypeDefs.h"

//--------------------------------------------------------------------------------
/// Socket constants.
#define SOCKET_TIMEOUT      SOCKET_ERROR - 1
#define DEFAULT_TIMEOUT     5
#define NO_TIMEOUT          0xFFFF

class CFInetAddrImpl;

class CFInetAddr
{
public:
  CFInetAddr();
  CFInetAddr(const char* ip, int port);
  ~CFInetAddr();

private:
  CFInetAddrImpl* m_pImpl;
};