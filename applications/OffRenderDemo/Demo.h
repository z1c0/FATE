#ifndef __DEMO__H__
#define __DEMO__H__

#include "../../framework/include/fate.h"

//------------------------------------------------------------------------------
class CDemo : public CFateApp
{
public:

  CDemo(EFateDrawMode DrawMode);
  virtual ~CDemo();

  virtual bool InitFateApp();

  virtual bool ButtonReleased(DWORD dwBtnID);

  virtual void Draw();

private:

  CFOffscreenRenderer m_offscreenRenderer;

  CFButton *m_pBtnQuit;
  CFLabel *m_pLabelInfo; 
};

#endif  // __DEMO__H__