#include "FVEObjPanel.h"
#include "../base/FateApp.h"

//--------------------------------------------------------------------------------
CFVEObjPanel::CFVEObjPanel() : CFPanel(), CFVEObjManager()
{
  m_btnRefresh= new CFButton(TEXT("REFRESH"), 70, 20);
  m_btnUpdate = new CFButton(TEXT("UPDATE" ), 70, 20);
  m_btnClear  = new CFButton(TEXT("CLEAR"  ), 70, 20);
  m_btnQuit   = new CFButton(TEXT("X"   ), 20, 20);
  m_btnPlus   = new CFButton(TEXT("+"   ), 20, 20);
  m_btnMinus  = new CFButton(TEXT("-"   ), 20, 20);
  m_listObjs = new CFDropList(5, 120, NULL, NULL, NULL);
  m_listAttrs= new CFDropList(5, 80 , NULL, NULL, NULL);
  m_labelVal= new CFLabel(100, 20);
}

//--------------------------------------------------------------------------------
CFVEObjPanel::~CFVEObjPanel()
{
  SAFE_DELETE(m_listAttrs);
  SAFE_DELETE(m_listObjs);
  SAFE_DELETE(m_btnRefresh);
  SAFE_DELETE(m_btnUpdate);
  SAFE_DELETE(m_btnClear);
  SAFE_DELETE(m_btnQuit);
  SAFE_DELETE(m_btnPlus);
  SAFE_DELETE(m_btnMinus);
  SAFE_DELETE(m_labelVal);
}

//--------------------------------------------------------------------------------
void CFVEObjPanel::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;

  // now create graphical contents
  // buttons
  m_btnRefresh->SetVisible(true);
  m_btnRefresh->SetX(10);
  m_btnRefresh->SetY(50);
  m_btnRefresh->SetId(ID_VEOBJ_PANEL_BTN_REFRESH);
  Add(*m_btnRefresh);
  
  m_btnUpdate->SetVisible(true);
  m_btnUpdate->SetX(m_btnRefresh->GetX());
  m_btnUpdate->SetY(m_btnRefresh->GetBottom() + 10);
  m_btnUpdate->SetId(ID_VEOBJ_PANEL_BTN_UPDATE);
  Add(*m_btnUpdate);
  
  m_btnClear->SetVisible(true);
  m_btnClear->SetX(m_btnUpdate->GetX());
  m_btnClear->SetY(m_btnUpdate->GetBottom() + 10);
  m_btnClear->SetId(ID_VEOBJ_PANEL_BTN_CLEAR);
  Add(*m_btnClear);

  m_btnQuit->SetVisible(true);
  m_btnQuit->SetX(m_app->GetWidth() - m_btnQuit->GetWidth());
  m_btnQuit->SetY(0);
  m_btnQuit->SetId(ID_VEOBJ_PANEL_BTN_QUIT);
  Add(*m_btnQuit);
  
  // Droplists.
  m_listObjs->SetVisible(true);
  m_listObjs->SetX(m_btnRefresh->GetRight() + 10);
  m_listObjs->SetY(m_btnRefresh->GetY());
  Add(*m_listObjs);
  
  m_listAttrs->SetVisible(true);
  m_listAttrs->SetX(m_btnRefresh->GetX());
  m_listAttrs->SetY(m_btnClear->GetBottom() + 40);
  Add(*m_listAttrs);

  // Value label.
  m_labelVal->SetVisible(true);
  m_labelVal->SetX(m_listAttrs->GetRight() + 10);
  m_labelVal->SetY(m_listAttrs->GetY());
  Add(*m_labelVal);

  // + / - buttons.
  m_btnMinus->SetVisible(true);
  m_btnMinus->SetX(m_labelVal->GetX());
  m_btnMinus->SetY(m_labelVal->GetBottom() + 10);
  m_btnMinus->SetId(ID_VEOBJ_PANEL_BTN_MINUS);
  Add(*m_btnMinus);
  
  m_btnPlus->SetVisible(true);
  m_btnPlus->SetX(m_labelVal->GetRight() - m_btnPlus->GetWidth());
  m_btnPlus->SetY(m_btnMinus->GetY());
  m_btnPlus->SetId(ID_VEOBJ_PANEL_BTN_PLUS);
  Add(*m_btnPlus);
  
  /*
  /////////////////////////////////////////////////////////////
  // TEST: REMOVE!!!
  m_listObjs->AddItem(TEXT"left_arm");
  m_listObjs->AddItem("right_arm");
  m_listObjs->AddItem("head");
  m_listObjs->AddItem("wheel_1");
  m_listObjs->AddItem("wheel_2");
  m_listObjs->AddItem("wheel_3");
  m_listObjs->AddItem("wheel_4");
  m_listObjs->AddItem("wheel_5");
  m_listObjs->AddItem("wheel_6");

  m_labelVal->SetText(TEXT("12.4"));

  m_listAttrs->AddItem("POS_X");
  m_listAttrs->AddItem("POS_Y");
  m_listAttrs->AddItem("POS_Z");
  m_listAttrs->AddItem("AZIM");
  m_listAttrs->AddItem("ELEV");
  m_listAttrs->AddItem("ROLL");
  /////////////////////////////////////////////////////////////
  */
}

//--------------------------------------------------------------------------------
bool CFVEObjPanel::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_VEOBJ_PANEL_BTN_REFRESH:
      return(true);
  
    case ID_VEOBJ_PANEL_BTN_UPDATE:
      return(true);
  
    case ID_VEOBJ_PANEL_BTN_CLEAR:
      return(true);
  
    case ID_VEOBJ_PANEL_BTN_QUIT:
      m_app->Exit();
      return(true);
  
    case ID_VEOBJ_PANEL_BTN_PLUS:    
      return(true);
  
    case ID_VEOBJ_PANEL_BTN_MINUS:   
      return(true);
  
    default:
      return(false);
  }
  return(true);
}
