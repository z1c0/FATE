#ifndef __FOFFSCREENRENDERER__H__
#define __FOFFSCREENRENDERER__H__

#include "../base/IFateComponent.h"
#include "../base/FBitmap.h"


//------------------------------------------------------------------------------
#define OFFSCREEN_RENDERER_RECEIVE_PORT  17012
#define OFFSCREEN_RENDER_TIMEOUT         5000  // milliseconds

//--------------------------------------------------------------------------------
class CFOffscreenRenderer : public IFateComponent  
{
  friend DWORD WINAPI ReceiveFrameThreadFunc(LPVOID pParam);

public:
	CFOffscreenRenderer();
	virtual ~CFOffscreenRenderer();

  bool Start();
  bool Stop();

  virtual void SetSystem(CFSystem *pSystem);

  virtual void Draw();

  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);

	virtual bool StylusUp(int xPos, int yPos);

protected:

  CFBitmap *m_pFrame;

  HANDLE m_hThread;
  bool m_bRun;

  bool m_bUseNoiseGenerator;
  unsigned int m_uiTimeout;
};

#endif // __FOFFSCREENRENDERER__H__
