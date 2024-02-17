#pragma once

#include "../../base/FateTypeDefs.h"

class CFInetAddrImpl
{
  friend class CFSocketImpl;
  friend class CFUDPSocketImpl;
  
public:
  CFInetAddrImpl() {};
  /*
  // constructors
	CFInetAddrImpl() { 
    memset(this, 0, sizeof(CFInetAddr));
    sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
  }; 
  CFInetAddrImpl(const SOCKADDR& sockAddr) { memcpy(this, &sockAddr, sizeof(SOCKADDR)); };
	CFInetAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); };
  CFInetAddrImpl(const ULONG ulAddr, const USHORT ushPort= 0) 
  {  
    memset(this, 0, sizeof(CFInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(ushPort);
	  sin_addr.s_addr= htonl(ulAddr);
  };
  */
  CFInetAddrImpl(const wchar_t*, const int)
  {  
	  assert(false);
  }
  CFInetAddrImpl(const char* pszStrIP, const int port) 
  {  
    m_addr.family= netSocketAddrINET;
    m_addr.port  = NetHToNS(port);
    m_addr.addr  = NetLibAddrAToIN(m_uiLibRefNum, pszStrIP);
  } 	
  /*
  /// returns the address in dotted-decimal format
	char* DottedDecimal() { return inet_ntoa(sin_addr); } 
	USHORT GetPort() const { return ntohs(sin_port); }
  ULONG GetIpAddr() const { return ntohl(sin_addr.s_addr); }
	/// operators added for efficiency
  const CFInetAddr& operator=(const SOCKADDR& sa) { 
    memcpy(this, &sa, sizeof(SOCKADDR));
		return *this; 
  }
  const CFInetAddr& operator=(const SOCKADDR_IN& sin) {
    memcpy(this, &sin, sizeof(SOCKADDR_IN));
		return *this; 
  }
	operator SOCKADDR() { return *((LPSOCKADDR)this); }
	operator LPSOCKADDR() { return (LPSOCKADDR)this; }
  operator LPSOCKADDR_IN() { return (LPSOCKADDR_IN) this; }
  */

private: 
  NetSocketAddrINType m_addr;

  static UInt16 m_uiLibRefNum;
};

