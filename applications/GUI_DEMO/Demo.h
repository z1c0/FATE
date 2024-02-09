#pragma once

#include "../../framework/include/fate.h"

class CDemo : public CFateApp  
{
public:
	CDemo();
	virtual ~CDemo();
  virtual void Draw();
  virtual bool InitFateApp();
  virtual bool MenuSelected(unsigned long ulMenuID, int iSelMain, int iSelSub);

private:
  
  CFMenu *m_menu;
  CFItemList *m_itemlist;
  CFDropList *m_droplist;
  CFVEObjPanel *m_pVEPanel;
};

