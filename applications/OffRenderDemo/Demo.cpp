#include "Demo.h"


//------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return new CDemo(DM_PORTRAIT); 
}


//------------------------------------------------------------------------------
CDemo::CDemo(EFateDrawMode DrawMode) : CFateApp(DrawMode)
{
}

//------------------------------------------------------------------------------
CDemo::~CDemo()
{
  m_offscreenRenderer.Stop();   

  delete m_pBtnQuit;
  delete m_pLabelInfo;
}

//------------------------------------------------------------------------------
bool CDemo::InitFateApp()   
{
  Add(m_offscreenRenderer); 
  m_offscreenRenderer.SetX(40); 
  m_offscreenRenderer.SetY(70);
  m_offscreenRenderer.Start();  

  // GUI
  m_pBtnQuit= new CFButton("QUIT"); 
  m_pBtnQuit->SetX(10);  
  m_pBtnQuit->SetY(10);
  Add(*m_pBtnQuit);  

  m_pLabelInfo= new CFLabel("Offscreen-Rendering Demo", 200, 30); 
  m_pLabelInfo->SetX(20);
  m_pLabelInfo->SetY(240);
  Add(*m_pLabelInfo);

  return true; 
}

//------------------------------------------------------------------------------
bool CDemo::ButtonReleased(DWORD dwBtnID)
{
  Exit();
  return true;
}

//-----------------------------------------------------------------------------
void CDemo::Draw()
{
  ClearScreen(RGB(0, 78, 152));   
}