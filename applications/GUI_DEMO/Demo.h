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
  CFMenu m_menu;
  CFDropList m_dropList;
  CFItemList m_itemList;
  CFDirList m_dirList;
  CFVEObjPanel m_vePanel;
  CFLabel m_label;
  CFProgressBar m_progressBar;
  CFFileList m_fileList;
};

