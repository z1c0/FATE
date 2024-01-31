#include "stdafx.h"
#include "PPTControl.h"


//--------------------------------------------------------------------------------
CPPTControl::CPPTControl(HINSTANCE hInstance)
{
  OleInitialize(NULL);

  strcpy(m_szWndClassName, "PPTRemoting");  
  RegisterWindowClass(hInstance);
  
  // first make sure that no instances of powerpoint are running
  KillAllInstancesOfPPT();

  // launch powerpoint
  //RunPPT();

  // auxiliary flag
  m_bFirst= true;

  m_szFileName[0]= 0;
}

//--------------------------------------------------------------------------------
CPPTControl::~CPPTControl()
{
  ClosePPT();
}

//--------------------------------------------------------------------------------
void CPPTControl::DrawMask(bool bDraw)
{
  // display mask windows
  for (int i=0; i<m_iNrOfWnds; i++) {
    if (bDraw) {
      SetWindowPos(m_hWnd[i], HWND_TOPMOST, 0, 0, 0, 0, 
                   SWP_SHOWWINDOW|SWP_NOSIZE|SWP_NOMOVE);
    } else {
      ShowWindow(m_hWnd[i], SW_HIDE);
    }

    UpdateWindow(m_hWnd[i]);
  }
}

//------------------------------------------------------------------------------
bool CPPTControl::IsThisFileAlreadyPlaying(const char *pszFileName)
{
  return(!strcmp(pszFileName, m_szFileName));
}

//--------------------------------------------------------------------------------
/// Opens a specified slideshow in presentation mode.
bool CPPTControl::RunSlideShow(const char *pszFileName)
{
  Presentations presentations;
  SlideShowSettings settings;
  SlideShowWindow ssWindow;
  COleDispatchDriver lpDispatch;
  float fX, fY, fW, fH;

  // clear name of slideshow that is currently played
  m_szFileName[0]= 0;

  if (m_bFirst) {
    m_appPPT.SetVisible(TRUE);
    m_bFirst= false;
  }

  // display just a black screen
  SetWindowPos(m_hWndFullMask, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);

  presentations= m_appPPT.GetPresentations();
  try {
    m_pres= presentations.Open(pszFileName, TRUE, TRUE, TRUE);
  
  } catch(...) {
    // file could not be found/opened!
    return(false);
  }

  // draw masking rectangles
  DrawMask(true);
    
  // run the slide show  
  settings= m_pres.GetSlideShowSettings();
  lpDispatch= settings.Run();  

  // set dimensions
  ssWindow= m_pres.GetSlideShowWindow();

  fX= (float)(m_iPosX)   / 1.332f;
  fY= (float)(m_iPosY)   / 1.332f;
  fW= (float)(m_iWidth)  / 1.332f;
  fH= (float)(m_iHeight) / 1.332f;
  
  ssWindow.SetLeft(fX);
  ssWindow.SetTop(fY);
  ssWindow.SetWidth(fW);
  ssWindow.SetHeight(fH);

  /*
  // For testing purposes
  HWND hWndPPT= FindWindow("screenClass", NULL);
  HWND hWndSlideShow= FindWindowEx(hWndPPT, NULL, "paneClassDC", "");
  RECT rect;
  GetWindowRect(hWndSlideShow, &rect);
  TRACE("###############################################\n");
  TRACE("WIDTH - desired: %d, actual: %d, ratio: %f\n", m_iWidth, rect.right, (float)rect.right / (float)m_iWidth);
  TRACE("HEIGHT - desired: %d, actual: %d, ratio: %f\n", m_iHeight, rect.bottom, (float)rect.bottom / (float)m_iHeight);
  TRACE("###############################################\n");
  */
  
  m_view= ssWindow.GetView();
  lpDispatch.ReleaseDispatch();
  ssWindow.ReleaseDispatch();
  settings.ReleaseDispatch();  
  
  ShowWindow(m_hWndFullMask, SW_HIDE);
  
  // good so far ... set name of slideshow that is currently played
  strcpy(m_szFileName, pszFileName);
  
  return(true);
}

