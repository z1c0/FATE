#ifndef __DEMO__H__
#define __DEMO__H__

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
class CDemo : public CFateApp  
{
public:
	CDemo();
	virtual ~CDemo();
  virtual void Draw();
  virtual bool InitFateApp();

private:
  
  CFMenu *m_menu;
  CFItemList *m_itemlist;
  CFDropList *m_droplist;
  CFVEObjPanel *m_pVEPanel;
};

#endif  // __DEMO__H__
