#pragma once

#include "../../base/FateTypeDefs.h"
#include <netinet/in.h>
#include <arpa/inet.h>

class CFInetAddrImpl
{
  friend class CFSocketImpl;
  friend class CFUDPSocketImpl;
public:
	CFInetAddrImpl() {}
  CFInetAddrImpl(const char* ip, int port)
  {
    ::bzero(&m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip);
    m_addr.sin_port = htons(port);
  }
  CFInetAddrImpl(const wchar_t* ip, int port) {}

private:
  sockaddr_in m_addr;
};