//--------------------------------------------------------------------------------
bool CPPTControl::StopSlideShow()
{
  if (OpenSlideShowExists()) {
    HWND hWndPPT;

    // clear name of file that is currently played
    m_szFileName[0]= 0;

    // display just a black screen
    SetWindowPos(m_hWndFullMask, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
    // hide mask windows
    DrawMask(false);

    if (m_view.m_lpDispatch != NULL) {
      m_view.Exit();
      m_view.ReleaseDispatch();
    }
    
    try {
      m_pres.Close();
      m_pres.ReleaseDispatch();    
    
    } catch (CException *e) {
      m_pres.ReleaseDispatch();    
      e->Delete();
    }    

    // hide powerpoint application window
    hWndPPT= FindWindow("PP10FrameClass", NULL);
    ShowWindow(hWndPPT, SW_HIDE);

    // hide black screen again
    ShowWindow(m_hWndFullMask, SW_HIDE);
    
    return(true);
  }
  
  return(false);
}

//--------------------------------------------------------------------------------
/// PRIVATE METHODS
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
bool CPPTControl::RunPPT()
{
  IUnknown *pUnk;
  COleException e;
  CLSID clsid;
  HRESULT hr;

  // translate server ProgID into a CLSID
  CLSIDFromProgID(L"Powerpoint.Application", &clsid);  

  // get an interface to the running instance, if any..
  hr= GetActiveObject(clsid, NULL, (IUnknown**)&pUnk);
  if (!FAILED(hr)) {
    SlideShowWindow ssWindow;

    // get IDispatch interface for Automation...
    pUnk->QueryInterface(IID_IDispatch, (void **)&m_appPPT);
    // release the no-longer-needed IUnknown...
    pUnk->Release();

    // get handle to active presentation if there is any
    m_pres= m_appPPT.GetActivePresentation();
    
    SlideShowSettings settings= m_pres.GetSlideShowSettings();
    COleDispatchDriver lpDispatch= settings.Run();  
    ssWindow= m_pres.GetSlideShowWindow();

  } else if(m_appPPT.CreateDispatch("Powerpoint.Application", &e)) {
    return(true);  
  }
  
  return(false);
}

//--------------------------------------------------------------------------------
bool CPPTControl::ClosePPT()
{
  m_appPPT.Quit();
  m_appPPT.ReleaseDispatch();
  
  return(true);
}

//--------------------------------------------------------------------------------
/// Determines, wether there is an open slideshow or not.
bool CPPTControl::OpenSlideShowExists()
{
  if (m_view.m_lpDispatch != NULL) {
    try {      
      m_view.GetSlideShowName();
    
    } catch(COleDispatchException *e) {
      m_view.ReleaseDispatch();
      e->Delete();
    }
  }  
  return(m_view.m_lpDispatch != NULL);
}

//--------------------------------------------------------------------------------
bool CPPTControl::CreateMask(const RECT* pRects, int iNrOfRects)
{
  m_iNrOfWnds= iNrOfRects;

  // create fullscreen mask
  m_hWndFullMask= CreateWindowEx(WS_EX_TOOLWINDOW, m_szWndClassName, "", WS_POPUP,
                                 //0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                                 0, 0, 1024, 768,
                                 NULL, NULL, m_hInstance, NULL);

  for (int i=0; i<m_iNrOfWnds; i++) {
    m_hWnd[i]= CreateWindowEx(WS_EX_TOOLWINDOW, m_szWndClassName, "", WS_POPUP,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, m_hInstance, NULL);
    SetWindowPos(m_hWnd[i], HWND_TOPMOST, pRects[i].left, pRects[i].top, 
                 pRects[i].right - pRects[i].left, pRects[i].bottom - pRects[i].top, SWP_HIDEWINDOW); 
  }
  return(true);
}

//--------------------------------------------------------------------------------
void CPPTControl::SetDimensions(int iPosX, int iPosY, int iWidth, int iHeight)
{
  RECT rect[4];

  m_iPosX  = iPosX;
  m_iPosY  = iPosY;
  m_iWidth = iWidth;
  m_iHeight= iHeight;
  
  // now we can create the mask
  rect[0].left  = 0;
  rect[0].top   = 0;
  rect[0].right = m_iPosX;
  rect[0].bottom= 768;//GetSystemMetrics(SM_CYSCREEN);

  rect[1].left  = m_iPosX;
  rect[1].top   = 0;
  rect[1].right = 1024;//GetSystemMetrics(SM_CXSCREEN);
  rect[1].bottom= m_iPosY;

  rect[2].left  = m_iPosX + m_iWidth;
  rect[2].top   = 0;
  rect[2].right = 1024;//GetSystemMetrics(SM_CXSCREEN);
  rect[2].bottom= 768;//GetSystemMetrics(SM_CYSCREEN);

  rect[3].left  = m_iPosX;
  rect[3].top   = m_iPosY + m_iHeight;
  rect[3].right = 1024;//GetSystemMetrics(SM_CXSCREEN);
  rect[3].bottom= 768;//GetSystemMetrics(SM_CYSCREEN);

  CreateMask(rect, 4);
}

//--------------------------------------------------------------------------------
bool CPPTControl::KillAllInstancesOfPPT()
{
  const char *pszPPTApp= "POWERPNT.EXE";
  HINSTANCE       hInstLib = NULL;
  HANDLE          hSnapShot= NULL;
  PROCESSENTRY32  procentry;
  BOOL            bFlag;
 
  // get a handle to a Toolhelp snapshot of all processes.
  hSnapShot= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapShot == INVALID_HANDLE_VALUE) {
    FreeLibrary(hInstLib);
    return(false);
  }

  // get the first process' information.
  procentry.dwSize= sizeof(PROCESSENTRY32);
  bFlag= Process32First(hSnapShot, &procentry);

  // while there are processes, keep looping.
  while (bFlag) {            
    // did we just encounter this process?
    if (_stricmp(procentry.szExeFile, pszPPTApp) == 0) {
      // kill it ... 
      HANDLE hProc= OpenProcess(PROCESS_TERMINATE, FALSE, procentry.th32ProcessID);
      TerminateProcess(hProc, 0);
    }
       
    procentry.dwSize= sizeof(PROCESSENTRY32);
    bFlag= Process32Next(hSnapShot, &procentry);      
  }   

  return(true);
}

//--------------------------------------------------------------------------------
ATOM CPPTControl::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize       = sizeof(WNDCLASSEX); 
	wcex.style			  = CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)DefWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			  = NULL;
	wcex.hCursor		  = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName= m_szWndClassName;
	wcex.hIconSm		  = NULL;

  m_hInstance= hInstance;

	return RegisterClassEx(&wcex);
}
