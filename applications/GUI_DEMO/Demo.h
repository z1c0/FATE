#pragma once

#include "../../framework/include/fate.h"

class CDemo : public CFateApp  
{
public:
	CDemo();
	virtual ~CDemo();
  virtual void Draw();
  virtual bool InitFateApp();
  virtual bool MenuItemSelected(unsigned long ulMenuID, unsigned int id);
  virtual bool ButtonReleased(unsigned long ulBtnID);

private:  
  CFButton* m_btns;
  CFPanel* m_panels;
  CFItemList m_itemList;
  CFDropList m_dropList;
  CFDirList m_dirList;
  CFProgressBar m_progressBar;
  CFFileList m_fileList;
  CFVEObjPanel m_vePanel;
  CFLabel m_label;
  CFMenu m_menu;
  CFButton m_btnAdd;
  CFButton m_btnClear;
};

