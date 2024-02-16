#pragma once

#include "../util/FLinkedList.h"
#include "FSocket.h"

//--------------------------------------------------------------------------------
class CFServer : public CFSocket  
{
public:
	CFServer();
	virtual ~CFServer();
  void Listen(int port) 
  {
    Bind(port);
    CFSocket::Listen();
  };
  int SendToClient(const char* pBuff, const int iSize, const int iClient);
  int ReceiveFromClient(char* pBuff, int iSize, const int iClient);
  /// How many established connections are there?
  unsigned int GetNrOfAcceptedSockets() { return(m_clientList.GetSize()); };
  bool IsReceiving(int iClient);
  /// Get pointer to corresponding client socket number  i 
  CFSocket* GetClient(int i) { return(m_clientList.Get(i)); };

private:  
  /// List of all accepted clients.
  CFLinkedList<CFSocket*> m_clientList;
};


