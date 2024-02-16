#ifndef __IFSOCKETWIN32__H__
#define __IFSOCKETWIN32__H__

#include "FInetAddr_WIN32.h"

//--------------------------------------------------------------------------------
/// Socket constants.
#define SOCKET_TIMEOUT      SOCKET_ERROR - 1
#define DEFAULT_TIMEOUT     5
#define NO_TIMEOUT          0xFFFF


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
  //--------------------------------------------------------------------------------
  /// User can specify a timeout for send/receive operations. Timeout is specified
  /// in seconds. The constant NO_TIMEOUT means, that operations will block.
  virtual void SetTimeout(DWORD dwTimeout) { m_dwTimeout= dwTimeout; };
  virtual operator SOCKET() { return(m_hSocket); };
  inline int GetListenPort() { return(m_iListenPort); };
  
	static CFInetAddr GetHostByName(const char* pHostName, const USHORT usPort= 0);
	static const char* GetHostByAddr(const CFInetAddr* pInetAddr);

protected:
  
  int m_iListenPort;
  SOCKET m_hSocket;
  DWORD m_dwTimeout;
};

#endif // __IFSOCKETWIN32__H__
