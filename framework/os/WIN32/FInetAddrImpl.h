#pragma once

#include "../../base/FateTypeDefs.h"

class CFInetAddrImpl : public sockaddr_in
{
public:
	CFInetAddrImpl() { 
    memset(this, 0, sizeof(CFInetAddrImpl));
    sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
  }; 
  CFInetAddrImpl(const SOCKADDR& sockAddr) { memcpy(this, &sockAddr, sizeof(SOCKADDR)); };
	CFInetAddrImpl(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); };
  CFInetAddrImpl(const ULONG ulAddr, const USHORT ushPort= 0) {  
    memset(this, 0, sizeof(CFInetAddrImpl));
	  sin_family= AF_INET;
		sin_port= htons(ushPort);
	  sin_addr.s_addr= htonl(ulAddr);
  };
  CFInetAddrImpl(const wchar_t* pStrIP, const USHORT usPort= 0) {  
    char szStrIP[32];
    
    WideCharToMultiByte(CP_ACP, 0, pStrIP, wcslen(pStrIP) + 1, szStrIP, 32, 0, 0);
    memset(this, 0, sizeof(CFInetAddrImpl));
	  sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(szStrIP); 
  }
  CFInetAddrImpl(const char* pStrIP, const USHORT usPort= 0) {  
    memset(this, 0, sizeof(CFInetAddrImpl));
	  sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(pStrIP); 
  } 	
  /// returns the address in dotted-decimal format
	char* DottedDecimal() { return inet_ntoa(sin_addr); } 
	USHORT GetPort() const { return ntohs(sin_port); }
  ULONG GetIpAddr() const { return ntohl(sin_addr.s_addr); }
	/// operators added for efficiency
  const CFInetAddrImpl& operator=(const SOCKADDR& sa) { 
    memcpy(this, &sa, sizeof(SOCKADDR));
		return *this; 
  }
  const CFInetAddrImpl& operator=(const SOCKADDR_IN& sin) {
    memcpy(this, &sin, sizeof(SOCKADDR_IN));
		return *this; 
  }
	operator SOCKADDR() { return *((LPSOCKADDR)this); }
	operator LPSOCKADDR() { return (LPSOCKADDR)this; }
  operator LPSOCKADDR_IN() { return (LPSOCKADDR_IN) this; }
};

