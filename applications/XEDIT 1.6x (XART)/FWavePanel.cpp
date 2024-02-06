// FWavePanel.cpp: implementation of the CFWavePanel class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FWavePanel.h"
#include "Resource.h"
#include "../../framework/audio/FWaveIn.h"
#include "math.h"
#include <stdlib.h>

// buffers for audio output
#define MAX_BUFF   176400
char g_szBuff1[MAX_BUFF];
char g_szBuff2[MAX_BUFF];


//--------------------------------------------------------------------------------
// Thread function is called when playback starts.
DWORD WINAPI PlayBackThreadFunc(LPVOID pParam)
{
  // cast back parameter
  CFWavePanel *wavePanel= (CFWavePanel*)pParam;  
  CFWaveCutApp* pApp    = (CFWaveCutApp*)wavePanel->m_app;
  CFMMWave* wave        = wavePanel->GetWave();
  CFMarker *posMarker   = wavePanel->m_posMarker;
 
  MMTIME mmtime;
  mmtime.wType= TIME_SAMPLES;
  int iVal= 0;
  int iValMax= 0;

  int iBytesPerSample  = wave->GetBytesPerSample();
  const DWORD dwBlocks = 30;  // limit of blocks to examine
  DWORD dwBlockAlign   = wave->GetBlockAlign();
  DWORD dwPreviousBlock= wavePanel->m_dwPlayStartBlock;
  DWORD dwBlockCount   = wave->GetBlockCount();
  DWORD dwBlocksInBuff = wave->GetBytesPerSec() / dwBlockAlign;
  DWORD dwBlockPos     = 0;
  DWORD dwIndex        = 0;
  DWORD dwBlocksSent   = 0;  // number of blocks already sent to audio hardware 
  DWORD dwBlocksRet;         // blocks returned that can be played safely  
  DWORD dwBlocksRequest;     // number of blocks requested for filling small buffers
  DWORD dwAddJump= 0;        // variable 
  DWORD i;    
  BOOL bToggle= TRUE;
  BOOL bPlayMore= TRUE;      // send buffers to audio hardware?

  // correct number of played blocks by start position
  dwBlocksSent= wavePanel->m_dwPlayStartBlock;

  // stop drawing loop of double buffer => full performance is needed
  pApp->EnableFateLoop(FALSE);  

  CFWaveOut::MarkBufferAsDone(0);
  CFWaveOut::MarkBufferAsDone(1);
  
  while (wavePanel->m_bRunThread) {
    // calculate marker position
    CFWaveOut::GetPosition(&mmtime);
    dwBlockPos= wavePanel->m_dwPlayStartBlock + mmtime.u.sample;

    // stop if end of data is reached
    if(mmtime.u.sample >= (wavePanel->m_dwPlayLengthSize / dwBlockAlign)) {
      wavePanel->m_bRunThread = FALSE;
      dwBlockPos = wavePanel->m_dwPlayStartBlock + (wavePanel->m_dwPlayLengthSize / dwBlockAlign);    
      CFWaveOut::Reset();
      bPlayMore= FALSE;
    }

    // correct for playout jump
    if ((wavePanel->m_bJump)&&(dwBlockPos > wavePanel->m_dwJumpPos)) {
      dwBlockPos+= wavePanel->m_dwJumpBlockCount;
    }
    
    // limit blocks
    if ((dwBlockPos - dwPreviousBlock) > dwBlocks)
      dwPreviousBlock= dwBlockPos - dwBlocks;
    dwAddJump= 0;

    // send small buffers to audio hardware
    if ((bPlayMore)&&(CFWaveOut::IsBufferDone(bToggle))&&(dwBlocksSent < dwBlockCount)) {
      dwBlocksRequest= dwBlocksInBuff;

      // special treatment when playout jumps are needed
      if ((wavePanel->m_bJump)&&(dwBlocksSent < wavePanel->m_dwJumpPos)) {
        DWORD dwDiff= wavePanel->m_dwJumpPos - dwBlocksSent;
        if (dwDiff < dwBlocksInBuff) {
          dwBlocksRequest= dwDiff;
          dwAddJump= wavePanel->m_dwJumpBlockCount;
        }
      }      

      if (bToggle) {
        wave->ResetIterator();
        dwBlocksRet= wave->GetSamples(g_szBuff1, dwBlocksSent, dwBlocksRequest);
        CFWaveOut::Play(g_szBuff1, dwBlocksRet * dwBlockAlign);
        
      } else {
        wave->ResetIterator();
        dwBlocksRet= wave->GetSamples(g_szBuff2, dwBlocksSent, dwBlocksRequest);
        CFWaveOut::Play(g_szBuff2, dwBlocksRet * dwBlockAlign);
      }

      dwBlocksSent+= (dwAddJump + dwBlocksRet);
      bToggle= !bToggle;
    }

    // update level display
    for(i= dwPreviousBlock; i<dwBlockPos; i++) {
      iVal= abs((*wave)[i]);
      if (iVal > iValMax)
        iValMax= iVal;
    }
    if (iBytesPerSample == 1)   //  8 bits audio 
      pApp->ChangeLevel8Bit(iValMax);
    else                        // 16 bits audio
      pApp->ChangeLevel16Bit(iValMax);
    iValMax= 0;

    // move play marker
    if (dwPreviousBlock != dwBlockPos) {
      posMarker->RestoreUnder();
      posMarker->SetBlockPos(dwBlockPos);    
      posMarker->SaveUnder();
      posMarker->Draw();
    }
    
    dwPreviousBlock= dwBlockPos;

    wavePanel->UpdatePosLabel();
  
    pApp->DrawDoubleBuffer();  
  }

  pApp->ResetLevel();

  // enable drawing of double buffer again
  pApp->EnableFateLoop(TRUE);
  
  return(0);
}

