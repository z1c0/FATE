#include "../../base/FateTypeDefs.h"
#include "../../base/FateApp.h"
#include "../../comm/FUDPSocket.h"
#include "../../base/FSystem.h"
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
  if (g_sockDebug) {
    va_start(pArgs, pszFormat);
    StrVPrintF(szBuffer, pszFormat, pArgs);
    va_end(pArgs);
    StrCat(szBuffer, "\r\n");

    g_sockDebug->Send(szBuffer, strlen(szBuffer) + 1, g_addrDebug);
  }
}



//------------------------------------------------------------------------------
static bool StartApplication()
{
  UInt32 uiWidth = 320;
  UInt32 uiHeight= 320;
  UInt32 uiDepth = 0x800000;  // 24BIT
  Boolean bUseColor = true;     

  WinScreenMode(winScreenModeGet, &uiWidth, &uiHeight, NULL, &bUseColor);
  WinScreenMode(winScreenModeSet, &uiWidth, &uiHeight, &uiDepth, &bUseColor);  

  // instantiate application object
  g_pApp= CreateFateApp();
  g_pApp->SetSystem(new CFSystem(new CFSystemImpl(uiWidth, uiHeight)));
  g_pApp->Init();
  g_pApp->InitFateApp();

  // Force redraw at startup.
  g_pApp->GetSystem()->ForceRedraw();

  return(true);
}

//------------------------------------------------------------------------------
static bool HandleEvent(EventType *pEvent)
{
  PdaKey key;

  switch(pEvent->eType) { 
    case PALMOS_REPAINT:
      g_pApp->Draw();
      // tell registered components to draw themselves
      g_pApp->DrawNotify();
      // draw the doublebuffer
      g_pApp->DrawDoubleBuffer();
    	break;

    case penDownEvent:
      // send event to registered components if application does not handle it
      if (g_pApp->StylusDown(pEvent->screenX, pEvent->screenY)) return(true);
      if (g_pApp->StylusDownNotify(pEvent->screenX, pEvent->screenY)) return(true);
      break;

    case penMoveEvent:
      // send event to registered components if application does not handle it
      if (g_pApp->StylusMove(pEvent->screenX, pEvent->screenY)) return(true);
      if (g_pApp->StylusMoveNotify(pEvent->screenX, pEvent->screenY)) return(true);
      break;

    case penUpEvent:
      // send event to registered components if application does not handle it
      if (g_pApp->StylusUp(pEvent->screenX, pEvent->screenY)) return(true);
      if (g_pApp->StylusUpNotify(pEvent->screenX, pEvent->screenY)) return(true);
      break;

    case keyDownEvent:
      // which key?
      switch(pEvent->data.keyDown.chr) {
        case hard1Chr:  // LEFT
          key= KeyWest;
          break;

        case hard4Chr:  // RIGHT
          key= KeyEast;
          break;

        case 38:  // UP      
          key= KeyNorth;
          break;

        case 40:  // DOWN
          key= KeySouth;
          break;

        case 49:  // BUTTON 1
          key= KeyOne;
          break;

        case 50:  // BUTTON 2
          key= KeyTwo;
          break;

        case 51:  // BUTTON 3
          key= KeyThree;
          break;

        case 52:  // BUTTON 4
          key= KeyFour;
          break;

        default:
          key= KeyNone;
          break;
      }
      // send event to registered components if application does not handle it
      if (key != KeyNone) {
        if (!g_pApp->KeyDown(key))
          g_pApp->KeyDownNotify(key);
      }
      // Return "true" in any case to consume all key events.
      return(true);  
      break;        

    case FATE_EVENT_ID_BUTTONPRESS:      
      // send WM_BUTTONPRESS event to registered components if application does not handle it      
      if (!g_pApp->ButtonPressed(((FateEventType*)pEvent)->data.FateEventData.ulID))
        g_pApp->ButtonPressNotify(((FateEventType*)pEvent)->data.FateEventData.ulID);
      break;

    case FATE_EVENT_ID_BUTTONRELEASE:      
      // send WM_BUTTONRELEASE event to registered components if application does not handle it
      if (!g_pApp->ButtonReleased(((FateEventType*)pEvent)->data.FateEventData.ulID))
        g_pApp->ButtonReleaseNotify(((FateEventType*)pEvent)->data.FateEventData.ulID);
      break;

    default: 
      return(false);
  }

  return(false);
}
//------------------------------------------------------------------------------
void EventLoop()
{
	Err error;
	EventType event;

	// Main event loop.
	do {		

   ///if (EvtEventAvail())

		EvtGetEvent(&event, 0);

		// Handle event
    if (!HandleEvent(&event)) {
      if (!SysHandleEvent(&event)) {
			  MenuHandleEvent(0, &event, &error);		
      }
    } 

    // FATE loop
    if (g_pApp->IsListening()) {
      g_pApp->CheckServers();
    }
    if (g_pApp->IsFateLoopEnabled()) {
      g_pApp->Idle();
      g_pApp->DrawDoubleBuffer();
    }
  } while (event.eType != appStopEvent);
}

//------------------------------------------------------------------------------
static bool NormalAppLaunch()
{
  // basic initialization
  if (!StartApplication())
    return(false);
	
  // main loop
	EventLoop();

  if (g_pApp) {
    // delete application object
    delete(g_pApp);

    int iNrOfInstances= IFateComponent::GetNumberOfInstances();
    if (iNrOfInstances != 0) {
      char szTemp[256];
      StrPrintF(szTemp, "%d instances of IFateComponent are left allocated!", iNrOfInstances);
      ErrDisplay(szTemp);
    }

    SAFE_DELETE(g_sockDebug);
    SAFE_DELETE(g_addrDebug);
  }
}

//------------------------------------------------------------------------------
UInt32 PilotMain(UInt16 cmd, void *cmdPBP, UInt16 launchFlags)
{
  switch (cmd) { 	
		case sysAppLaunchCmdNormalLaunch:
      NormalAppLaunch();
      break;

    default:
      break;
	}

	return(0);
}
