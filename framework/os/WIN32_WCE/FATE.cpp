#include "../../base/FateTypeDefs.h"
#include "../../base/FateApp.h"
#include "../../base/FSystem.h"
#include "../../base/FBitmap.h"
#include "../../base/FFile.h"
#include "../../util/FPoint.h"
#include "../../comm/FComPort.h"
#include "../../comm/FUDPSocket.h"
#include "../../util/FLinkedList.h"
#include "FSystemImpl.h"

//--------------------------------------------------------------------------------
// globals
CFateApp    *g_pApp;
CFUDPSocket *g_sockDebug= NULL;
CFInetAddr  *g_addrDebug= NULL;
CFComPort   *g_COM= NULL;
bool g_bInitOK= true;
int g_iScreenW = GetSystemMetrics(SM_CXSCREEN);
int g_iScreenH = GetSystemMetrics(SM_CYSCREEN);

//--------------------------------------------------------------------------------
// This method has to be implemented by the actual application.
// An instance of a class derived from CFateApp is expected to be returned.
extern CFateApp* CreateFateApp(); 


//--------------------------------------------------------------------------------
void __InitRemoteConsole(LPCTSTR pszHost, int iPort)
{
  if (!_tcscmp(pszHost, TEXT("COM"))) {
    // user wants a RS232 connection
    SAFE_DELETE(g_COM);
    g_COM= new CFComPort();
    g_COM->Open(iPort, BD_9600);
  
  } else {
    // we assume a UDP connection
    SAFE_DELETE(g_sockDebug);
    SAFE_DELETE(g_addrDebug);
    g_sockDebug= new CFUDPSocket();
    g_sockDebug->Create();
    g_addrDebug= new CFInetAddr(pszHost, iPort);
  }
}

//--------------------------------------------------------------------------------
void __RemoteConsole(LPCTSTR pszFormat, ...)
{
  TCHAR szBuffer[2048];
  char szSend[2048];
  va_list pArgs;
  
  // initialized UDP?
  if (g_sockDebug) {
    va_start(pArgs, pszFormat);
    _vstprintf(szBuffer, pszFormat, pArgs);
    va_end(pArgs);
    _tcscat(szBuffer, TEXT("\r\n"));

    WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, szSend, 2048, NULL, NULL);
    g_sockDebug->Send(szSend, strlen(szSend) + 1, g_addrDebug);
  }

  // initialized COM?
  if (g_COM) {
    va_start(pArgs, pszFormat);
    _vstprintf(szBuffer, pszFormat, pArgs);
    va_end(pArgs);
    _tcscat(szBuffer, TEXT("\n"));

    WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, szSend, 2048, NULL, NULL);
    g_COM->Write(szSend, strlen(szSend));
  }
}



