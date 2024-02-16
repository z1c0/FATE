#ifndef __FINETADDR_PALMOS__H__
#define __FINETADDR_PALMOS__H__


//--------------------------------------------------------------------------------
class CFInetAddr
{
  friend class IFSocket;
  friend class CFUDPSocket;
  friend class CSocket;

public:
  CFInetAddr() {};
  /*
  // constructors
	CFInetAddr() { 
    memset(this, 0, sizeof(CFInetAddr));
    sin_family= AF_INET;
		sin_port= 0;
		sin_addr.s_addr= 0;
  }; 
  CFInetAddr(const SOCKADDR& sockAddr) { memcpy(this, &sockAddr, sizeof(SOCKADDR)); };
	CFInetAddr(const SOCKADDR_IN& sin) { memcpy(this, &sin, sizeof(SOCKADDR_IN)); };
  CFInetAddr(const ULONG ulAddr, const USHORT ushPort= 0) 
  {  
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
  */
  CFInetAddr(const char* pszStrIP, const unsigned short usPort= 0) 
  {  
    m_addr.family= netSocketAddrINET;
    m_addr.port  = NetHToNS(usPort);
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


#endif  // __FINETADDR_PALMOS__H__