//--------------------------------------------------------------------------------
// CFWavePanel - class members
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
CFWavePanel::CFWavePanel()
{
  // inits
  m_bFormChanged= TRUE;
  m_bCreated= FALSE;
  m_bInfo = FALSE;
  m_hFillBrush= CreateSolidBrush(RGB(255, 0, 0));
  m_colWaveBack= RGB(255, 255, 255);
  m_hWavePen= CreatePen(PS_SOLID, 1, RGB(0, 0, 50));
  m_iWidth= 320;
  m_iHeight= 160;
  m_selMarker= NULL;  // currently no marker is selected
  m_bJump= FALSE;
  m_dwJumpPos = 0;
  m_dwJumpBlockCount = 0;
  m_actionLast = ACTION_NONE;
  m_bmpSaving = NULL;
  m_bmpLoading= NULL;
  m_zoomSlider= NULL;
  m_dwOldVolume= 0;

  // create markers
  m_iMarkerWidth= 3;
  m_inMarker= new CFMarker(this);
  m_outMarker= new CFMarker(this);
  m_posMarker= new CFMarker(this);
  m_selMarker = m_posMarker;

  //zoom slider
  m_zoomSlider= new CFZoomSlider(this, m_iWidth);

  // create labels
  m_posLabel= new CFLabel(TEXT("pos:"), 120, 17);
  m_posLabel->SetVisible(TRUE);
  m_inLabel= new CFLabel(TEXT("in:"), 120, 17);
  m_inLabel->SetVisible(TRUE);
  m_outLabel= new CFLabel(TEXT("out:"), 120, 17);
  m_outLabel->SetVisible(TRUE);
  m_totLabel= new CFLabel(TEXT("total:"), 120, 17);
  m_totLabel->SetVisible(TRUE);
  m_selLabel= new CFLabel(TEXT("sel:"), 120, 17);
  m_selLabel->SetVisible(TRUE);
  m_unselLabel= new CFLabel(TEXT("unsel:"), 120, 17);
  m_unselLabel->SetVisible(TRUE);
}

//--------------------------------------------------------------------------------
CFWavePanel::~CFWavePanel()
{
  // save old volume setting
  CFWaveOut::SetVolume(m_dwOldVolume);
  // finalize CFWaveOut
  CFWaveOut::Finalize();  
  
  // delete markers
  SAFE_DELETE(m_inMarker);
  SAFE_DELETE(m_outMarker);
  SAFE_DELETE(m_posMarker);
  // delete bitmaps
  SAFE_DELETE(m_bmpWaveForm);
  SAFE_DELETE(m_bmpWaveWnd);
  SAFE_DELETE(m_bmpSaving);
  SAFE_DELETE(m_bmpLoading);
  // delete buttons
  delete(m_btnRec);
  delete(m_btnPause);
  delete(m_btnPlay);
  delete(m_btnStop);
  delete(m_btnMarkIn);
  delete(m_btnMarkOut);
  delete(m_btnPlayFrom);
  delete(m_btnPlayTill);
  delete(m_btnPlayIn);
  delete(m_btnPlayOut);
  delete(m_btnYouk);
  // delete labels
  delete(m_posLabel);
  delete(m_inLabel);
  delete(m_outLabel);
  delete(m_totLabel);
  delete(m_selLabel);
  delete(m_unselLabel);
  // delete panels
  delete(m_infoPanel);
  // delete zoomslider
  SAFE_DELETE(m_zoomSlider);
  // delete GDI stuff
  DeleteObject(m_hWavePen);
  DeleteObject(m_hFillBrush);  
}

