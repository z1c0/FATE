#include "FOffscreenRenderer.h"
#include "../base/FateApp.h"
#include "../comm/FUDPSocket.h"


//------------------------------------------------------------------------------
DWORD WINAPI ReceiveFrameThreadFunc(LPVOID pParam)
{
  // cast back parameter
  CFOffscreenRenderer *pOffscreenRenderer= (CFOffscreenRenderer*)pParam;
  CFUDPSocket sock;
  int iWidth = 160;
  int iHeight= 120;
  int iSize  = iWidth*iHeight*3;
  CFBitmap *pFrame= NULL;
  char *pBits= NULL;
  unsigned int uiLast, uiNow;

  // create offscreen bitmap
  pFrame= pOffscreenRenderer->m_pFrame;
  pFrame->Create(iWidth, iHeight);
  pFrame->SolidFill(0);
  // get a pointer to the bitmaps bits
  pBits= pFrame->GetBits();

  // start listening for frames
  sock.Create();
  sock.Bind(OFFSCREEN_RENDERER_RECEIVE_PORT);
  sock.SetTimeout(NO_TIMEOUT);

  // save current time
  uiLast= GetTickCount();

  while (pOffscreenRenderer->m_bRun) {
    if ((pOffscreenRenderer->m_pSystem)&&(sock.IsReceiving())) {
      // we received something increase timeout
      pOffscreenRenderer->m_uiTimeout= OFFSCREEN_RENDER_TIMEOUT;
      uiLast= uiNow;
      sock.Receive(pBits, iSize);
    
    } else {
      // did we time out
      uiNow= GetTickCount();
      if (uiNow - uiLast > pOffscreenRenderer->m_uiTimeout) {
        // generate noise or use black screen?
        if (pOffscreenRenderer->m_bUseNoiseGenerator) {
          int i=0;
          unsigned char c;

          // set small timeout
          pOffscreenRenderer->m_uiTimeout= 0;
          uiLast= uiNow;

          for (int y=0; y<iHeight; y++) {
            for (int x=0; x<iWidth; x++) {
              c= rand();
              pBits[i+0]= c;
              pBits[i+1]= c;
              pBits[i+2]= c;
              i+= 3;
            }
          }
        
        } else {
          // just display a black screen
          pFrame->SolidFill(0);
        }
      }
    }
    pFrame->Blit();
    Sleep(2);
  }

  return(0);
}


//--------------------------------------------------------------------------------
CFOffscreenRenderer::CFOffscreenRenderer()
{
  m_bUseNoiseGenerator= true;
  m_uiTimeout= OFFSCREEN_RENDER_TIMEOUT;
  m_bRun= false;
  m_pFrame= NULL;

}

//------------------------------------------------------------------------------
CFOffscreenRenderer::~CFOffscreenRenderer()
{
  Stop();
  
  SAFE_DELETE(m_pFrame);
}

//-------------------------------------------------------------------------------- 
void CFOffscreenRenderer::SetSystem(CFSystem *pSystem)
{
  m_pSystem = pSystem;
  // Always draw directly to the application's doublebuffer!
  m_pFrame = new CFBitmap(*m_pSystem->GetDoubleBuffer());
  // Create the offscreen bitmap.
  m_pFrame->SetX(m_iPosX);
  m_pFrame->SetY(m_iPosY);
}

//------------------------------------------------------------------------------
void CFOffscreenRenderer::SetX(int iPosX)
{
  IFateComponent::SetX(iPosX);
  if (m_pFrame) 
    m_pFrame->SetX(iPosX);
}

//------------------------------------------------------------------------------
void CFOffscreenRenderer::SetY(int iPosY)
{
  IFateComponent::SetY(iPosY);
  if (m_pFrame) 
    m_pFrame->SetY(iPosY);
}

//------------------------------------------------------------------------------
void CFOffscreenRenderer::Draw()
{
  if (m_pFrame) {  
    m_pFrame->Blit();
  }
}

//------------------------------------------------------------------------------
bool CFOffscreenRenderer::Start()
{
  if ((!m_bRun)&&(m_pSystem)) {
    m_bRun = true;
    m_hThread = CreateThread(NULL, 0, ReceiveFrameThreadFunc, this, 0, NULL);
    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
bool CFOffscreenRenderer::Stop()
{
  if (m_bRun) {
    m_bRun= false;
    WaitForSingleObject(m_hThread, INFINITE);
  }
  return(true);
}

//------------------------------------------------------------------------------
bool CFOffscreenRenderer::StylusUp(int xPos, int yPos)
{
  if ((m_pFrame)&&(m_pFrame->PointInside(xPos, yPos))) {
    m_bUseNoiseGenerator= !m_bUseNoiseGenerator;
    return(true);
  }
  return(false);
}
