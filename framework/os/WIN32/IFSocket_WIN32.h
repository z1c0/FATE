#ifndef __IFSOCKETWIN32__H__
#define __IFSOCKETWIN32__H__

#include "FInetAddr_WIN32.h"


//--------------------------------------------------------------------------------
/// Class provides an interface for sockets.
class IFSocket  
{
public:
	IFSocket() 
  { 
    m_hSocket= INVALID_SOCKET; 
    m_dwTimeout= DEFAULT_TIMEOUT; 
    m_iListenPort= -1;
  };
	virtual ~IFSocket() { if ((m_hSocket)&&(m_hSocket != INVALID_SOCKET)) Close(); };
  virtual bool Create()= 0;
  virtual int Send(const char* pBuff, const int iSize)= 0;
  virtual int Receive(char* pBuff, const int iSize)= 0;
  virtual operator SOCKET() { return(m_hSocket); };
  
	static CFInetAddr GetHostByName(const char* pHostName, const USHORT usPort= 0);
	static const char* GetHostByAddr(const CFInetAddr* pInetAddr);

protected:
  
  int m_iListenPort;
};

#endif // __IFSOCKETWIN32__H__
