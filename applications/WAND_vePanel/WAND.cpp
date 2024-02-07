#include "WAND.h"

//--------------------------------------------------------------------------------
/// "Inferface routine" for the FATE framework. Application object must be created
/// within this method and returned by it.
CFateApp* CreateFateApp()
{
  return(new CWAND());
}


//--------------------------------------------------------------------------------
CWAND::CWAND()
{
  m_btnQuit= NULL;
  m_pCont= NULL;
  m_panelVE= NULL;
}

//--------------------------------------------------------------------------------
CWAND::~CWAND()
{
  SAFE_DELETE(m_btnQuit);
  SAFE_DELETE(m_pCont);
  SAFE_DELETE(m_panelVE);
}

//--------------------------------------------------------------------------------
/// Initialization method.
bool CWAND::InitFateApp()
{
  // blue background
  ClearScreen(RGB(0, 0, 255));

  // create and init controller component
  m_pCont= new CFController(TEXT("192.168.0.1"), 9010);
  m_pCont->StartSending();
  m_pCont->SetEnabled(true);  // now controller receives key input
  Add(*m_pCont);

  // create and init VEPanel
  m_panelVE= new CFVEObjPanel();
  m_panelVE->SetVisible(true);
  Add(*m_panelVE);

  return true;
}

//--------------------------------------------------------------------------------
/// Event handling method for buttons.
bool CWAND::ButtonReleased(DWORD dwBtnID)
{
  // TODO: Check for ID.
  if (dwBtnID == 555)
  {
      CFSocket sock;
      LPCTSTR pszCapt= "BUTTON1";
      GUI_PACKET pack;
      pack.uiCmd = WIDGET_ADD;
      pack.uiKind = WIDGET_BUTTON;
      pack.uiID= 12;
      pack.uiLen= strlen(pszCapt) + 1;
      bool bRet= 0;
      int i= 0;
      
      bRet= sock.Create();
      bRet= sock.Connect("127.0.0.1", 1234);
      i= sock.Send((char*)&pack, sizeof(GUI_PACKET));
      i= sock.Send(pszCapt, strlen(pszCapt) + 1);
      sock.Close();
      return(TRUE);
  }

  return(FALSE);
}
