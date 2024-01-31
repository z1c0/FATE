#ifndef __IFSOCKET_PALMOS__H__
#define __IFSOCKET_PALMOS__H__

#include <PalmOS.h>
#include "FInetAddr.h"


//--------------------------------------------------------------------------------
/// Socket constants.
#define INVALID_SOCKET      -1
#define SOCKET_ERROR        -1
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
    m_refSock= INVALID_SOCKET; 
    m_ulTimeout= DEFAULT_TIMEOUT; 
    m_iListenPort= -1;
  };
	virtual ~IFSocket() { if (m_refSock != INVALID_SOCKET) Close(); };

  virtual bool Create()= 0;
  virtual bool Close();
	virtual bool Bind(const CFInetAddr* pInetAddr);
	virtual bool Bind(int iPort);
  virtual int Send(const char* pBuff, const int iSize)= 0;
  virtual int Receive(char* pBuff, const int iSize)= 0;
  //--------------------------------------------------------------------------------
  /// User can specify a timeout for send/receive operations. Timeout is specified
  /// in seconds. The constant NO_TIMEOUT means, that operations will block.
  virtual void SetTimeout(unsigned long ulTimeout) { m_ulTimeout= ulTimeout; };
  bool IsReceiving();
  int GetListenPort() { return(m_iListenPort); };
  
  //--------------------------------------------------------------------------------
  // static IFSocket methods 
  static bool InitSocketLibrary();  
  static bool CleanupSocketLibrary();

	static CFInetAddr* GetHostByName(const char* pHostName, const int iPort= 0);
	static const char* GetHostByAddr(const CFInetAddr* pInetAddr);
  
protected:

  NetSocketRef m_refSock;
  
  int m_iListenPort;
  unsigned long m_ulTimeout;

  static UInt16 m_uiLibRefNum;
};

#endif // __IFSOCKET_PALMOS__H__
