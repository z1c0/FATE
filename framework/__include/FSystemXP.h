#ifndef __FSYSTEMXP_H__
#define __FSYSTEMXP_H__

#include <windows.h>
#include "FSystem.h"

//------------------------------------------------------------------------------
class CFSystem : public CFSystem
{
public:
  CFSystem(HWND hWnd, int iWidth, int iHeight);
  virtual ~CFSystem();

  virtual void RenderDoubleBuffer();

  virtual void ForceRedraw();

  virtual void QueueEvent(int iEventID, int iComponentID, void *pCustomData);

  virtual bool ShutDownSystem();

protected:

  HWND m_hWnd;
  HDC m_hDestDC;
};

#endif  // __FSYSTEMXP_H__
