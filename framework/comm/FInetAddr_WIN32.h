// FInetAddr.h: interface for the CFInetAddr class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FINETADDR_H__06B514DD_FFD0_4258_9133_9F6A2D47FB1A__INCLUDED_)
#define AFX_FINETADDR_H__06B514DD_FFD0_4258_9133_9F6A2D47FB1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock.h>

//--------------------------------------------------------------------------------
class CFInetAddr : public sockaddr_in
{
public:
  // constructors
	CFInetAddr() { 
    memset(this, 0, sizeof(CFInetAddr));
    sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
  }; 
  CFInetAddr(const SOCKADDR& sockAddr) { memcpy(this, &sockAddr, sizeof(SOCKADDR)); };
	CFInetAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); };
  CFInetAddr(const ULONG ulAddr, const USHORT ushPort= 0) {  
    memset(this, 0, sizeof(CFInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(ushPort);
	  sin_addr.s_addr= htonl(ulAddr);
  };
  CFInetAddr(const wchar_t* pStrIP, const USHORT usPort= 0) {  
    char szStrIP[32];
    
    WideCharToMultiByte(CP_ACP, 0, pStrIP, wcslen(pStrIP) + 1, szStrIP, 32, 0, 0);
    memset(this, 0, sizeof(CFInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(szStrIP); 
  }
  CFInetAddr(const char* pStrIP, const USHORT usPort= 0) {  
    memset(this, 0, sizeof(CFInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(pStrIP); 
  } 	
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
};


#endif // !defined(AFX_FINETADDR_H__06B514DD_FFD0_4258_9133_9F6A2D47FB1A__INCLUDED_)
