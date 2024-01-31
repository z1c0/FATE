#ifndef __FVEOBJPANEL__H__
#define __FVEOBJPANEL__H__

#include "FVEObjManager.h"
#include "../gui/FPanel.h"
#include "../gui/FDropList.h"
#include "../gui/FButton.h"
#include "../gui/FLabel.h"

//------------------------------------------------------------------------------
class CFLabel;

//--------------------------------------------------------------------------------
/// Constants for IDs
#define ID_VEOBJ_PANEL_BTN_REFRESH       0xA0000001
#define ID_VEOBJ_PANEL_BTN_UPDATE        0xA0000002
#define ID_VEOBJ_PANEL_BTN_CLEAR         0xA0000003
#define ID_VEOBJ_PANEL_BTN_QUIT          0xA0000004
#define ID_VEOBJ_PANEL_BTN_PLUS          0xA0000005
#define ID_VEOBJ_PANEL_BTN_MINUS         0xA0000006

//--------------------------------------------------------------------------------
class CFVEObjPanel : public CFPanel, public CFVEObjManager
{
public:
	CFVEObjPanel();
	virtual ~CFVEObjPanel();
  virtual void SetSystem(CFSystem *pSystem);
  
  virtual void Draw() {};
  
  virtual bool ButtonReleased(DWORD dwBtnID);

protected:

  CFDropList *m_listObjs;
  CFDropList *m_listAttrs;
  CFButton *m_btnRefresh;
  CFButton *m_btnUpdate;
  CFButton *m_btnClear;
  CFButton *m_btnQuit;
  CFButton *m_btnPlus;
  CFButton *m_btnMinus;
  CFLabel *m_labelVal;
};

#endif  // __FVEOBJPANEL__H__
