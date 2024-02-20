#include "Demo.h"

#define NR_OF_PANELS 6

#define ID_QUIT 100


CFateApp* CreateFateApp()
{
  return(new CDemo());
}

CDemo::CDemo() :
  CFateApp(),
  m_itemList(5, 150, NULL, NULL),
  m_dropList(3, 120, NULL, NULL, NULL),
  m_dirList(4, 150, NULL, NULL),
  m_progressBar(140, 30),
  m_fileList(8, 150, NULL, NULL)
{
  m_btns = new CFButton[NR_OF_PANELS];
  m_panels = new CFPanel[NR_OF_PANELS];
}

CDemo::~CDemo()
{
  delete[] m_btns;
  delete[] m_panels;
}

void CDemo::Draw()
{
  ClearScreen(RGB(0, 225, 50));
}

bool CDemo::InitFateApp()
{
  for (int i = 0; i < NR_OF_PANELS; i++)
  {
    TCHAR str[10];
    _stprintf(str, "%d", i);

    m_btns[i].SetCaption(str);
    m_btns[i].SetId(i);
    m_btns[i].SetX(i * 20);
    m_btns[i].SetY(300);
    Add(m_btns[i]);

    m_panels[i].SetVisible(i == 2);
    Add(m_panels[i]);
  }

  m_menu.AddEntry(TEXT("File"));
  m_menu.AddEntry(TEXT("Edit"));
  m_menu.AddEntry(TEXT("View"));
  m_menu.AddSubEntry(TEXT("New"),   0, 0);
  m_menu.AddSubEntry(TEXT("Open"),  0, 0);
  m_menu.AddSubEntry(TEXT("Save"),  0, 0);
  m_menu.AddSubEntry(TEXT("Close"), 0, ID_QUIT);
  m_menu.AddSubEntry(TEXT("Copy"),   1, 0);
  m_menu.AddSubEntry(TEXT("Cut"),  1, 0);
  m_menu.AddSubEntry(TEXT("Paste"),  1, 0);
  Add(m_menu); 

  // Panel 0
  m_itemList.AddItem(TEXT("entry 1"));
  m_itemList.AddItem(TEXT("entry 2"));
  m_itemList.AddItem(TEXT("entry 3"));
  m_itemList.AddItem(TEXT("entry 4"));
  m_itemList.AddItem(TEXT("entry 5"));
  m_itemList.AddItem(TEXT("entry 6"));
  m_itemList.AddItem(TEXT("entry 7"));
  m_itemList.AddItem(TEXT("entry 8"));
  m_itemList.AddItem(TEXT("entry 9"));
  m_itemList.SetX(40);
  m_itemList.SetY(80);
  m_panels[0].Add(m_itemList);

  // Panel 1
  m_dropList.AddItem(TEXT("entry 1"));
  m_dropList.AddItem(TEXT("entry 2"));
  m_dropList.AddItem(TEXT("entry 3"));
  m_dropList.AddItem(TEXT("entry 4"));
  m_dropList.AddItem(TEXT("entry 5"));
  m_dropList.AddItem(TEXT("entry 6"));
  m_dropList.AddItem(TEXT("entry 7"));
  m_dropList.AddItem(TEXT("entry 8"));
  m_dropList.AddItem(TEXT("entry 9"));
  m_dropList.SetX(10);
  m_dropList.SetY(50);
  m_panels[1].Add(m_dropList);

  // Panel 2
  m_dirList.SetX(10);
  m_dirList.SetY(50);
  m_panels[2].Add(m_dirList);

  // Panel 3
  m_panels[3].Add(m_vePanel);  

  // Panel 4
  m_label.SetX(20);
  m_label.SetY(60);
  m_label.SetText(GetFateVersion().GetLongString());
  m_panels[4].Add(m_label);
  m_progressBar.SetX(20);
  m_progressBar.SetY(120);
  m_progressBar.SetProgress(30);
  m_panels[4].Add(m_progressBar);  

  // Panel 5
  m_fileList.SetX(50);
  m_fileList.SetY(50);
  m_panels[5].Add(m_fileList);  

  return true;
}

bool CDemo::ButtonReleased(unsigned long ulBtnID)
{
  for (unsigned int i = 0; i < NR_OF_PANELS; i++)
  {
    m_panels[i].SetVisible(i == ulBtnID);
  }
  return true;
}

bool CDemo::MenuItemSelected(unsigned long ulMenuID, unsigned int id)
{
  if (id == ID_QUIT)
  {
    this->Exit();
    return true;
  }
  return false;
}
