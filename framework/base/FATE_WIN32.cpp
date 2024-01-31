#include "FateTypeDefs.h"
#include "FateApp.h"
#include "FSystem_WIN32.h"
#include "FBitmap_WIN32.h"
#include "FFile_WIN32.h"
#include "../util/FLinkedList.h"
#include "../comm/FUDPSocket_WIN32.h"


#define FATE_APP_WIDTH   240
#define FATE_APP_HEIGHT  320


//--------------------------------------------------------------------------------
/// Global variables
CFateApp  *g_pApp;
CFUDPSocket *g_sockDebug= NULL;
CFInetAddr  *g_addrDebug= NULL;
bool g_bInitOK= true;


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
  TCHAR szBuffer[2048];
  va_list pArgs;
  
  // initialized?
  if (g_sockDebug) {
    va_start(pArgs, pszFormat);
    _vstprintf(szBuffer, pszFormat, pArgs);
    va_end(pArgs);
    _tcscat(szBuffer, TEXT("\r\n"));

    g_sockDebug->Send(szBuffer, strlen(szBuffer) + 1, g_addrDebug);
  }
}

//--------------------------------------------------------------------------------
// Primary window callback function.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  PAINTSTRUCT ps;
  // Stylus coordinates.
  int iPosX= 0;
  int iPosY= 0;
  PdaKey key;

  if ((g_pApp == NULL)||(!g_bInitOK)) {
    return 0;
  }

  switch (message) {
    case WM_CREATE:
      RECT rect;
      int iTotWidth, iTotHeight;
      int appwidth;
      int appheight;
      appwidth = g_pApp->IsLandScapeMode() ? FATE_APP_HEIGHT : FATE_APP_WIDTH;
      appheight = g_pApp->IsLandScapeMode() ? FATE_APP_WIDTH : FATE_APP_HEIGHT;
      g_pApp->SetSystem(new CFSystem(hWnd, appwidth, appheight));
      g_pApp->Init();
      if (!g_pApp->InitFateApp()) {
        PostQuitMessage(0);
        g_bInitOK = false;
      }
      GetClientRect(hWnd, &rect);
      iTotWidth = g_pApp->GetWidth() + (g_pApp->GetWidth() - rect.right);
      iTotHeight= g_pApp->GetHeight() + (g_pApp->GetHeight() - rect.bottom);
      SetWindowPos(hWnd, HWND_TOP, 0, 0, iTotWidth, iTotHeight, SWP_NOMOVE);
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
      
      // send WM_LBUTTONDOWN event to registered components if application does not handle it
		  if (!g_pApp->StylusDown(iPosX, iPosY))
        g_pApp->StylusDownNotify(iPosX, iPosY);
  	  break;

    case WM_MOUSEMOVE:
      // left mouse button must be pressed
      if ((wParam & MK_LBUTTON) != MK_LBUTTON) break;

      iPosX= LOWORD(lParam); iPosY= HIWORD(lParam);

      // send WM_MOUSEMOVE event to registered components if application does not handle it
		  if (!g_pApp->StylusMove(iPosX, iPosY))
        g_pApp->StylusMoveNotify(iPosX, iPosY);
      break;

		case WM_LBUTTONUP:
      iPosX= LOWORD(lParam); iPosY= HIWORD(lParam);

      // send WM_LBUTTONUP event to registered components if application does not handle it
		  if (!g_pApp->StylusUp(iPosX, iPosY))
        g_pApp->StylusUpNotify(iPosX, iPosY);
			break;

    case WM_KEYDOWN:          
      // which key?
      switch(wParam) {
        case 37:  // LEFT
          key = KeyWest;
          break;

        case 39:  // RIGHT
          key = KeyEast;
          break;

        case 38:  // UP      
          key = KeyNorth;
          break;

        case 40:  // DOWN
          key = KeySouth;
          break;

        case 32:  // SPACE
          key = KeyOne;
          break;

        case 50:  // BUTTON "2"
          key = KeyTwo;
          break;

        case 51:  // BUTTON "3"
          key = KeyThree;
          break;

        case 52:  // BUTTON "4"
          key = KeyFour;
          break;

        default:
          key = KeyNone;
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
      
      // send WM_KEYUP event to registered components if application does not handle it
      if (key != KeyNone) {
        if (!g_pApp->KeyUp(key))
          g_pApp->KeyUpNotify(key);
      }
      break;

    case WM_CHAR:
      // send WM_CHAR event to registered components if application does not handle it
      if (!g_pApp->Char((int)wParam))
        g_pApp->CharNotify((int)wParam);
      break;

    case WM_BUTTONPRESS:      
      // send WM_BUTTONPRESS event to registered components if application does not handle it
      if (!g_pApp->ButtonPressed((DWORD)wParam))
        g_pApp->ButtonPressNotify((DWORD)wParam);
      break;
    
    case WM_BUTTONRELEASE:      
      // send WM_BUTTONRELEASE event to registered components if application does not handle it
      if (!g_pApp->ButtonReleased((DWORD)wParam))
        g_pApp->ButtonReleaseNotify((DWORD)wParam);
      break;

    case WM_MENUSELECTION:        
      // send WM_MENUSELECTION event to registered components if application does not handle it
      if (!g_pApp->MenuSelected((DWORD)wParam, LOWORD(lParam), HIWORD(lParam)))
        g_pApp->MenuSelectNotify((DWORD)wParam, LOWORD(lParam), HIWORD(lParam));
      break;    
   
    case WM_ITEMLISTSELECT: 
      // send WM_ITEMLISTSELECT event to registered components if application does not handle it
      if (!g_pApp->ItemListSelected((DWORD)wParam, (ITEMLISTENTRY*)lParam))
        g_pApp->ItemListSelectNotify((DWORD)wParam, (ITEMLISTENTRY*)lParam);
      break;

    case WM_DROPLISTEXPAND:
      // send WM_LISTBOXEXPAND event to registered components if application does not handle it
      if (!g_pApp->DropListExpanded((DWORD)wParam))
        g_pApp->DropListExpandNotify((DWORD)wParam);
      break;

    case WM_DROPLISTSELECT: 
      // send WM_LISTBOXSELECT event to registered components if application does not handle it
      if (!g_pApp->DropListSelected((DWORD)wParam, (ITEMLISTENTRY*)lParam))
        g_pApp->DropListSelectNotify((DWORD)wParam, (ITEMLISTENTRY*)lParam);
      break;
  
    case WM_DROPLISTCOLLAPSE: 
      // send WM_LISTBOXCOLLAPSE event to registered components if application does not handle it
      if (!g_pApp->DropListCollapsed((DWORD)wParam))
        g_pApp->DropListCollapseNotify((DWORD)wParam);
      break;

    case WM_SLIDERCHANGE: 
      // send WM_SLIDERCHANGE to registered components if application does not handle it
      if (!g_pApp->SliderChanged((DWORD)wParam, (int)lParam))
        g_pApp->SliderChangeNotify((DWORD)wParam, (int)lParam);
      break;

    case WM_CLIENTCONNECT:
      // send WM_SLIDERCHANGE to registered components if application does not handle it
      if (!g_pApp->ClientConnect((CFServer*)wParam))
        g_pApp->ClientConnectNotify((CFServer*)wParam);
      break;


    case WM_DESTROY:
		  g_pApp->CloseFateApp();
      PostQuitMessage(0);
		  break;

    default:
      if (g_pApp->ExtraEventHandler(message, (void*)wParam)) {
        return 0;
      }
		  return DefWindowProc(hWnd, message, wParam, lParam);

  }
  return 0;
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
  wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName	= 0;
  wc.lpszClassName= szWindowClass;

	return(RegisterClass(&wc));
}


