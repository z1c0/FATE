#include "Demo.h"

//--------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CDemo());
}



//--------------------------------------------------------------------------------
CDemo::CDemo() : CFateApp()
{
  m_menu = NULL;
  m_droplist = NULL;
  m_itemlist = NULL;
  m_pVEPanel = NULL;
}

//--------------------------------------------------------------------------------
CDemo::~CDemo()
{
  if (m_menu) delete(m_menu);
  if (m_droplist) delete(m_droplist);
  if (m_itemlist) delete(m_itemlist);
  if (m_pVEPanel) delete(m_pVEPanel);
}

//--------------------------------------------------------------------------------
void CDemo::Draw()
{
  ClearScreen(RGB(0, 225, 50));
}

//--------------------------------------------------------------------------------
bool CDemo::InitFateApp()
{
  m_menu= new CFMenu();
  m_menu->AddEntry(TEXT("File"));
  m_menu->AddEntry(TEXT("Edit"));
  m_menu->AddEntry(TEXT("View"));
  m_menu->AddSubEntry(TEXT("New"),   0);
  m_menu->AddSubEntry(TEXT("Open"),  0);
  m_menu->AddSubEntry(TEXT("Save"),  0);
  m_menu->AddSubEntry(TEXT("Close"), 0);
  m_menu->AddSubEntry(TEXT("Copy"),   1);
  m_menu->AddSubEntry(TEXT("Cut"),  1);
  m_menu->AddSubEntry(TEXT("Paste"),  1);
  Add(*m_menu);

  m_droplist= new CFDropList(3, 150, NULL, NULL, NULL);
  m_droplist->AddItem(TEXT("entry 1"));
  m_droplist->AddItem(TEXT("entry 2"));
  m_droplist->AddItem(TEXT("entry 3"));
  m_droplist->AddItem(TEXT("entry 4"));
  m_droplist->AddItem(TEXT("entry 5"));
  m_droplist->AddItem(TEXT("entry 6"));
  m_droplist->AddItem(TEXT("entry 7"));
  m_droplist->AddItem(TEXT("entry 8"));
  m_droplist->AddItem(TEXT("entry 9"));
  m_droplist->SetY(50);
  Add(*m_droplist);

  m_itemlist= new CFItemList(3, 150, NULL, NULL);
  m_itemlist->AddItem(TEXT("entry 1"));
  m_itemlist->AddItem(TEXT("entry 2"));
  m_itemlist->AddItem(TEXT("entry 3"));
  m_itemlist->AddItem(TEXT("entry 4"));
  m_itemlist->AddItem(TEXT("entry 5"));
  m_itemlist->AddItem(TEXT("entry 6"));
  m_itemlist->AddItem(TEXT("entry 7"));
  m_itemlist->AddItem(TEXT("entry 8"));
  m_itemlist->AddItem(TEXT("entry 9"));
  m_itemlist->SetY(50);
  Add(*m_itemlist);

  //m_pVEPanel= new CFVEObjPanel();
  //Add(*m_pVEPanel);  

  return true;
}

