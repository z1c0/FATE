#pragma once

#include "../base/FateTypeDefs.h"

class CFInetAddrImpl;

class CFInetAddr
{
  friend class CFSocket;
  friend class CFUDPSocket;
  
  CFInetAddr(CFInetAddrImpl* pImpl) : m_pImpl(pImpl) {}

public:
  CFInetAddr();
  CFInetAddr(const char* ip, int port);
  CFInetAddr(const wchar_t* ip, int port);
  ~CFInetAddr();

private:
  CFInetAddrImpl* m_pImpl;
};