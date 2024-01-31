#ifndef __FSOCKETWIN32__H__
#define __FSOCKETWIN32__H__

#include "IFSocket.h"

//--------------------------------------------------------------------------------
/// Implementation of a TCP socket.
class CFSocket : public IFSocket
{
public:
  CFSocket() : IFSocket() {};
  virtual ~CFSocket() {};
	
  virtual bool Create();
	
  bool Listen();
	
  bool Connect(const CFInetAddr* pInetAddr);
	bool Connect(char *pAddrStr, USHORT usPort);
	
  int Accept(CFSocket& sock);
	
  virtual int Send(const char* pBuff, const int iSize);
	virtual int Receive(char* pBuff, const int iSize);
	
  int Write(const char* pBuff, const int iSize);
	
  bool GetRemoteAddr(CFInetAddr* pIntAddr);
	
  bool GetInetAddr(CFInetAddr* pInetAddr);
  
  bool IsClientConnecting();
};

#endif  // __FSOCKETWIN32__H__