//--------------------------------------------------------------------------------
// Primary window callback function.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  // stylus coordinates
  int iPosX= 0;
  int iPosY= 0;
  int iSwap= 0;
  PdaKey key;

  if (!g_bInitOK) return(0);

  switch (message) {
    case WM_CREATE:
      g_pApp->SetSystem(new CFSystem(new CFSystemImpl(hWnd, g_pApp->GetWidth(), g_pApp->GetHeight(), g_pApp->GetDrawMode())));
      g_pApp->Init();
      if (!g_pApp->InitFateApp()) 
      {
        PostQuitMessage(0);
        g_bInitOK = FALSE;
      }
      break;
    
    case WM_KILLFOCUS:
      GXSuspend();
      break;

    case WM_ACTIVATE:
      GXResume();
      if(wParam == WA_INACTIVE)
        SHFullScreen(hWnd, SHFS_SHOWSTARTICON|SHFS_SHOWTASKBAR|SHFS_SHOWSIPBUTTON);
      else
		SHFullScreen(hWnd, SHFS_HIDESTARTICON|SHFS_HIDETASKBAR|SHFS_HIDESIPBUTTON);
      g_pApp->ActivateFateApp();
      break;

    case WM_PAINT:
      g_pApp->Draw();
      // tell registered components to draw themselves
      g_pApp->DrawNotify();
      // draw the doublebuffer
      hdc= BeginPaint(hWnd, &ps);
      g_pApp->DrawDoubleBuffer();
      EndPaint(hWnd, &ps);
    	break;

     case WM_LBUTTONDOWN:
      iPosX= LOWORD(lParam); iPosY= HIWORD(lParam);

      // if app is not in normal draw mode, transformation of coordinates is required
      switch(g_pApp->GetDrawMode()) {

        case DM_PORTRAIT:
          // already OK, nothing to be done ...
          break;
        
        case DM_PORTRAIT_FLIPPED:
          iPosX= g_iScreenW - iPosX;
          iPosY= g_iScreenH - iPosY;
          break;

        case DM_LANDSCAPE:
          iSwap= iPosX;
          iPosX= iPosY;
          iPosY= g_iScreenH - iSwap;
          break;

        case DM_LANDSCAPE_FLIPPED:
          iSwap = iPosY;
          iPosY = iPosX;
          iPosX=  g_iScreenH - iSwap;
          break;
      }

      // send WM_LBUTTONDOWN event to registered components if application does not handle it
	  if (!g_pApp->StylusDown(iPosX, iPosY))
	  {
        g_pApp->StylusDownNotify(iPosX, iPosY);
      }
  	  break;

    case WM_MOUSEMOVE:
      iPosX= LOWORD(lParam); iPosY= HIWORD(lParam);

      // if app is not in normal draw mode, transformation of coordinates is required
      switch(g_pApp->GetDrawMode()) {

        case DM_PORTRAIT:
          // already OK, nothing to be done ...
          break;
        
        case DM_PORTRAIT_FLIPPED:
          iPosX= g_iScreenW - iPosX;
          iPosY= g_iScreenH - iPosY;
          break;

        case DM_LANDSCAPE:
          iSwap= iPosX;
          iPosX= iPosY;
          iPosY= g_iScreenH - iSwap;
          break;

        case DM_LANDSCAPE_FLIPPED:
          iSwap= iPosY;
          iPosY= iPosX;
          iPosX= g_iScreenW - iSwap;
          break;
      }

      // send WM_MOUSEMOVE event to registered components if application does not handle it
		  if (!g_pApp->StylusMove(iPosX, iPosY))
        g_pApp->StylusMoveNotify(iPosX, iPosY);
      break;

		case WM_LBUTTONUP:
      iPosX= LOWORD(lParam); iPosY= HIWORD(lParam);

      // if app is not in normal draw mode, transformation of coordinates is required
      switch(g_pApp->GetDrawMode()) {

        case DM_PORTRAIT:
          // already OK, nothing to be done ...
          break;
        
        case DM_PORTRAIT_FLIPPED:
          iPosX= g_iScreenW - iPosX;
          iPosY= g_iScreenH - iPosY;
          break;

        case DM_LANDSCAPE:
          iSwap= iPosX;
          iPosX= iPosY;
          iPosY= g_iScreenH - iSwap;
          break;

        case DM_LANDSCAPE_FLIPPED:
          iSwap= iPosY;
          iPosY= iPosX;
          iPosX= g_iScreenW - iSwap;
          break;
      }

      // send WM_LBUTTONUP event to registered components if application does not handle it
		  if (!g_pApp->StylusUp(iPosX, iPosY))
        g_pApp->StylusUpNotify(iPosX, iPosY);
			break;

    case WM_KEYDOWN:          
      // which key?
      switch(wParam) {
        case 37:  // LEFT
          key= KeyWest;
          break;

        case 39:  // RIGHT
          key= KeyEast;
          break;

        case 38:  // UP      
          key= KeyNorth;
          break;

        case 40:  // DOWN
          key= KeySouth;
          break;

        case 13:  // ENTER
          key= KeyEnter;
          break;

        case 193:  // BUTTON 1
          key= KeyOne;
          break;

        case 194:  // BUTTON 2
          key= KeyTwo;
          break;

        case 195:  // BUTTON 3
          key= KeyThree;
          break;

        case 196:  // BUTTON 4
          key= KeyFour;
          break;

        case 197:  // RECORD BUTTON 
          key= KeyRec;
          break;

        case 200:  // VOLUME UP
          key= KeyVolUp;
          break;

        case 201:  // VOLUME DOWN
          key= KeyVolDown;
          break;

        default:
          key= KeyNone;
          break;
      }
      // send WM_KEYDOWN event to registered components if application does not handle it
      if (key != KeyNone) {
        if (!g_pApp->KeyDown(key))
          g_pApp->KeyDownNotify(key);
      }
      break;

    case WM_KEYUP:
      // which key?
      switch(wParam) {
        case 37:  // LEFT
          key= KeyWest;
          break;

        case 39:  // RIGHT
          key= KeyEast;
          break;

        case 38:  // UP      
          key= KeyNorth;
          break;

        case 40:  // DOWN
          key= KeySouth;
          break;

        case 13:  // ENTER
          key= KeyEnter;
          break;

        case 193:  // BUTTON 1
          key= KeyOne;
          break;

        case 194:  // BUTTON 2
          key= KeyTwo;
          break;

        case 195:  // BUTTON 3
          key= KeyThree;
          break;

        case 196:  // BUTTON 4
          key= KeyFour;
          break;

        case 197:  // RECORD BUTTON 
          key= KeyRec;
          break;

        case 200:  // VOLUME UP
          key= KeyVolUp;
          break;

        case 201:  // VOLUME DOWN
          key= KeyVolDown;
          break;

        default:
          key= KeyNone;
          break;
      }
      
      // send WM_KEYUP event to registered components if application does not handle it
      if (key != KeyNone) {
        if (!g_pApp->KeyUp(key))
          g_pApp->KeyUpNotify(key);
      }
      break;

    case WM_CHAR:
      // send WM_CHAR event to registered components if application does not handle it
      if (!g_pApp->Char((TCHAR)wParam))
        g_pApp->CharNotify((TCHAR)wParam);
      break;

    case FATE_EVENT_ID_BUTTONPRESS:      
      // send WM_BUTTONPRESS event to registered components if application does not handle it
      if (!g_pApp->ButtonPressed((DWORD)wParam))
        g_pApp->ButtonPressNotify((DWORD)wParam);
      break;
    
    case FATE_EVENT_ID_BUTTONRELEASE:      
      // send WM_BUTTONRELEASE event to registered components if application does not handle it
      if (!g_pApp->ButtonReleased((DWORD)wParam))
        g_pApp->ButtonReleaseNotify((DWORD)wParam);
      break;

    case FATE_EVENT_ID_MENUSELECTION:        
      // send WM_MENUSELECTION event to registered components if application does not handle it
      if (!g_pApp->MenuItemSelected((DWORD)wParam, (DWORD)lParam))
	    {
        g_pApp->MenuItemSelectNotify((DWORD)wParam, (DWORD)lParam);
	    }
      break;    
   
    case FATE_EVENT_ID_ITEMLISTSELECT: 
      // send WM_ITEMLISTSELECT event to registered components if application does not handle it
      if (!g_pApp->ItemListSelected((DWORD)wParam, (ITEMLISTENTRY*)lParam))
        g_pApp->ItemListSelectNotify((DWORD)wParam, (ITEMLISTENTRY*)lParam);
      break;

    case FATE_EVENT_ID_DROPLISTEXPAND:
      // send WM_LISTBOXEXPAND event to registered components if application does not handle it
      if (!g_pApp->DropListExpanded((DWORD)wParam))
        g_pApp->DropListExpandNotify((DWORD)wParam);
      break;

    case FATE_EVENT_ID_DROPLISTSELECT: 
      // send WM_LISTBOXSELECT event to registered components if application does not handle it
      if (!g_pApp->DropListSelected((DWORD)wParam, (ITEMLISTENTRY*)lParam))
        g_pApp->DropListSelectNotify((DWORD)wParam, (ITEMLISTENTRY*)lParam);
      break;
  
    case FATE_EVENT_ID_DROPLISTCOLLAPSE: 
      // send WM_LISTBOXCOLLAPSE event to registered components if application does not handle it
      if (!g_pApp->DropListCollapsed((DWORD)wParam))
        g_pApp->DropListCollapseNotify((DWORD)wParam);
      break;

    case FATE_EVENT_ID_SLIDERCHANGE: 
      // send WM_SLIDERCHANGE to registered components if application does not handle it
      if (!g_pApp->SliderChanged((DWORD)wParam, (int)lParam))
        g_pApp->SliderChangeNotify((DWORD)wParam, (int)lParam);
      break;

    case WM_CLOSE:
      g_pApp->Exit();
      break;

    case WM_DESTROY:
	  g_pApp->CloseFateApp();
      // close GAPI
      GXCloseInput();
      GXCloseDisplay();            
      PostQuitMessage(0);
	  break;

    case WM_TIMER:
		g_pApp->Timer(static_cast<unsigned long>(wParam));
		break;

    default:
	  return DefWindowProc(hWnd, message, wParam, lParam);

  }
  return(0);
}

