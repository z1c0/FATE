// SKApp.cpp : Defines the entry point for the application.
#include <windows.h>
#include "resource.h"
#include "UDPSocket.h"

//--------------------------------------------------------------------------------
/// Constants.
#define IDC_TASKBAR_ICON          2004
#define MENU_ITEM_EXIT            0x0010
#define WM_ICON_NOTIFY            WM_USER + 23


//--------------------------------------------------------------------------------
// global Variables:
HINSTANCE g_hInst;							  
HWND g_hWnd;
HMENU g_hMenu;
HDC g_hSourceDC, g_hDestDC;
HBITMAP g_hOldBmp;
int g_iSrcLeft= 0;
int g_iSrcTop= 0;
int g_iScrWidth= 640;
int g_iSrcHeight= 480;
int g_iDstWidth= 160;
int g_iDstHeight= 120;
char* g_pBits= NULL;
CUDPSocket *g_pSendSocket= NULL;
CInetAddr *g_pAddr= NULL;

//--------------------------------------------------------------------------------
LRESULT OnIconNotify(WPARAM wParam, LPARAM lParam)
{
  UINT uID= (UINT)wParam;  // ID assigned to taskbar icon
  
  switch(uID) {
    case IDC_TASKBAR_ICON:
      UINT uMouseMsg;
      uMouseMsg= (UINT)lParam; // mouse messages
      switch (uMouseMsg) {  
        case WM_RBUTTONDOWN:
          POINT pt;

          GetCursorPos(&pt);          
          SetForegroundWindow(g_hWnd);  
          TrackPopupMenuEx(g_hMenu, TPM_RIGHTALIGN|TPM_BOTTOMALIGN|TPM_LEFTBUTTON,
                           pt.x, pt.y, g_hWnd, NULL);
          PostMessage(g_hWnd, WM_NULL, 0, 0);
         break;

        default:
          break;

      }
      break;    
  }
  return(0);
}

//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

  switch (message) 	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

    case WM_ICON_NOTIFY:
      OnIconNotify(wParam, lParam);
      break;

    case WM_COMMAND:
      if (wParam == MENU_ITEM_EXIT)
        PostQuitMessage(0);
      break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

//--------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			  = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		  	= 0;
	wcex.hCursor  		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName= "ScreenCaptureClass";
	wcex.hIconSm		  = 0;

	return(RegisterClassEx(&wcex));
}

//--------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst= hInstance; // store instance handle in our global variable

   g_hWnd= CreateWindowEx(WS_EX_TOOLWINDOW,
                          "ScreenCaptureClass", 
                          0,
                          WS_POPUP, 
                          CW_USEDEFAULT, 
                          0, 
                          CW_USEDEFAULT,
                          0, 
                          NULL, 
                          NULL, 
                          hInstance,
                          NULL);

   if (!g_hWnd) {
     MessageBox(g_hWnd, "Could not create window!", "ERROR", MB_OK);
     return(FALSE);
   }   

   ShowWindow(g_hWnd, SW_SHOW); 
   
   return(TRUE);
}


//--------------------------------------------------------------------------------
void CaptureScreenAndSend()
{
  StretchBlt(g_hDestDC, 0, 0, g_iDstWidth, g_iDstHeight, 
             g_hSourceDC, g_iSrcLeft, g_iSrcTop, g_iScrWidth, g_iSrcHeight, SRCCOPY);
  
  g_pSendSocket->Send(g_pBits, g_iDstWidth*g_iDstHeight*3, g_pAddr);
}


//--------------------------------------------------------------------------------
void AddTaskbarIcon()
{
   DWORD dwMessage= NIM_ADD;
   NOTIFYICONDATA nid;

   nid.cbSize          = sizeof(NOTIFYICONDATA); 
   nid.hWnd            = g_hWnd;
   nid.uID             = IDC_TASKBAR_ICON; 
   nid.uFlags          = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
   nid.uCallbackMessage= WM_ICON_NOTIFY; 
   nid.hIcon           = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
   lstrcpyn(nid.szTip, "ScreenCapture", sizeof(nid.szTip));

   ::Shell_NotifyIcon(dwMessage, &nid);
}

//--------------------------------------------------------------------------------
void RemoveTaskbarIcon()
{
  // remove the taskbar icon
  DWORD dwMessage= NIM_DELETE;
  NOTIFYICONDATA nid;

  nid.cbSize= sizeof(NOTIFYICONDATA);
  nid.uID   = IDC_TASKBAR_ICON;
  nid.hWnd  = g_hWnd;

  ::Shell_NotifyIcon(dwMessage, &nid);
}

//------------------------------------------------------------------------------
bool InitNetwork()
{
  g_pSendSocket= new CUDPSocket();  
  g_pSendSocket->Create();
  //g_pSendSocket->EnableBroadcast();
  //g_pAddr= new CInetAddr("192.168.0.3", 17012);
  g_pAddr= new CInetAddr("127.0.0.1", 17012);
  
  return(true);
}

//------------------------------------------------------------------------------
bool CreateOffscreenBitmap()
{
  g_hSourceDC= GetDC(NULL);
  g_hDestDC= CreateCompatibleDC(g_hSourceDC);
  SetStretchBltMode(g_hDestDC, HALFTONE);

  // fill bitmapinfo structure
  BITMAPINFO bmi= {0};
  bmi.bmiHeader.biBitCount     = 24;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biClrUsed      = 0;
  bmi.bmiHeader.biCompression  = 0;
  bmi.bmiHeader.biHeight       = g_iDstHeight;
  bmi.bmiHeader.biPlanes       = 1;
  bmi.bmiHeader.biSize         = 40;
  bmi.bmiHeader.biSizeImage    = g_iDstHeight * g_iDstWidth * 3;
  bmi.bmiHeader.biWidth        = g_iDstWidth;
  bmi.bmiHeader.biXPelsPerMeter= 3780;
  bmi.bmiHeader.biYPelsPerMeter= 3780;
  bmi.bmiColors[0].rgbBlue     = 0;
  bmi.bmiColors[0].rgbGreen    = 0;
  bmi.bmiColors[0].rgbRed      = 0;
  bmi.bmiColors[0].rgbReserved = 0;   

  HBITMAP hBmp= CreateDIBSection(g_hDestDC, &bmi, DIB_RGB_COLORS, (void**)&g_pBits, NULL, 0);	
  g_hOldBmp= (HBITMAP)SelectObject(g_hDestDC, hBmp);
  
  return(true);
}

//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	
  MyRegisterClass(hInstance);

	// perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) return(0);

  AddTaskbarIcon();
  // create the popup-menu
  g_hMenu= CreatePopupMenu();
  InsertMenu(g_hMenu, 0, MF_BYPOSITION, MENU_ITEM_EXIT, "Exit");

  InitNetwork();
  CreateOffscreenBitmap();

  // main message loop:
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  // run till completed
  while (msg.message != WM_QUIT) {
    // is there a message to process?
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      // dispatch the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    
    } else {
      // check for incoming TCP  data
      CaptureScreenAndSend();
      Sleep(10); // don't consume whole CPU time
    }
  }
  
  // finished
  RemoveTaskbarIcon();
  DestroyMenu(g_hMenu);
  WSACleanup();
  
  return(1);
}

