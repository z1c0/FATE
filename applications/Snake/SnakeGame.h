#ifndef __SNAKEGAME__H__
#define __SNAKEGAME__H__

#include "../../framework/include/fate.h"
#include "LevelCont.h"


//------------------------------------------------------------------------------
class CSnakeGame : public CFateApp
{

public:
  
  CSnakeGame();
  virtual ~CSnakeGame();

  virtual bool InitFateApp();
  virtual void Idle();

  virtual bool ButtonReleased(unsigned long ulBtnID);

private:

  CFButton m_btnquit;

  CLevelContainer m_levelcont;

};

#endif  // __SNAKEGAME__H__