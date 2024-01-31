#ifndef __WAND__H__
#define __WAND__H__

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
class CWAND : public CFateApp  
{
public:
	CWAND();
	virtual ~CWAND();
  virtual bool InitFateApp();
  virtual bool ButtonReleased(DWORD dwBtnID);

private:

  CFButton *m_btnQuit;
  CFController *m_pCont;
  CFVEObjPanel *m_panelVE;
};

#endif  // __WAND__H__
