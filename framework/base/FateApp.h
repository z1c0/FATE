#pragma once

#include "IFateContainer.h"
#include "FBitmap.h"
#include "../FateVersion.h"

class CFServer;
class CFMsgPanel;
class CFSystem;


//--------------------------------------------------------------------------------
// Window messages for exchanging information about events triggered by user
// interface controls.
#define WM_BUTTONPRESS           0xBE01
#define WM_BUTTONRELEASE         0xBE02
#define WM_MENUSELECTION         0xBE03
#define WM_ITEMLISTSELECT        0xBE04
#define WM_DROPLISTEXPAND        0xBE05
#define WM_DROPLISTSELECT        0xBE06
#define WM_DROPLISTCOLLAPSE      0xBE07
#define WM_SLIDERCHANGE          0xBE08
#define WM_CLIENTCONNECT         0xBE09



//--------------------------------------------------------------------------------
// Constant specifying no transparency.
#define COL_NO_TRANSPARENCY      0xFF000000


//--------------------------------------------------------------------------------
/// Utility functions for debug output via UDP datagrams
#ifdef _REMOTE_CONSOLE
  #define INIT_REMOTE_CONSOLE  __InitRemoteConsole
#else
  #define INIT_REMOTE_CONSOLE
#endif
#ifdef _REMOTE_CONSOLE
  #define REMOTE_CONSOLE        __RemoteConsole
#else
  #define REMOTE_CONSOLE 
#endif

void __InitRemoteConsole(LPCTSTR pszHost, int iPort);
void __RemoteConsole(LPCTSTR pszFormat, ...);


//--------------------------------------------------------------------------------
class CFateApp : public IFateContainer
{
  friend class IFateContainer;
  friend class CFButton;
  friend class CFMenu;
  friend class CFDropList;
  friend class CFKeyBoard;
  friend class CFOffscreenRenderer;

public:
  
  // Structure containing info about dragging operation.
  struct DRAGGINGINFO
  {
    CFBitmap *pBmpDrag;   // visual representation of drag operation
    TCHAR *pszDesc;       // textual description
    int iOffsX;           // dragging offsets
    int iOffsY;
    int iOrigX;           // drag origin
    int iOrigY;
    void *pReserved1;    // additional user purpose data
    void *pReserved2;    // additional user purpose data
    DWORD dwReserved1;
    DWORD dwReserved2;
  };

	CFateApp(EFateDrawMode DrawMode = DM_PORTRAIT);
	virtual ~CFateApp();

  bool Init();
  const CFateVersion& GetFateVersion() const { return m_fateVersion; }
  
  // Methods that can be overridden by subclasses.
  virtual bool InitFateApp()     { return(true); };
  virtual bool ActivateFateApp() { return(true); };
  virtual bool CloseFateApp()    { return(true); };
  
  // UI event handling methods are implemented empty
  virtual bool ButtonPressed(unsigned long ulBtnID) { return(false); };
  virtual bool ButtonReleased(unsigned long ulBtnID) { return(false); };
  virtual bool MenuItemSelected(unsigned long ulMenuID, unsigned int id) { return false; };
  virtual bool ItemListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry) { return(false); };
  virtual bool DropListExpanded(unsigned long ulListID) { return(false); };
  virtual bool DropListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry) { return(false); };
  virtual bool DropListCollapsed(unsigned long ulListID) { return(false); };
  virtual bool SliderChanged(unsigned long ulSliderID, int iVal) { return(false); };
  virtual bool Char(TCHAR chChar) { return(false); };
  virtual bool ClientConnect(class CFServer* pServer) { return(false); };
  virtual bool Timer(unsigned long id) { return false; };

  virtual void SetSystem(CFSystem *pSystem);
  virtual CFSystem* GetSystem();
  
  virtual void Idle() {};  // Supposed to be overridden.
  
  void DrawDoubleBuffer();

  EFateDrawMode GetDrawMode() const { return m_DrawMode; };
  bool IsLandScapeMode() const { return ((m_DrawMode == DM_LANDSCAPE)||(m_DrawMode == DM_LANDSCAPE_FLIPPED)); };
  
  void EnableFateLoop(bool bEnabled) { m_bFateLoopEnabled= bEnabled; };
  bool IsFateLoopEnabled() { return(m_bFateLoopEnabled); };
  
  bool IsDragMode() { return(m_bDragMode); };
  void StartDragMode() { m_bDragMode= true; };
  void StartDragMode(CFBitmap *pBmpDrag, int iPosX, int iPosY);
  void DoDrag(int iPosX, int iPosY);
  void DoDragX(int iPosX, int iMinX, int iMaxX);
  void DoDragY(int iPosY, int iMinY, int iMaxY);
  void StopDragMode() { m_di.pBmpDrag->RestoreUnder(); m_bDragMode= false; };
  void SetDragInfo(const DRAGGINGINFO di) { m_di= di; };
  DRAGGINGINFO* GetDragInfo() { return(&m_di); };
  
  void SetCaptureComp(IFateComponent *pCapt) { if (!m_pCapt) m_pCapt= pCapt; };
  IFateComponent *GetCaptureComp() { return (m_pCapt); };
  void ReleaseCaptureComp() { m_pCapt= NULL; };  
  
  TCHAR* GetAppPath() { return(m_szAppPath); };
  
  bool ClearScreen(COLORREF colClear= RGB(255, 255, 255));
  
  unsigned long GetNextID() { return m_ulNextID++; };
  
  void SetDrawMode(EFateDrawMode DrawMode) { m_DrawMode = DrawMode; };
  
  void Message(LPCTSTR pszMsg);
  
  void SetTransparency(COLORREF colTrans) { m_colTrans= colTrans; };
  void NoTransparency() { m_colTrans= COL_NO_TRANSPARENCY; };
  
  bool Exit();
  
  bool SaveDoubleBuffer(LPCTSTR pszFileName);
  
  void AddServer(int iPort);
  inline bool IsListening() { return(m_bIsListening); };
  void CheckServers();
 
  static CFateApp* GetApp() { return m_pApp; };

protected:

  unsigned long m_ulNextID; // next free ID for control
  
  bool m_bDragMode;
  DRAGGINGINFO m_di;
  bool m_bFateLoopEnabled;
  IFateComponent *m_pSysCapt;  
  CFMsgPanel *m_panelMsg;

  CFBitmap *m_pDblBuffer;

private:	
  static CFateApp *m_pApp;
  
  CFateVersion m_fateVersion;
  EFateDrawMode m_DrawMode;  
  IFateComponent *m_pCapt;
  TCHAR m_szAppPath[MAX_PATH];
  COLORREF m_colTrans;
  bool m_bIsListening;
  CFLinkedList<CFServer*> m_ListServers;
};

