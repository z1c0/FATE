// Navi.cpp: implementation of the CNavi class.
//////////////////////////////////////////////////////////////////////
#include "Navi.h"

//--------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CNavi());
}

//--------------------------------------------------------------------------------
CNavi::CNavi() : CFateApp()
{
}

//--------------------------------------------------------------------------------
CNavi::~CNavi()
{
  delete m_pMap;
}

//--------------------------------------------------------------------------------
void CNavi::Draw()
{
  ClearScreen(RGB(0, 240, 0));
}

//--------------------------------------------------------------------------------
bool CNavi::InitFateApp()
{
  CFBitmap *bmpMap= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpMap->Load("map.bmp")) return(false);
  m_pMap = new CFNaviMap(bmpMap);
  m_pMap->SetX(20);
  m_pMap->SetY(20);
  m_pMap->SetWidth(200);
  m_pMap->SetHeight(200);
  Add(*m_pMap);

  m_menu.AddEntry();
  m_menu.AddEntry();
  m_menu.AddEntry();
  m_menu.AddEntry();
  m_menu.AddEntry();
  m_menu.SetX(0);
  m_menu.SetY(230);
  Add(m_menu);
  m_menu.AddEntry();
  m_menu.AddEntry();
  m_menu.AddEntry();

  m_cont.StartSending();
  Add(m_cont);

  return true;
}

//--------------------------------------------------------------------------------
bool CNavi::StylusDown(int xPos, int yPos)
{
  return false;
}
