// FFilePanel.cpp: implementation of the CFFilePanel class.
//////////////////////////////////////////////////////////////////////
#include "FFilePanel.h"
#include "PPTRC.h"
#include "ids.h"
#include "ppt_protocol.h"
#include "resource.h"


//--------------------------------------------------------------------------------
CFFilePanel::CFFilePanel() : CFPanel()
{
  m_iMaxItems  = 10;
  m_iListIndent= 10;
  m_pszFullPath= NULL;
  m_ItemList   = NULL;
  m_btnLoad    = NULL;
  m_btnRefresh = NULL;
  m_btnDirUp   = NULL;
  m_btnHome    = NULL;
  m_fileLabel  = NULL;
}

//--------------------------------------------------------------------------------
CFFilePanel::~CFFilePanel()
{
  SAFE_DELETE(m_ItemList);
  SAFE_DELETE(m_btnLoad);
  SAFE_DELETE(m_btnRefresh);
  SAFE_DELETE(m_btnDirUp);
  SAFE_DELETE(m_btnHome);
  SAFE_DELETE(m_fileLabel);
  SAFE_DELETE(m_pszFullPath);
}

//--------------------------------------------------------------------------------
BOOL CFFilePanel::Create()
{
  // cast application pointer
  m_theApp= (CPPTRC*)m_app;

  // create the item list
  CFBitmap *bmpUp;
  CFBitmap *bmpDown;
  bmpUp= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  bmpDown= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDown->Load(IDB_DOWN_SCROLL)) return(FALSE);
  if (!bmpUp->Load(IDB_UP_SCROLL)) return(FALSE);
  m_ItemList= new CFItemList(m_iMaxItems, m_app->GetWidth() - m_iListIndent * 2 - bmpDown->GetWidth(), 
                             bmpUp, bmpDown);
  Add(*m_ItemList);
  m_ItemList->SetVisible(TRUE);
  m_ItemList->SetId(ID_LIST_PPT);   

  // create button for loading PPT presentations
  CFBitmap *bmpA= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  CFBitmap *bmpB= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpA->Load(IDB_BUTTON_LOAD)) return(FALSE);
  if (!bmpB->Load(IDB_BUTTON_LOAD_P)) return(FALSE);
  m_btnLoad= new CFButton(bmpA, bmpB);
  m_btnLoad->SetVisible(TRUE);
  m_btnLoad->SetEnabled(FALSE);
  m_btnLoad->SetId(ID_BTN_LOAD);
  Add(*m_btnLoad);
  
  // create button for back-navigation in file system
  CFBitmap *bmp1= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  CFBitmap *bmp2= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmp1->Load(IDB_BUTTON_DIR_UP)) return(FALSE);
  if (!bmp2->Load(IDB_BUTTON_DIR_UP_P)) return(FALSE);
  m_btnDirUp= new CFButton(bmp1, bmp2);
  m_btnDirUp->SetVisible(TRUE);
  m_btnDirUp->SetId(ID_BTN_DIR_UP);
  Add(*m_btnDirUp);
  
  // create button for refreshing the list of PPT presentations
  CFBitmap *bmp3= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  CFBitmap *bmp4= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmp3->Load(IDB_BUTTON_REFRESH)) return(FALSE);
  if (!bmp4->Load(IDB_BUTTON_REFRESH_P)) return(FALSE);
  m_btnRefresh= new CFButton(bmp3,bmp4);
  Add(*m_btnRefresh);
  m_btnRefresh->SetVisible(TRUE);
  m_btnRefresh->SetId(ID_BTN_REFRESH);

  // create button for navigating to home directory
  CFBitmap *bmp5= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  CFBitmap *bmp6= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmp5->Load(IDB_BUTTON_HOME)) return(FALSE);
  if (!bmp6->Load(IDB_BUTTON_HOME_P)) return(FALSE);
  m_btnHome= new CFButton(bmp5,bmp6);
  Add(*m_btnHome);
  m_btnHome->SetVisible(TRUE);
  m_btnHome->SetId(ID_BTN_HOME);

  // create label for showing file information
  m_fileLabel= new CFLabel(220, 20);
  Add(*m_fileLabel);
  m_btnLoad->SetVisible(TRUE);
  
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFFilePanel::Draw()
{
  m_fileLabel->Draw();
  m_ItemList->Draw();
}

