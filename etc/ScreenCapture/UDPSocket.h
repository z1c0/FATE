#ifndef __UDPSOCKET__H___
#define __UDPSOCKET__H___

#include "InetAddr.h"


//--------------------------------------------------------------------------------
/// Socket constants.
#define SOCKET_TIMEOUT      SOCKET_ERROR - 1
#define DEFAULT_TIMEOUT     5
#define NO_TIMEOUT          0xFFFF

//--------------------------------------------------------------------------------
/// Implementation of a UDP socket.
class CUDPSocket
{
public:
  CUDPSocket();
  ~CUDPSocket();

  bool Create();
  bool Close();

	bool Bind(LPCINETADDR pInetAddr);
	bool Bind(USHORT usPort);

  int Send(const char* pBuff, const int iSize, LPCINETADDR pInetAddr);
  int Receive(char* pBuff, const int iSize, LPINETADDR pInetAddr);

  //--------------------------------------------------------------------------------
  /// User can specify a timeout for send/receive operations. Timeout is specified
  /// in seconds. The constant NO_TIMEOUT means, that operations will block.
  void SetTimeout(DWORD dwTimeout) { m_dwTimeout= dwTimeout; };  
  
  void EnableBroadcast();
  void JoinMultiCastGroup(const char *pszGroupAddress);
protected:
  int m_iListenPort;
  SOCKET m_hSocket;
  DWORD m_dwTimeout;

  static bool m_bWinsockInitialized;
};

#endif  // __UDPSOCKET__H___