//--------------------------------------------------------------------------------
// Registers the window class.
ATOM RegisterWndClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;
  wc.style			  = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc	= (WNDPROC)WndProc;
  wc.cbClsExtra		= 0;
  wc.cbWndExtra		= 0;
  wc.hInstance		= hInstance;
  wc.hIcon			  = NULL;
  wc.hCursor			= 0;
  wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName	= 0;
  wc.lpszClassName= szWindowClass;

	return(RegisterClass(&wc));
}


//--------------------------------------------------------------------------------
//  FUNCTION: InitInstance(HANDLE, int)
//  PURPOSE: Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  GXDisplayProperties gxDisplay;
  TCHAR	*szTitle      = TEXT("FateApp");       // the title bar text
	TCHAR	*szWindowClass= TEXT("FateAppClass");  // the window class name

	// if application is already running give focus to window
	HWND hWnd = FindWindow(szWindowClass, szTitle);	
	if (hWnd) {
		// set focus to foremost child window
		// the "| 0x01" is used to bring any owned windows to the foreground and activate them.
    SetForegroundWindow((HWND)((ULONG)hWnd|0x00000001));
		return(0);
	} 

  // instantiate application object
  g_pApp = CreateFateApp();
  g_pApp->SetWidth(g_iScreenW);
  g_pApp->SetHeight(g_iScreenH);
  
  RegisterWndClass(hInstance, szWindowClass);

  hWnd= CreateWindow(szWindowClass, szTitle, WS_VISIBLE, 0, 0,
                     g_iScreenW, g_iScreenH, 
	                   NULL, NULL, hInstance, NULL);

  // was window created succuessfully?
  if (!hWnd) return FALSE;  
  
  // retrieve display properties
  gxDisplay = GXGetDisplayProperties();
  // make sure 16-bit color is available
	if ((gxDisplay.cBPP != 16)||(!(gxDisplay.ffFormat|kfDirect565)))
  {
    MessageBox(hWnd, TEXT("Full 16-bit color display is required!"), TEXT(""), MB_OK);
		GXCloseDisplay();
		return(FALSE);
	}
    
  // initialize the GAPI display for fullscreen mode
  GXOpenDisplay(hWnd, GX_FULLSCREEN);
  SHFullScreen(hWnd, SHFS_HIDESTARTICON|SHFS_HIDETASKBAR|SHFS_HIDESIPBUTTON);
  
  // set up buttons for exclusive control
  GXOpenInput();
  
  typedef BOOL (__stdcall *UnregisterFunc1Proc)(UINT, UINT);
  HINSTANCE hCoreDll;
  UnregisterFunc1Proc procUndergisterFunc;
  hCoreDll= LoadLibrary(_T("coredll.dll"));

  procUndergisterFunc= (UnregisterFunc1Proc)GetProcAddress(hCoreDll, _T("UnregisterFunc1"));

  for (int i=0xc1; i<=0xCF; i++) {
	  procUndergisterFunc(MOD_WIN, i);
	  RegisterHotKey(hWnd, i, MOD_WIN, i);
  }

  FreeLibrary(hCoreDll);  
  
  ShowWindow(hWnd, nCmdShow);
	
  return(TRUE);
}


//--------------------------------------------------------------------------------
// Entry point of the application.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					         LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	// perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return(FALSE);
	}	

	// main message loop:
  while (true) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
    {
		  if (msg.message == WM_QUIT)
			  break;

      TranslateMessage(&msg);
 		  DispatchMessage(&msg);
    } 
    else 
    {
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
	}

  // free application object
  delete(g_pApp);
 
  SAFE_DELETE(g_sockDebug);
  SAFE_DELETE(g_addrDebug);
  SAFE_DELETE(g_COM);

  return(msg.wParam);
}