//--------------------------------------------------------------------------------
bool CFWavePanel::StylusDown(int xPos, int yPos)
{
  if((CFWaveOut::IsPlaying())||(!m_wave.IsValid())) return (FALSE);  // if it is playing do nothing
  
  if (m_bmpWaveForm->PointInside(xPos, yPos)) {
    if ((m_posMarker->IsSet())&&(m_posMarker->PointInside(xPos, yPos))) { // start drag posMarker
      
      // clear all markers
      m_bmpWaveForm->Blit();
      m_outMarker->SaveUnder();
      m_outMarker->Draw();
      m_inMarker->SaveUnder();
      m_inMarker->Draw();
      m_posMarker->SaveUnder();
      m_posMarker->Draw();
      
      UpdateWaveLabels();
      
      // init dragging operation
      m_app->StartDragMode(m_posMarker->GetImage(), xPos, 0);
      m_selMarker= m_posMarker;
      
      return(TRUE);
    
    } else if ((m_inMarker->IsSet())&&(m_inMarker->PointInside(xPos, yPos))) { // start drag marker1
      
      // clear all markers
      m_bmpWaveForm->Blit();
      m_posMarker->SaveUnder();
      m_posMarker->Draw();
      m_outMarker->SaveUnder();
      m_outMarker->Draw();
      m_inMarker->SaveUnder();
      m_inMarker->Draw();
      
      // init dragging operation
      m_app->StartDragMode(m_inMarker->GetImage(), xPos, 0);
      m_selMarker= m_inMarker;
      
      return(TRUE);
    
    } else if ((m_outMarker->IsSet())&&(m_outMarker->PointInside(xPos, yPos))) { // start drag marker2
    
      // clear all markers
      m_bmpWaveForm->Blit();
      m_posMarker->SaveUnder();
      m_posMarker->Draw();
      m_inMarker->SaveUnder();
      m_inMarker->Draw();
      m_outMarker->SaveUnder();
      m_outMarker->Draw();
      
      // init dragging operation
      m_app->StartDragMode(m_outMarker->GetImage(), xPos, 0);
      m_selMarker= m_outMarker;
      
      return(TRUE);
    
    } else { //set posMarker and start drag
    
      // clear all markers
      m_bmpWaveForm->Blit();
      m_inMarker->SaveUnder();
      m_inMarker->Draw();
      m_outMarker->SaveUnder();
      m_outMarker->Draw();
      m_posMarker->SetPosX(xPos);
      m_posMarker->SaveUnder();
      m_posMarker->Draw();
      
      UpdateWaveLabels();
      
      // init dragging operation
      m_app->StartDragMode(m_posMarker->GetImage(), xPos, 0);
      m_selMarker= m_posMarker;
      
      return(TRUE);
    }    
  } else if (m_zoomSlider->PointInside(xPos, yPos)) {
    
    // start drag on zoomslider
    m_bmpWaveWnd->Blit(); 
    m_bmpWaveForm->Blit();
    m_inMarker->SaveUnder();
    m_inMarker->Draw();
    m_outMarker->SaveUnder();
    m_outMarker->Draw();
    m_posMarker->SaveUnder();
    m_posMarker->Draw();
    m_zoomSlider->SaveUnder();
    m_zoomSlider->Draw();
    
    // zoom area dragging operation
    m_app->StartDragMode(m_zoomSlider->GetImage(), xPos, 0);
    CFateApp::DRAGGINGINFO* lpdi = m_app->GetDragInfo();    
    
    // set flag to indicate that drag belongs to zoomarea
    lpdi->dwReserved1= 0xCAFE;
    return(TRUE);
  
  }
  
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFWavePanel::StylusMove(int xPos, int yPos)
{
  if (m_app->IsDragMode()) {
    CFateApp::DRAGGINGINFO* lpdi = m_app->GetDragInfo();    

    // zoom area drag?
    if (lpdi->dwReserved1 == 0xCAFE) {
      m_app->DoDragX(xPos, 0, m_iWidth - m_zoomSlider->GetWidth());
      return(TRUE);
    
    }  // marker drag?   
    else if ((lpdi->dwReserved1 != 0xCAFE)&&(m_bmpWaveForm->PointInside(xPos, yPos))) {
      m_app->DoDrag(xPos, 0);
      return(TRUE);    
    }
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFWavePanel::StylusUp(int xPos, int yPos)
{
  if (m_app->IsDragMode()) {
    CFateApp::DRAGGINGINFO* lpdi= m_app->GetDragInfo();    
    if (lpdi->dwReserved1 == 0xCAFE) {
    
      m_zoomSlider->SetPosX(lpdi->pBmpDrag->GetX());
      lpdi->dwReserved1= 0;
      m_app->StopDragMode();
      
      m_bFormChanged= TRUE;
      
      UpdateMarkers();
      Draw();

      return(TRUE);
    
    } else {
      
      CFMarker *markerSwap;
      
      // which marker was dragged?
      m_selMarker->SetPosX(lpdi->pBmpDrag->GetX());

      // swap markers?
      if ( (m_inMarker->GetBlockPos() > m_outMarker->GetBlockPos()) && m_inMarker->IsSet() && m_outMarker->IsSet()) {
        markerSwap= m_inMarker;
        m_inMarker= m_outMarker;
        m_outMarker= markerSwap;
      }
    }

    lpdi->dwReserved1= 0;
    m_app->StopDragMode();
    UpdateWaveLabels();
    Draw();
    return(TRUE);
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFWavePanel::Draw()
{
  // if wave form has changed draw it, else use buffered image
  // wave must be valid
  if ((m_bFormChanged)&&(m_wave.IsValid())) {    
    int iWidth = m_iWidth;
    int iHeight= m_bmpWaveForm->GetHeight();
    int iPosX  = 0;
    int iPosY   = iHeight - 1;
    int iRangeY= 0;

    // reset samples blocks iterator
    m_wave.ResetIterator();

    RECT recStart; 
    recStart.left  = 0;
    recStart.top   = 0;
    recStart.bottom= iWidth; 
    recStart.right = 1;
  
    RECT recEnd; 
    recEnd.left  = iWidth - 1;
    recEnd.top   = 0;
    recEnd.bottom= iWidth; 
    recEnd.right = iWidth;
 
    DWORD dwZoomBlockStart = m_zoomSlider->GetZoomBlockStart();
    DWORD dwZoomBlockEnd = m_zoomSlider->GetZoomBlockEnd();
    DWORD dwZoomBlockCount = dwZoomBlockEnd - dwZoomBlockStart;
      
    SHORT sValue= 0;
    SHORT sMax = 0;

    HPEN hOldPen;
    POINT *points;

  	int iTempY= 0;

    DWORD dwBlockIndex = 0;
    DWORD dwPointsToDraw = 0;
    DWORD dwStep = 0;
    float fStep;
    float fStepX;    

    int iMultiplier = 8; // iWidth * Multiplier = max number of points to draw

    if (dwZoomBlockCount > (DWORD)(iWidth*iMultiplier)) {
      dwPointsToDraw= (DWORD)(iWidth * iMultiplier); 
    } else {
      dwPointsToDraw= dwZoomBlockCount;
    }
    dwStep = dwZoomBlockCount / dwPointsToDraw;
    fStep = dwZoomBlockCount / (float)(dwPointsToDraw);
    fStepX = iWidth / (float)(dwPointsToDraw - 1);

    // reduce even more the number of audio samples examined
    if (dwStep > 20)
      dwStep = 20;
    
    // delete background
    m_bmpWaveForm->SolidFill(m_colWaveBack);

    // 8-bit or 16-bit wave?
    if (m_wave.GetBitsPerSample() == 8) 
      iRangeY= 128;
    else 
      iRangeY= 32000;

    //allocate space for the points
    dwPointsToDraw*= 2;
    points= (POINT*)malloc(dwPointsToDraw * sizeof(POINT));      

    hOldPen= (HPEN)SelectObject(m_bmpWaveForm->GetSourceDC(), m_hWavePen);
    
    // iterate through wave, from section of sections in blocks of 
    // size dwStep, finding the max value.
    for (DWORD i= 0; i < dwPointsToDraw; i+=2) {    
      sMax = 0;

      for(DWORD j=0 ; j < dwStep; j++) {  // find max value in section
        dwBlockIndex = (dwZoomBlockStart + (DWORD)(i*fStep) + j);
        sValue= m_wave[dwBlockIndex];
        if (sValue > sMax) {
          sMax = sValue;
        }
      }    

      iTempY= (int)(-iHeight * sMax / iRangeY);
		  if(iTempY >  0) iTempY=  0;//iPosY;
      if(iTempY < -iHeight) iTempY= -iHeight;

      points[i].x= (LONG)(iPosX + i * fStepX);
      points[i].y= iPosY + iTempY;	
      points[i+1].x= points[i].x;
      points[i+1].y= iPosY;
    }   

    // draw the waveform
    Polyline(m_bmpWaveForm->GetSourceDC(), points, dwPointsToDraw);	

    if (dwZoomBlockStart == 0)
      FillRect(m_bmpWaveForm->GetSourceDC(), &recStart, m_hFillBrush); 
    if (dwZoomBlockEnd == m_wave.GetBlockCount())
      FillRect(m_bmpWaveForm->GetSourceDC(), &recEnd, m_hFillBrush); 
        
    // release memory & cleanup
    free(points);
    SelectObject(m_bmpWaveForm->GetSourceDC(), hOldPen);
    m_bFormChanged= FALSE;
  }

  // blit buffered images
  m_bmpWaveWnd->Blit();
  m_bmpWaveForm->Blit();
  
  if (m_zoomSlider->GetZoomBlockCount()) {
    m_zoomSlider->Draw();
  }
  
  m_outMarker->Draw();
  m_inMarker->Draw();
  m_posMarker->SaveUnder();
  m_posMarker->Draw();
  UpdateWaveLabels();
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::Create()
{
  // initiate CFWaveOut
  CFWaveOut::Initialize();
  // save old volume setting
  CFWaveOut::GetVolume(&m_dwOldVolume);
  
  m_pApp= (CFWaveCutApp*)m_pParent;

  // create wave window
  m_bmpWaveWnd= new CFBitmap(*m_pSystem->GetDoubleBuffer());

  if (!m_bmpWaveWnd->Load(IDB_WAVEPANEL)) return(FALSE);
  
  // create bitmap for waveform
  m_bmpWaveForm= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpWaveForm->Create(m_bmpWaveWnd->GetWidth(), m_bmpWaveWnd->GetHeight() - 14)) 
    return(FALSE);
  m_bmpWaveForm->SolidFill(RGB(51, 102, 153));
  
  // load and save bitmaps
  m_bmpSaving= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpSaving->Load(IDB_SAVING)) return(FALSE);
  m_bmpLoading= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!m_bmpLoading->Load(IDB_LOADING)) return(FALSE);

  // create markers
  CFBitmap *bmpMarker1= new CFBitmap(*m_bmpWaveForm);  
  CFBitmap *bmpMarker2= new CFBitmap(*m_bmpWaveForm);  
  if (!bmpMarker1->Load(IDB_AREAMARKER)) return(FALSE);
  if (!bmpMarker2->Load(IDB_AREAMARKER)) return(FALSE);
  if (!m_inMarker->Create(bmpMarker1)) return(FALSE);  
  if (!m_outMarker->Create(bmpMarker2)) return(FALSE);
  CFBitmap *bmpMarker3= new CFBitmap(*m_bmpWaveForm);  
  if (!bmpMarker3->Load(IDB_POSMARKER)) return(FALSE);
  if (!m_posMarker->Create(bmpMarker3)) return(FALSE);

  //create zoom bar
  CFBitmap *m_bmpZoomPat = new CFBitmap(*m_bmpWaveForm);
  if (!m_bmpZoomPat->Load(IDB_ZOOMBAR)) return(FALSE);
  if (!m_zoomSlider->Create(m_bmpZoomPat)) return(FALSE);

  // create futurelab-info-insert 
  CFBitmap *bmpInfo= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpInfo->Load(IDB_XARTINFO)) return(FALSE);
  m_infoPanel= new CFInfoPanel();
  Add(*m_infoPanel);
  m_infoPanel->Create(bmpInfo);
  m_infoPanel->SetVisible(FALSE);

  // create and add buttons
  // Rec button
  CFBitmap *bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_REC)) return(FALSE);
  CFBitmap *bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_REC_P)) return(FALSE);
  m_btnRec= new CFButton(bmpDef, bmpPress);
  m_btnRec->SetId(ID_BTN_WAVE_REC);
  m_btnRec->SetVisible(TRUE);
  Add(*m_btnRec);

  // Pause button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PAUSE)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PAUSE_P)) return(FALSE);
  m_btnPause= new CFButton(bmpDef, bmpPress);
  m_btnPause->SetId(ID_BTN_WAVE_PAUSE);
  m_btnPause->SetVisible(TRUE);
  Add(*m_btnPause);
  
  // Play button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PLAY)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PLAY_P)) return(FALSE);
  m_btnPlay= new CFButton(bmpDef, bmpPress);
  m_btnPlay->SetId(ID_BTN_WAVE_PLAY);
  m_btnPlay->SetVisible(TRUE);
  Add(*m_btnPlay);
  
  // Stop button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_STOP))  return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_STOP_P)) return(FALSE);
  m_btnStop= new CFButton(bmpDef, bmpPress);
  m_btnStop->SetId(ID_BTN_WAVE_STOP);
  m_btnStop->SetVisible(TRUE);
  Add(*m_btnStop);
  
  // MarkIn button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_MARKIN)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_MARKIN_P)) return(FALSE);
  m_btnMarkIn= new CFButton(bmpDef, bmpPress);
  m_btnMarkIn->SetId(ID_BTN_WAVE_MARKIN);
  m_btnMarkIn->SetVisible(TRUE);
  Add(*m_btnMarkIn);
  
  // MarkOut button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_MARKOUT)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_MARKOUT_P)) return(FALSE);
  m_btnMarkOut= new CFButton(bmpDef, bmpPress);
  m_btnMarkOut->SetId(ID_BTN_WAVE_MARKOUT);
  m_btnMarkOut->SetVisible(TRUE);
  Add(*m_btnMarkOut);
  
  /*
  // -100 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_M100)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_M100_P)) return(FALSE);
  m_btnM100= new CFButton(bmpDef, bmpPress);
  m_btnM100->SetId(ID_BTN_WAVE_M100);
  m_btnM100->SetVisible(TRUE);
  Add(m_btnM100);
  
  // -10 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_M10)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_M10_P)) return(FALSE);
  m_btnM10= new CFButton(bmpDef, bmpPress);
  m_btnM10->SetId(ID_BTN_WAVE_M10);
  m_btnM10->SetVisible(TRUE);
  Add(m_btnM10);
  
  // -1 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_M1)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_M1_P)) return(FALSE);
  m_btnM1= new CFButton(bmpDef, bmpPress);
  m_btnM1->SetId(ID_BTN_WAVE_M1);
  m_btnM1->SetVisible(TRUE);
  Add(m_btnM1);
  
  // +1 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_P1)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_P1_P)) return(FALSE);
  m_btnP1= new CFButton(bmpDef, bmpPress);
  m_btnP1->SetId(ID_BTN_WAVE_P1);
  m_btnP1->SetVisible(TRUE);
  Add(m_btnP1);
  
  // +10 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_P10)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_P10_P)) return(FALSE);
  m_btnP10= new CFButton(bmpDef, bmpPress);
  m_btnP10->SetId(ID_BTN_WAVE_P10);
  m_btnP10->SetVisible(TRUE);
  Add(m_btnP10);
  
  // +100 button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_P100)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_P100_P)) return(FALSE);
  m_btnP100= new CFButton(bmpDef, bmpPress);
  m_btnP100->SetId(ID_BTN_WAVE_P100);
  m_btnP100->SetVisible(TRUE);
  Add(m_btnP100);
  */
  
  // PlayFrom button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PLAYFROM)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PLAYFROM_P)) return(FALSE);
  m_btnPlayFrom= new CFButton(bmpDef, bmpPress);
  m_btnPlayFrom->SetId(ID_BTN_WAVE_PLAYFROM);
  m_btnPlayFrom->SetVisible(TRUE);
  Add(*m_btnPlayFrom);
  
  // PlayTill button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PLAYTILL)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PLAYTILL_P)) return(FALSE);
  m_btnPlayTill= new CFButton(bmpDef, bmpPress);
  m_btnPlayTill->SetId(ID_BTN_WAVE_PLAYTILL);
  m_btnPlayTill->SetVisible(TRUE);
  Add(*m_btnPlayTill);
  
  // PlayInside button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PLAYIN)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PLAYIN_P)) return(FALSE);
  m_btnPlayIn= new CFButton(bmpDef, bmpPress);
  m_btnPlayIn->SetId(ID_BTN_WAVE_PLAYIN);
  m_btnPlayIn->SetVisible(TRUE);
  Add(*m_btnPlayIn);
  
  // PlayOutSide button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_PLAYOUT)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_PLAYOUT_P)) return(FALSE);
  m_btnPlayOut= new CFButton(bmpDef, bmpPress);
  m_btnPlayOut->SetId(ID_BTN_WAVE_PLAYOUT);
  m_btnPlayOut->SetVisible(TRUE);
  Add(*m_btnPlayOut);  

  // YOUK button
  bmpDef= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_BUTTON_XART)) return(FALSE);
  bmpPress= new CFBitmap(*m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_BUTTON_XART)) return(FALSE);
  m_btnYouk= new CFButton(bmpDef, bmpPress);
  m_btnYouk->SetId(ID_BTN_WAVE_YOUK);
  m_btnYouk->SetVisible(TRUE);
  Add(*m_btnYouk);  

  // add labels
  Add(*m_posLabel);
  Add(*m_inLabel);
  Add(*m_outLabel);
  Add(*m_totLabel);
  Add(*m_selLabel);
  Add(*m_unselLabel);
  m_bCreated= TRUE;
  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::SaveWave(LPCTSTR pFileName)
{
  if(m_wave.SaveAs(pFileName))
    return (TRUE);
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::CloseOldWave()
{
  if (m_wave.IsValid()) {
    if (m_wave.IsChanged()) {
      m_bmpSaving->Blit();
      m_pApp->DrawDoubleBuffer();
      m_wave.Save();
    }
    m_wave.Clear();
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::LoadWave(LPCTSTR pFileName)
{  
  // clean waveOut
  CFWaveOut::Close();
  
  // draw "loading" graphics
  m_bmpLoading->Blit();
  m_pApp->DrawDoubleBuffer();

  // load the wave file
  if (!m_wave.Load(pFileName)) {
    LPCTSTR pTemp= TEXT("ERROR: FILE NOT LOADED");
    ((CFWaveCutApp*)m_app)->SetFileLabelText(pTemp);
    m_bmpWaveForm->SolidFill(RGB(51, 102, 153));
    m_bFormChanged= TRUE; 
    return(FALSE); 
  }

  // DEMO LIMITATION
#ifdef _XEDIT_DEMO
  if (m_wave.GetWaveTime() > XEDIT_DEMO_TIME_LIMIT) {
    m_wave.Clear();
    LPCTSTR pTemp= TEXT("ERROR: DEMO LIMITATION");
    ((CFWaveCutApp*)m_app)->SetFileLabelText(pTemp);
    m_bmpWaveForm->SolidFill(RGB(51, 102, 153));
    m_bFormChanged= TRUE; 
    return(FALSE); 
  }
#endif

  // set values
  m_dwPlayStartBlock= 0;
  m_dwPlayLengthSize= m_wave.GetBlockCount() * m_wave.GetBlockAlign();

  // open device
  CFWaveOut::Open(m_wave.GetWaveFormat());
  
  // set zoom to whole wave
  m_zoomSlider->Reset(m_wave.GetBlockCount());
 
  // set tone head
  m_inMarker->Unset();
  m_outMarker->Unset();
  m_posMarker->Set();
  UpdateMarkers();

  // draw new wave
  m_bFormChanged= TRUE; 

  // display file name
  ((CFWaveCutApp*)m_app)->SetFileName(m_wave.GetFileName());
  UpdateWaveLabels();

  // clear flags
  m_actionLast = ACTION_NONE;
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFWavePanel::SetX(int iPosX)
{ 
  m_iPosX= iPosX; 
  if (m_bCreated) UpdatePos();
};

//--------------------------------------------------------------------------------
void CFWavePanel::SetY(int iPosY)
{ 
  m_iPosY= iPosY;
  if (m_bCreated) UpdatePos();
};

//--------------------------------------------------------------------------------
void CFWavePanel::UpdatePos() 
{
  m_posLabel->SetX(m_iPosX + 5);
  m_posLabel->SetY(m_iPosY);
  m_inLabel->SetX(m_iPosX + 5);
  m_inLabel->SetY(m_posLabel->GetBottom() + 2);
  m_outLabel->SetX(m_iPosX + 5);
  m_outLabel->SetY(m_inLabel->GetBottom() + 2);
  m_totLabel->SetX(m_posLabel->GetRight() + 5);
  m_totLabel->SetY(m_iPosY);
  m_selLabel->SetX(m_posLabel->GetRight() + 5);
  m_selLabel->SetY(m_totLabel->GetBottom() + 2);
  m_unselLabel->SetX(m_posLabel->GetRight() + 5);
  m_unselLabel->SetY(m_selLabel->GetBottom() + 2);

  m_bmpWaveWnd->SetX(m_iPosX);
  m_bmpWaveWnd->SetY(m_outLabel->GetBottom() + 7);
  m_bmpWaveForm->SetX(m_iPosX);
  m_bmpWaveForm->SetY(m_bmpWaveWnd->GetY() + 11);
  m_bmpSaving->SetX(m_pApp->GetWidth() / 2 - m_bmpSaving->GetWidth() / 2);
  m_bmpSaving->SetY(m_pApp->GetHeight() / 2 - m_bmpSaving->GetHeight() / 2);
  m_bmpLoading->SetX(m_bmpSaving->GetX());
  m_bmpLoading->SetY(m_bmpSaving->GetY());

  m_zoomSlider->SetPosX(0);  
  m_zoomSlider->SetPosY(m_bmpWaveWnd->GetY() + 2);  

  m_inMarker->SetPosY(m_bmpWaveWnd->GetBottom() - m_inMarker->GetHeight() - 3);
  m_outMarker->SetPosY(m_bmpWaveWnd->GetBottom() - m_inMarker->GetHeight() - 3);
  m_posMarker->SetPosY(m_bmpWaveWnd->GetBottom() - m_inMarker->GetHeight() - 3);  

  m_infoPanel->SetX(25);
  m_infoPanel->SetY(10);

  m_btnRec->SetX(m_iPosX + 1);
  m_btnRec->SetY(m_app->GetHeight() - m_btnRec->GetHeight() - 1);
  m_btnPlay->SetX(m_btnRec->GetRight() + 13);
  m_btnPlay->SetY(m_btnRec->GetY());
  m_btnPause->SetX(m_btnPlay->GetRight() + 3);
  m_btnPause->SetY(m_btnRec->GetY());
  m_btnStop->SetX(m_btnPause->GetRight() + 3);
  m_btnStop->SetY(m_btnRec->GetY());
  m_btnMarkIn->SetX(m_btnStop->GetRight() + 3);
  m_btnMarkIn->SetY(m_btnRec->GetY());
  m_btnMarkOut->SetX(m_btnMarkIn->GetRight() + 3);
  m_btnMarkOut->SetY(m_btnRec->GetY());
  
  /*
  m_btnM100->SetX(m_btnMarkOut->GetRight() + 23);
  m_btnM100->SetY(m_btnRec->GetY());
  m_btnM10->SetX(m_btnM100->GetRight() + 3);
  m_btnM10->SetY(m_btnRec->GetY());
  m_btnM1->SetX(m_btnM10->GetRight() + 3);
  m_btnM1->SetY(m_btnRec->GetY());
  m_btnP1->SetX(m_btnM1->GetRight() + 3);
  m_btnP1->SetY(m_btnRec->GetY());
  m_btnP10->SetX(m_btnP1->GetRight() + 3);
  m_btnP10->SetY(m_btnRec->GetY());
  m_btnP100->SetX(m_btnP10->GetRight() + 3);
  m_btnP100->SetY(m_btnRec->GetY());
  */
  
  m_btnPlayFrom->SetX(m_btnMarkOut->GetRight() + 13);
  m_btnPlayFrom->SetY(m_btnPlay->GetY());
  m_btnPlayTill->SetX(m_btnPlayFrom->GetRight() + 3);
  m_btnPlayTill->SetY(m_btnPlay->GetY());
  m_btnPlayIn->SetX(m_btnPlayTill->GetRight() + 3);
  m_btnPlayIn->SetY(m_btnPlay->GetY());
  m_btnPlayOut->SetX(m_btnPlayIn->GetRight() + 3);
  m_btnPlayOut->SetY(m_btnPlay->GetY());

  m_btnYouk->SetX(m_iWidth - m_btnYouk->GetWidth() - 1);
  m_btnYouk->SetY(m_btnPlay->GetY());
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::Play()
{
  if (CFWaveOut::IsPlaying()||m_bRunThread) {  
    m_bRunThread= FALSE;
    WaitForSingleObject(m_hPlayThread, INFINITE);
    CFWaveOut::Reset();
  }  
  m_bRunThread= TRUE;
  m_hPlayThread= CreateThread(NULL, 0, PlayBackThreadFunc, (LPVOID)this, 0, NULL);
  
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::Pause()
{
  if (CFWaveOut::IsPlaying() || m_bRunThread) {
    m_bRunThread= FALSE;
    WaitForSingleObject(m_hPlayThread, INFINITE);
    CFWaveOut::Reset();
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFWavePanel::Stop() 
{
  if (CFWaveOut::IsPlaying() || m_bRunThread) {
    m_bRunThread= FALSE;
    WaitForSingleObject(m_hPlayThread, INFINITE);
    CFWaveOut::Reset();
  }
  return(TRUE);
}

//--------------------------------------------------------------------------------
// Moves the tone-head.
void CFWavePanel::MoveHead(DWORD dwOffs)
{
  if ((m_posMarker->GetBlockPos() + dwOffs) > 0 && (m_posMarker->GetBlockPos() + dwOffs) < m_wave.GetBlockCount()) {
    m_posMarker->RestoreUnder();  
    m_inMarker->RestoreUnder();
    m_outMarker->RestoreUnder();
    m_inMarker->SaveUnder();
    m_outMarker->SaveUnder();
    m_inMarker->Draw();
    m_outMarker->Draw();
    m_posMarker->SetBlockPos(m_posMarker->GetBlockPos() + dwOffs);
    m_posMarker->SaveUnder();
    m_posMarker->Draw();
  }
}

//--------------------------------------------------------------------------------
// Updates markers after a zoom / cut operation.
void CFWavePanel::UpdateMarkers()
{
  m_posMarker->SetBlockPos(m_posMarker->GetBlockPos());
  m_inMarker->SetBlockPos(m_inMarker->GetBlockPos());
  m_outMarker->SetBlockPos(m_outMarker->GetBlockPos());
}

//--------------------------------------------------------------------------------
// Updates labels when something has changed.
void CFWavePanel::UpdateWaveLabels()
{
  TCHAR szBuff[128];

  _stprintf(szBuff, TEXT("total: %d.%03d"), m_wave.GetWaveTime()/1000, m_wave.GetWaveTime()%1000);
  m_totLabel->SetText(szBuff);

  if (!m_bInfo) {

    _stprintf(szBuff, TEXT("pos: %d.%03d"), m_posMarker->GetTimePos()/1000, m_posMarker->GetTimePos()%1000);
    m_posLabel->SetText(szBuff);

    // if first marker is set
    if (m_inMarker->IsSet())
      _stprintf(szBuff, TEXT("in: %d.%03d"), m_inMarker->GetTimePos()/1000, m_inMarker->GetTimePos()%1000);
    else
      _stprintf(szBuff, TEXT("in: "));
    m_inLabel->SetText(szBuff);

    // if second marker is set
    if (m_outMarker->IsSet()) 
      _stprintf(szBuff, TEXT("out: %d.%03d"), m_outMarker->GetTimePos()/1000, m_outMarker->GetTimePos()%1000);
    else   
      _stprintf(szBuff, TEXT("out: "));
    m_outLabel->SetText(szBuff);
    
    // for selection info, both markers must be set
    if ((m_inMarker->IsSet())&&(m_outMarker->IsSet())) {
      DWORD dwSelTime= m_outMarker->GetTimePos() - m_inMarker->GetTimePos();

      _stprintf(szBuff, TEXT("sel: %d.%03d"), dwSelTime/1000, dwSelTime%1000);
      m_selLabel->SetText(szBuff);
      
      _stprintf(szBuff, TEXT("unsel: %d.%03d"), (m_wave.GetWaveTime() - dwSelTime)/1000, (m_wave.GetWaveTime() - dwSelTime)%1000);
      m_unselLabel->SetText(szBuff);
    } else { 
      _stprintf(szBuff, TEXT("sel: "));
      m_selLabel->SetText(szBuff);
      
      _stprintf(szBuff, TEXT("unsel: "));
      m_unselLabel->SetText(szBuff);
    }

  } else { 
    
    MEMORYSTATUS Buffer;
    GlobalMemoryStatus(&Buffer);

    _stprintf(szBuff, TEXT("rate: %d Hz"), m_wave.GetSampleRate());
    m_posLabel->SetText(szBuff);

    _stprintf(szBuff, TEXT("bits: %d"), m_wave.GetBitsPerSample());
     m_inLabel->SetText(szBuff);

    _stprintf(szBuff, TEXT("channels: %d"), m_wave.GetChannels());
    m_outLabel->SetText(szBuff);

    _stprintf(szBuff, TEXT("free mem: %d k "), Buffer.dwAvailPhys/1024);
    m_selLabel->SetText(szBuff);
 
    _stprintf(szBuff, TEXT("file size: %d k"),m_wave.GetFileLength()/1024);
    m_unselLabel->SetText(szBuff);
  }
}

//--------------------------------------------------------------------------------
bool CFWavePanel::ButtonPressed(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFWavePanel::ButtonReleased(DWORD dwBtnID)
{
  //some local variables
  DWORD dwPos, dwInPos, dwOutPos, dwTill, dwFrom, dwCount, dwSize;
  CFMarker *markerTemp;
    
  switch(dwBtnID) {
    case ID_BTN_APP_UNDO:
      if (CFWaveOut::IsPlaying()) break;
      if (m_wave.Undo()) {
        m_bFormChanged= TRUE;
        m_zoomSlider->Reset(m_wave.GetBlockCount());
        m_inMarker->SetBlockPos(m_dwUndoMarker1);
        m_outMarker->SetBlockPos(m_dwUndoMarker2);
        m_posMarker->SetBlockPos(m_dwUndoPosMarker);
        UpdateWaveLabels();
        Draw();
      }
      break;

    case ID_BTN_APP_REDO: 
      if (CFWaveOut::IsPlaying()) break;
      if (m_wave.Redo()) {
        m_bFormChanged= TRUE;
        m_zoomSlider->Reset(m_wave.GetBlockCount());
        m_inMarker->SetBlockPos(m_dwUndoMarker1);
        m_outMarker->SetBlockPos(m_dwUndoMarker2);
        m_posMarker->SetBlockPos(m_dwUndoPosMarker);
        UpdateWaveLabels();
        Draw();
      }
      break;

    case ID_BTN_APP_CUTOUT:
      if (CFWaveOut::IsPlaying()) break;
      // both markers must be set
      if ((!m_inMarker->IsSet())&&(!m_outMarker->IsSet())) break;
      // do inside cut
      if (m_wave.Cut(m_inMarker->GetBlockPos(), m_outMarker->GetBlockPos(), TRUE)) {
        // save positions for undo
        m_dwUndoMarker1  = m_inMarker->GetBlockPos();
        m_dwUndoMarker2  = m_outMarker->GetBlockPos();
        m_dwUndoPosMarker= m_posMarker->GetBlockPos();
        // refresh display
        m_bFormChanged= TRUE;
        m_zoomSlider->Reset(m_wave.GetBlockCount());
        m_inMarker->SetBlockPos(m_inMarker->GetBlockPos());
        m_outMarker->SetBlockPos(m_inMarker->GetBlockPos());
        m_posMarker->Set();
        UpdateWaveLabels();
        Draw();
        m_actionLast = ACTION_CUTOUT;
      }
      break;

    case ID_BTN_APP_CUTIN:
      if (CFWaveOut::IsPlaying()) break;
      // both markers must be set
      if ((!m_inMarker->IsSet())&&(!m_outMarker->IsSet())) break;
      // do outside cut
      if (m_wave.Cut(m_inMarker->GetBlockPos(), m_outMarker->GetBlockPos(), FALSE)) {
        // save positions for undo
        m_dwUndoMarker1  = m_inMarker->GetBlockPos();
        m_dwUndoMarker2  = m_outMarker->GetBlockPos();
        m_dwUndoPosMarker = m_posMarker->GetBlockPos();
        m_bFormChanged= TRUE;
        m_zoomSlider->Reset(m_wave.GetBlockCount());
        m_inMarker->SetBlockPos(0);
        m_outMarker->SetBlockPos(m_wave.GetBlockCount());
        m_posMarker->Set();
        UpdateWaveLabels();
        Draw();
        m_actionLast = ACTION_CUTIN;
      }
      break;

    case ID_BTN_APP_ZOOMIN:
      if (CFWaveOut::IsPlaying()) break;
      m_zoomSlider->ZoomIn(m_posMarker->GetBlockPos());
      // redraw
      m_bFormChanged= TRUE;
      UpdateMarkers();
      Draw();
      break;

    case ID_BTN_APP_ZOOMOUT:
      if (CFWaveOut::IsPlaying()) break;
      m_zoomSlider->ZoomOut(m_posMarker->GetBlockPos());
      // redraw
      m_bFormChanged= TRUE;
      UpdateMarkers();
      Draw();
      break;

    case ID_BTN_APP_INFO:
      if (CFWaveOut::IsPlaying()) break;
      m_bInfo = !m_bInfo;
      UpdateWaveLabels();
      break;
    
    case ID_BTN_WAVE_PLAY: 
      if (!m_wave.IsValid()) break; //Check if there is wave
      if (CFWaveOut::IsPlaying()) break;

      dwPos = m_posMarker->GetBlockPos();
      if (dwPos == m_wave.GetBlockCount())
        dwPos = 0;

      // set data
      m_bJump= FALSE;
      
      // refresh marker
      m_posMarker->RestoreUnder();
      m_posMarker->SetBlockPos(dwPos);
      m_posMarker->SaveUnder();

      // set play values
      m_dwPlayStartBlock = dwPos;
      m_dwPlayLengthSize= (m_wave.GetBlockCount() - dwPos) * m_wave.GetBlockAlign() ;
      
      // play wave
      Play(); 

      break;

    case ID_BTN_WAVE_PAUSE: 
      // set data
      m_bJump= FALSE;
      // pause wave
      Pause(); 
      UpdateWaveLabels();
      break;

    case ID_BTN_WAVE_STOP:
      // stop wave
      m_bJump= FALSE;
      Stop();
      m_posMarker->RestoreUnder();
      m_posMarker->Set(); // Reset Head
      m_posMarker->SaveUnder();
      m_posMarker->Draw();
      UpdateWaveLabels();
      break;

    case ID_BTN_WAVE_PLAYFROM:
      if (!m_wave.IsValid()) break; //Check if there is wave

      dwInPos= m_inMarker->GetBlockPos();
      dwOutPos= m_outMarker->GetBlockPos();
      dwPos = m_posMarker->GetBlockPos();
      
      if ((m_inMarker->IsSet())&&(m_outMarker->IsSet())) { 
        
        if ((dwPos < dwInPos) && (dwPos < dwOutPos)) {
          dwFrom = 0;
        } else if ((dwPos > dwInPos) && (dwPos > dwOutPos)) {
          dwFrom = max(dwInPos,dwOutPos);
        } else dwFrom = min(dwInPos,dwOutPos);
      
      } else if ((m_inMarker->IsSet()) && (!m_outMarker->IsSet())) {
        
        if(dwPos > dwInPos)  
          dwFrom= dwInPos;
        else
          dwFrom = 0;
      
      } else if ((!m_inMarker->IsSet()) && (m_outMarker->IsSet())) {

        if(dwPos > dwOutPos)  
          dwFrom= dwOutPos;
        else
          dwFrom = 0;

      } else {
        dwFrom = 0;
      }
      
      // set data
      m_bJump= FALSE;
      
      // refresh marker
      m_posMarker->RestoreUnder();
      m_posMarker->SetBlockPos(dwFrom);
      m_posMarker->SaveUnder();

      // set play values
      m_dwPlayStartBlock = dwFrom;
      m_dwPlayLengthSize= (m_wave.GetBlockCount() - dwFrom) * m_wave.GetBlockAlign();
      
      // play wave
      Play();
      break;

    case ID_BTN_WAVE_PLAYTILL:
      if (!m_wave.IsValid()) break; //Check if there is wave
        
      dwInPos= m_inMarker->GetBlockPos();
      dwOutPos= m_outMarker->GetBlockPos();
      dwPos = m_posMarker->GetBlockPos();
      
      // at least one marker must be set and play-head must be in front of them
      if ((m_inMarker->IsSet())&&(m_outMarker->IsSet())) {
        
        if (dwPos < dwInPos) {
          dwTill= dwInPos;
        } else if (dwPos < dwOutPos) {
          dwTill= dwOutPos;
        } else {
          dwTill= m_wave.GetBlockCount();
        }
      
      } else if ((m_inMarker->IsSet())&&(!m_outMarker->IsSet())) {
      
        if (dwPos < dwInPos) {
          dwTill= dwInPos;
        } else {
          dwTill= m_wave.GetBlockCount();
          markerTemp= m_posMarker;
        }
      
      } else if ((!m_inMarker->IsSet())&&(m_outMarker->IsSet())) {
        
        if (dwPos < dwOutPos) {
          dwTill= dwOutPos;
        } else {
          dwTill= m_wave.GetBlockCount();
        }
      
      } else {
        dwTill= m_wave.GetBlockCount();
      }
      
      // set data
      m_bJump= FALSE;

      // refresh marker
      m_posMarker->RestoreUnder();
      m_posMarker->SetBlockPos(dwPos);
      m_posMarker->SaveUnder();

      // set play values
      m_dwPlayStartBlock = dwPos;
      m_dwPlayLengthSize= (dwTill - dwPos) * m_wave.GetBlockAlign() ;
      
      // play wave
      Play();

      break;

    case ID_BTN_WAVE_PLAYIN:
      if (!m_wave.IsValid()) break; //Check if there is wave
        
      dwInPos = m_inMarker->GetBlockPos();
      dwOutPos = m_outMarker->GetBlockPos();
     
      // both markers must be set for this function
      if ((m_inMarker->IsSet())&&(m_outMarker->IsSet())) {
       
        // set data
        m_bJump= FALSE;

        // refresh marker
        m_posMarker->RestoreUnder();
        m_posMarker->SetBlockPos(dwInPos);
        m_posMarker->SaveUnder();
        
        // set play values
        m_dwPlayStartBlock = dwInPos;
        m_dwPlayLengthSize= (dwOutPos - dwInPos) * m_wave.GetBlockAlign();
      
        // play wave
        Play();
      }
      break;

    case ID_BTN_WAVE_PLAYOUT:
      if (!m_wave.IsValid()) break; //Check if there is wave
      dwInPos= min(m_inMarker->GetBlockPos(), m_outMarker->GetBlockPos()) * m_wave.GetBlockAlign();
      dwOutPos= max(m_inMarker->GetBlockPos(), m_outMarker->GetBlockPos()) * m_wave.GetBlockAlign();
      
      dwCount= m_wave.GetBlockCount() * m_wave.GetBlockAlign();
      dwSize= dwCount - dwOutPos + dwInPos;
      
      // both markers must be set for this function
      if ((m_inMarker->IsSet())&&(m_outMarker->IsSet())) {
        // set data
        m_bJump= TRUE;
        m_dwJumpPos = m_inMarker->GetBlockPos();
        m_dwJumpBlockCount = m_outMarker->GetBlockPos() - m_inMarker->GetBlockPos();
        
        // refresh marker
        m_posMarker->RestoreUnder();
        m_posMarker->Set();
        m_posMarker->SaveUnder();
        
        // set play values
        m_dwPlayStartBlock= 0;
        m_dwPlayLengthSize= dwSize;

        // play wave
        Play();
      }
      break;

    case ID_BTN_WAVE_MARKIN:
      if (m_wave.IsValid()) {//Check if there is wave

        // clear all markers
        m_posMarker->RestoreUnder();  
        m_inMarker->RestoreUnder();
        m_outMarker->RestoreUnder();
        
        // redraw second marker
        m_outMarker->SaveUnder();
        m_outMarker->Draw();
        
        // set position for in-marker
        m_inMarker->Set();
        m_inMarker->SetPosX(m_posMarker->GetPosX());  
        m_inMarker->SaveUnder();  
        m_inMarker->Draw();
        
        // redraw tone-head
        m_posMarker->SaveUnder();
        m_posMarker->Draw();

        //swap markers?
        if ((m_inMarker->GetBlockPos() > m_outMarker->GetBlockPos()) && m_outMarker->IsSet()) {
          markerTemp= m_inMarker;
          m_inMarker= m_outMarker;
          m_outMarker= markerTemp;
        }
        UpdateWaveLabels();
      }
      break;

    case ID_BTN_WAVE_MARKOUT:
      if (m_wave.IsValid()) {//Check if there is wave
        
        // clear all markers
        m_posMarker->RestoreUnder();  
        m_inMarker->RestoreUnder();
        m_outMarker->RestoreUnder();

        // set position for out-marker
        m_outMarker->Set();
        m_outMarker->SetPosX(m_posMarker->GetPosX());  
        m_outMarker->SaveUnder();
        m_outMarker->Draw();

        // redraw second marker
        m_inMarker->SaveUnder();  
        m_inMarker->Draw();

        // redraw tone-head
        m_posMarker->SaveUnder();
        m_posMarker->Draw();
      
        //swap markers?
        if ((m_inMarker->GetBlockPos() > m_outMarker->GetBlockPos())  && m_inMarker->IsSet()) {
          markerTemp= m_inMarker;
          m_inMarker= m_outMarker;
          m_outMarker= markerTemp;
        }
        
        UpdateWaveLabels();
      }
      break;

    case ID_BTN_WAVE_YOUK:
      if (CFWaveOut::IsPlaying()) break;
      m_infoPanel->Draw();
      m_app->SetCaptureComp(m_infoPanel);
      break;

    default:
      return(FALSE);
      break;
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFWavePanel::UpdatePosLabel()
{
  TCHAR szBuff[128];

  if (!m_bInfo) {
    _stprintf(szBuff, TEXT("pos: %d.%03d"), m_posMarker->GetTimePos()/1000,m_posMarker->GetTimePos()%1000);
    m_posLabel->SetText(szBuff);
  }
}
