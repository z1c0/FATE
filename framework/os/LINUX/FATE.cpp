#include "../../base/FateTypeDefs.h"
#include "../../base/FateApp.h"
#include "../../comm/FUDPSocket.h"
#include "FSystemImpl.h"

//--------------------------------------------------------------------------------
/// Global variables
CFateApp    *g_pApp     = NULL;
CFUDPSocket *g_sockDebug= NULL;
CFInetAddr  *g_addrDebug= NULL;


//--------------------------------------------------------------------------------
// This method has to be implemented by the actual application.
// An instance of a class derived from CFateApp is expected to be returned.
extern CFateApp* CreateFateApp();

//--------------------------------------------------------------------------------
void __InitRemoteConsole(LPCTSTR pszHost, int iPort)
{
  SAFE_DELETE(g_sockDebug);
  SAFE_DELETE(g_addrDebug);
  g_sockDebug= new CFUDPSocket();
  g_sockDebug->Create();
  g_addrDebug= new CFInetAddr(pszHost, iPort);
}

//--------------------------------------------------------------------------------
void __RemoteConsole(LPCTSTR pszFormat, ...)
{
  char szBuffer[256];
  va_list pArgs;
  
  // initialized?
  if (g_sockDebug)
  {
    va_start(pArgs, pszFormat);
    sprintf(szBuffer, pszFormat, pArgs);
    va_end(pArgs);
    strcat(szBuffer, "\r\n");
    g_sockDebug->Send(szBuffer, strlen(szBuffer) + 1, g_addrDebug);
  }
}



//------------------------------------------------------------------------------
static bool StartApplication()
{
  g_pApp = CreateFateApp();
  g_pApp->SetSystem(new CFSystem(new CFSystemImpl(320, 200)));
  g_pApp->Init();
  g_pApp->InitFateApp();

  // Force redraw at startup.
  //g_pApp->GetSystem()->ForceRedraw();

  return true;
}

//------------------------------------------------------------------------------
void EventLoop()
{
	// Main event loop.
	do
  {
    // FATE loop
    if (g_pApp->IsListening())
    {
      g_pApp->CheckServers();
    }
    if (g_pApp->IsFateLoopEnabled())
    {
      g_pApp->Idle();
      g_pApp->DrawDoubleBuffer();
    }
  }
  while (true);
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // basic initialization 
  if (!StartApplication())
  {
    return false;
  }
	
  // main loop
	EventLoop();

  if (g_pApp)
  {
    int nrOfInstances = IFateComponent::GetNumberOfInstances();
    if (nrOfInstances != 0)
    {
      char szTemp[100];
      sprintf(szTemp, "%d instances of IFateComponent are left allocated!", nrOfInstances);
      g_pApp->Message(szTemp);
    }

    delete(g_pApp);
    SAFE_DELETE(g_sockDebug);
    SAFE_DELETE(g_addrDebug);
  }

	return 0;
}