//--------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  HWND hWnd;
  char *szTitle=       "FateApp";       // the title bar text
	char *szWindowClass= "FateAppClass";  // the window class name

  // instantiate application object
  g_pApp = CreateFateApp();
  
  RegisterWndClass(hInstance, szWindowClass);

  int width = g_pApp->IsLandScapeMode() ? FATE_APP_HEIGHT : FATE_APP_WIDTH;
  int height = g_pApp->IsLandScapeMode() ? FATE_APP_WIDTH : FATE_APP_HEIGHT;
  hWnd = CreateWindowEx(0, 
                        szWindowClass, 
                        szTitle,
                        WS_OVERLAPPEDWINDOW, //WS_POPUP,
                        500, 10, 
                        width, height,
                        NULL, NULL, hInstance, NULL);
  
  // was window created succuessfully?
  if (!hWnd) {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                  (LPTSTR)&lpMsgBuf,
                  0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION);
    // free the buffer.
    LocalFree(lpMsgBuf);
    return FALSE;  
  }
  
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
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if(msg.message == WM_QUIT) {
			  break;
      }

      TranslateMessage(&msg);
 		  DispatchMessage(&msg);
    
    } else {
      if (g_pApp->IsListening()) {
        g_pApp->CheckServers();
      }
      if (g_pApp->IsFateLoopEnabled()) {
        g_pApp->Idle();
        g_pApp->DrawDoubleBuffer();
      }
      Sleep(10);
    }
	}

  // Free application object.
  delete g_pApp;

  int iNrOfInstances = IFateComponent::GetNumberOfInstances();
  if (iNrOfInstances != 0) {
    char szTemp[256];
    sprintf(szTemp, "%d instances of IFateComponent are left allocated!", iNrOfInstances);
    MessageBox(NULL, szTemp, "Error", MB_OK);
  }

  SAFE_DELETE(g_sockDebug);
  SAFE_DELETE(g_addrDebug);

	return(msg.wParam);
}


