#include "FServer.h"

//--------------------------------------------------------------------------------
CFServer::CFServer() : CFSocket()
{
  Create();
}

//--------------------------------------------------------------------------------
CFServer::~CFServer()
{
}

//--------------------------------------------------------------------------------
/// Sends to connected client "iClient".
int CFServer::SendToClient(const char* pBuff, const int iSize, const int iClient)
{
  CFSocket *pSock= m_clientList.Get(iClient);
  if (pSock)
  {
    return pSock->Send(pBuff, iSize);
  }
  else
  {
    return SOCKET_ERROR;
  }
}
  
//--------------------------------------------------------------------------------
/// Receives from connected client "iClient".
int CFServer::ReceiveFromClient(char* pBuff, int iSize, const int iClient)
{
  CFSocket *pSock= m_clientList.Get(iClient);
  if (pSock)
  {
    return pSock->Receive(pBuff, iSize);
  }
  else
  {
    return SOCKET_ERROR;
  }
}

//--------------------------------------------------------------------------------
/// Is the server receiving from client "iClient" currently?
bool CFServer::IsReceiving(int iClient)
{
  CFSocket *pSock= m_clientList.Get(iClient);
  if (pSock)
  {
    return pSock->IsReceiving();
  }
  else
  {
    return false;
  }
}
