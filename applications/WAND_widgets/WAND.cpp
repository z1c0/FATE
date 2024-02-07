// WAND.cpp: implementation of the CWAND class.
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
  m_pPanel= NULL;
}

//--------------------------------------------------------------------------------
CWAND::~CWAND()
{
  SAFE_DELETE(m_btnQuit);
  SAFE_DELETE(m_pCont);
  SAFE_DELETE(m_pPanel);
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
  m_pCont->SetEnabled(TRUE);  // now controller receives key input
  Add(*m_pCont);

  // create and init "QUIT" button
  m_btnQuit= new CFButton(TEXT("X"));
  m_btnQuit->SetVisible(TRUE);
  m_btnQuit->SetId(1000);
  m_btnQuit->SetX(200);
  Add(*m_btnQuit);

  m_pPanel= new CFWidgetPanel();
  m_pPanel->SetVisible(TRUE);
  m_pPanel->StartListening(1234);
  Add(*m_pPanel);

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Event handling method for buttons.
bool CWAND::ButtonReleased(DWORD dwBtnID)
{
  // check for ID
  if (dwBtnID == 1000) {
    Exit();
    return(TRUE);
  }

  return(FALSE);
}
