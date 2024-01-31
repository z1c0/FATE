#ifndef __INETADDR___H__
#define __INETADDR___H__

#include <windows.h>

//--------------------------------------------------------------------------------
class CInetAddr : public sockaddr_in
{
public:
  // constructors
	CInetAddr() { 
    memset(this, 0, sizeof(CInetAddr));
    sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
  }; 
  CInetAddr(const SOCKADDR& sockAddr) { memcpy(this, &sockAddr, sizeof(SOCKADDR)); };
	CInetAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); };
  CInetAddr(const ULONG ulAddr, const USHORT ushPort= 0) {  
    memset(this, 0, sizeof(CInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(ushPort);
	  sin_addr.s_addr= htonl(ulAddr);
  };
  CInetAddr(const char* pStrIP, const USHORT usPort= 0) {  
    memset(this, 0, sizeof(CInetAddr));
	  sin_family= AF_INET;
		sin_port= htons(usPort);
		sin_addr.s_addr= inet_addr(pStrIP); 
  } 	
  /// returns the address in dotted-decimal format
	char* DottedDecimal() { return inet_ntoa(sin_addr); } 
	USHORT GetPort() const { return ntohs(sin_port); }
  ULONG GetIpAddr() const { return ntohl(sin_addr.s_addr); }
	/// operators added for efficiency
  const CInetAddr& operator=(const SOCKADDR& sa) { 
    memcpy(this, &sa, sizeof(SOCKADDR));
		return *this; 
  }
  const CInetAddr& operator=(const SOCKADDR_IN& sin) {
    memcpy(this, &sin, sizeof(SOCKADDR_IN));
		return *this; 
  }
	operator SOCKADDR() { return *((LPSOCKADDR)this); }
	operator LPSOCKADDR() { return (LPSOCKADDR)this; }
  operator LPSOCKADDR_IN() { return (LPSOCKADDR_IN) this; }
};
typedef CInetAddr* LPINETADDR;
typedef const CInetAddr* LPCINETADDR;

#endif  // __INETADDR___H__