//--------------------------------------------------------------------------------
bool CFFilePanel::StylusDown(int xPos, int yPos)
{
  return(FALSE);
}

//--------------------------------------------------------------------------------
void CFFilePanel::UpdatePos()
{
  if (m_pSystem) {
    m_ItemList->SetX(m_iListIndent);
    m_ItemList->SetY(m_iPosY + 10);

    m_btnDirUp->SetX(m_ItemList->GetX());
    m_btnDirUp->SetY(m_ItemList->GetBottom() + 5);
    
    m_btnRefresh->SetX(m_btnDirUp->GetRight() + 5);
    m_btnRefresh->SetY(m_btnDirUp->GetY());    
    
    m_btnHome->SetX(m_btnRefresh->GetRight() + 5);
    m_btnHome->SetY(m_btnRefresh->GetY());    

    m_fileLabel->SetX(m_ItemList->GetX());
    m_fileLabel->SetY(m_btnRefresh->GetBottom() + 7);

    m_btnLoad->SetX(m_ItemList->GetX());
    m_btnLoad->SetY(m_fileLabel->GetBottom() + 5);
  }
}

//--------------------------------------------------------------------------------
// Sets the entries for the item lists.
BOOL CFFilePanel::FillItemListFromServer(char *pszDir)
{
  char szMsg[256];
  
  // prepare message
  szMsg[0]= CMD_FILEINFO;
  strcpy(&szMsg[1], pszDir);
  return(m_theApp->SendToPPTHost(szMsg)); 
}

//--------------------------------------------------------------------------------
bool CFFilePanel::ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry)
{ 
  // was a file or a directory chosen?
  if (!_tcscmp(pEntry->pszAddInfo, TEXT("?"))) {
    char szDir[512];
#ifdef _WIN32_WCE
    WideCharToMultiByte(CP_ACP, 0, pEntry->pszItem, -1, szDir, 512, 0, 0);
#else
    strcpy(szDir, pEntry->pszItem);
#endif
    FillItemListFromServer(szDir);
    m_btnLoad->SetEnabled(FALSE);
  
  } else {
    m_fileLabel->SetText(pEntry->pszItem);
    if (m_pszFullPath) free(m_pszFullPath);
    m_pszFullPath= (TCHAR*)malloc((_tcslen(pEntry->pszAddInfo) + 1) * sizeof(TCHAR));
    _tcscpy(m_pszFullPath, pEntry->pszAddInfo);
    m_btnLoad->SetEnabled(TRUE);
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
bool CFFilePanel::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_BTN_REFRESH:
      // clear list
      m_ItemList->ClearItems();
      Draw();
      // request file list from powerpoint host
      if (!FillItemListFromServer(".")) {
        m_theApp->Error(TEXT("Could not reach PPT Server!"));
      }
      return(TRUE);

    case ID_BTN_DIR_UP:
      // clear list
      m_ItemList->ClearItems();
      Draw();
      // request file list from powerpoint host and go directory up
      if (!FillItemListFromServer("..")) {
        m_theApp->Error(TEXT("Could not reach PPT Server!"));
      }
      return(TRUE);

    case ID_BTN_HOME:
      // clear list
      m_ItemList->ClearItems();
      Draw();
      // request file list from powerpoint host and go directory up
      if (!FillItemListFromServer("HOME")) {
        m_theApp->Error(TEXT("Could not reach PPT Server!"));
      }
      return(TRUE);

    default:
      return(FALSE);
  }
  return(FALSE);
}