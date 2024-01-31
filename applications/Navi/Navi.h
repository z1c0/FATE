#ifndef __NAVI__H__
#define __NAVI__H__

#include "../../framework/include/fate.h"

class CNavi : public CFateApp  
{
public:
	CNavi();
	virtual ~CNavi();

  virtual bool InitFateApp();
  virtual bool StylusDown(int xPos, int yPos);
  virtual void Draw();

private:

  CFNaviMap *m_pMap;
  CFStartMenu m_menu;
  CFController m_cont;
};

#endif // __NAVI__H__
