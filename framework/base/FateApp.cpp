#include "FateApp.h"
#include "../comm/FSocket.h"
#include "../comm/FServer.h"
#include "../gui/FMsgPanel.h"


//------------------------------------------------------------------------------
bool CFMsgPanel::m_bCreated = false;
CFateApp* CFateApp::m_pApp = NULL;
int IFateComponent::m_iInstanceCounter = 0;


//--------------------------------------------------------------------------------
CFateApp::CFateApp(EFateDrawMode drawMode /* = DM_PORTRAIT */)
{
  m_bDragMode        = false;
  m_app              = this;
  m_pApp             = this;  // static pointer to application object
  m_pCapt            = NULL;
  m_pSysCapt         = NULL;
  m_DrawMode         = drawMode;
  m_bFateLoopEnabled = true;
  m_ulNextID         = 0;
  m_colTrans         = COL_NO_TRANSPARENCY;
  m_bEnabled         = true;
  m_bVisible         = true;
  m_panelMsg         = NULL;  
  m_bIsListening     = false;
  m_pSystem          = NULL;
                    
  CFSystem::GetPathToApplication(m_szAppPath);
  CFSocket::InitSocketLibrary();
}

//--------------------------------------------------------------------------------
CFateApp::~CFateApp()
{
  SAFE_DELETE(m_panelMsg);
  SAFE_DELETE(m_pSystem);
  
  for (int i=0; i<m_ListServers.GetSize(); i++)
  {
    delete(m_ListServers[i]);
  }
  m_ListServers.Clear();

  CFSocket::CleanupSocketLibrary();
}

//------------------------------------------------------------------------------
void CFateApp::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
  m_iWidth = m_pSystem->GetWidth();
  m_iHeight = m_pSystem->GetHeight();
}

//------------------------------------------------------------------------------
CFSystem* CFateApp::GetSystem()
{
  return m_pSystem;
}

//--------------------------------------------------------------------------------
/// Initialization routine. Creates the double buffer and an offscren bitmap if
/// special drawing mode is set.
bool CFateApp::Init()
{
  // we MUST have a system at this point!
  if (!m_pSystem) {
    return false; 
  }

  m_pBmp = m_pDblBuffer = &m_pSystem->GetDoubleBuffer();

  // create panel for error messages
  m_panelMsg= CFMsgPanel::Create();
  m_panelMsg->SetEnabled(false);
  Add(*m_panelMsg);

  return true;
}

//--------------------------------------------------------------------------------
/// Shuts down the application.
bool CFateApp::Exit() 
{ 
  if (!m_pSystem) {
    return false; 
  }
  // relelase the main device context
  m_pSystem->ShutDownSystem();
  
  return true; 
}

//--------------------------------------------------------------------------------
/// This method is called, whenever there is no window message which has to be
/// handled. Used for blitting the double buffer.
/// This loop can be stopped using the "EnableFateLoop(bool bEnable)" method if
/// performance is needed for other tasks.
void CFateApp::DrawDoubleBuffer()
{ 
  m_pSystem->RenderDoubleBuffer();
}

//--------------------------------------------------------------------------------
/// Starts dragging operation with simple sprite and position information.
void CFateApp::StartDragMode(CFBitmap *pBmpDrag, int iPosX, int iPosY)
{
  // prepare dragging info
  m_di.pBmpDrag= pBmpDrag;
  m_di.iOrigX= pBmpDrag->GetX();
  m_di.iOrigY= pBmpDrag->GetY();
  m_di.iOffsX= iPosX - pBmpDrag->GetX();
  m_di.iOffsY= iPosY - pBmpDrag->GetY();
  m_bDragMode= true;
}

//--------------------------------------------------------------------------------
/// Moves the drag-image to the specified position.
void CFateApp::DoDrag(int iPosX, int iPosY)
{
  if (m_bDragMode) {
    m_di.pBmpDrag->RestoreUnder();
    m_di.pBmpDrag->SetX(iPosX - m_di.iOffsX);
    m_di.pBmpDrag->SetY(iPosY - m_di.iOffsY);
    m_di.pBmpDrag->SaveUnder();
    if (m_colTrans == COL_NO_TRANSPARENCY)
      m_di.pBmpDrag->Blit();
    else 
      m_di.pBmpDrag->TransBlit(m_colTrans);
  }
}

//--------------------------------------------------------------------------------
/// Moves the drag-image in X to the specified position within certain limits.
void CFateApp::DoDragX(int iPosX, int iMinX, int iMaxX)
{
  if ((iPosX - m_di.iOffsX) <= iMinX) {
    iPosX= iMinX;
  } else if ((iPosX - m_di.iOffsX) >= iMaxX) {
    iPosX= iMaxX;
  } else {
    iPosX= iPosX - m_di.iOffsX;
  }

  if (m_bDragMode) {
    m_di.pBmpDrag->RestoreUnder();
    m_di.pBmpDrag->SetX(iPosX);
    m_di.pBmpDrag->SaveUnder();
    if (m_colTrans == COL_NO_TRANSPARENCY)
      m_di.pBmpDrag->Blit();
    else 
      m_di.pBmpDrag->TransBlit(m_colTrans);
  }
}

//--------------------------------------------------------------------------------
/// Moves the drag-image in Y to the specified position within certain limits.
void CFateApp::DoDragY(int iPosY, int iMinY, int iMaxY)
{
  if ( (iPosY - m_di.iOffsY) <= iMinY) {
    iPosY = iMinY;
  } else if ((iPosY - m_di.iOffsY) >= iMaxY) {
    iPosY = iMaxY;
  } else {
    iPosY = iPosY - m_di.iOffsY;
  }

  if (m_bDragMode) {
    m_di.pBmpDrag->RestoreUnder();
    m_di.pBmpDrag->SetY(iPosY);
    m_di.pBmpDrag->SaveUnder();
    if (m_colTrans == COL_NO_TRANSPARENCY)
      m_di.pBmpDrag->Blit();
    else 
      m_di.pBmpDrag->TransBlit(m_colTrans);
  }
}

//--------------------------------------------------------------------------------
/// Fills the double-buffer image with the specified color
bool CFateApp::ClearScreen(COLORREF colClear /* = RGB(255, 255, 255) */)
{
  if (m_pDblBuffer) {
    m_pDblBuffer->SolidFill(colClear);
    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
/// Display the specified message on the screen
void CFateApp::Message(LPCTSTR pszMsg)
{
  SetCaptureComp(m_panelMsg);
  m_panelMsg->SetMsg(pszMsg);
}

//--------------------------------------------------------------------------------
/// Adds a server which listens at port "iPort".
void CFateApp::AddServer(int iPort)
{
  if (!m_bIsListening) m_bIsListening= true;

  CFServer *pServer= new CFServer();
  pServer->Listen(iPort);
  
  m_ListServers.Append(pServer);
}

//--------------------------------------------------------------------------------
/// Iterates through registered servers and calls the event handler method if 
/// there are incoming connections for a server.
void CFateApp::CheckServers()
{
  for (int i=0; i<m_ListServers.GetSize(); i++) {
    if (m_ListServers[i]->IsClientConnecting())
      m_pSystem->QueueEvent(WM_CLIENTCONNECT, 0, NULL);
  }
}

//--------------------------------------------------------------------------------  
/// Save current doublebuffer to the specified file
bool CFateApp::SaveDoubleBuffer(LPCTSTR pszFileName)
{
  return(m_pDblBuffer->SaveToFile(pszFileName));
}