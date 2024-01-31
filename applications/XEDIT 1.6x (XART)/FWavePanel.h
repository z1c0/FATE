// FWavePanel.h: interface for the CFWavePanel class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FWAVEPANEL_H__51F5E5D1_636C_4AB7_A72F_99A3981EB651__INCLUDED_)
#define AFX_FWAVEPANEL_H__51F5E5D1_636C_4AB7_A72F_99A3981EB651__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FATE/stdafx.h"
#include "FMarker.h"
#include "FZoomSlider.h"
#include "FInfoPanel.h"
#include "ids.h"


//--------------------------------------------------------------------------------
#define XEDIT_DEMO_TIME_LIMIT  30000

//--------------------------------------------------------------------------------
enum ACTIONS
{
  ACTION_NONE,
  ACTION_CUTIN,
  ACTION_CUTOUT,
  ACTION_MOVE_POS_MARKER,
  ACTION_MOVE_IN_MARKER,
  ACTION_MOVE_OUT_MARKER,
};

//--------------------------------------------------------------------------------
/// Forward declaration
class CFWaveCutApp;


//--------------------------------------------------------------------------------
class CFWavePanel : public CFPanel  
{
  // declare callback functions as friends
  friend DWORD WINAPI PlayBackThreadFunc(LPVOID pParam);
public:
	BOOL Create();
	CFWavePanel();
  virtual ~CFWavePanel();  
  virtual BOOL ButtonReleased(DWORD dwBtnID);
	virtual BOOL ButtonPressed(DWORD dwBtnID);
	virtual BOOL StylusDown(int xPos, int yPos);
  virtual BOOL StylusMove(int xPos, int yPos);
	virtual BOOL StylusUp(int xPos, int yPos);
  virtual BOOL KeyDown(PdaKey key) { return(FALSE); };
  virtual BOOL KeyUp(PdaKey key) { return(FALSE); };
  void Draw();
  virtual void SetX(int iPosX); 
  virtual void SetY(int iPosY); 
  BOOL CloseOldWave();  
  BOOL LoadWave(LPCTSTR pFileName);
  BOOL SaveWave(LPCTSTR pFileName);
  CFMMWave* GetWave() { return(&m_wave); };
  DWORD GetZoomBlockStart() { return(m_zoomSlider->GetZoomBlockStart()); };
  DWORD GetZoomBlockEnd() { return(m_zoomSlider->GetZoomBlockEnd()); };
  DWORD GetZoomBlockCount() { return(m_zoomSlider->GetZoomBlockCount()); };  
  DWORD GetBlockCount() { return(m_wave.GetBlockCount()); };
  void MoveHead(DWORD dwOffs);  
  BOOL SetVolume(DWORD dwVol) { return(CFWaveOut::SetVolume(dwVol)); };
  BOOL Play();
  BOOL Pause();
  BOOL Stop();
private:
	void UpdatePosLabel();
  void UpdatePos();
  void UpdateMarkers();
  void UpdateWaveLabels();	
  CFInfoPanel *m_infoPanel;
  CFWaveCutApp *m_pApp;
  CFMMWave m_wave;
  CFBitmap *m_bmpWaveWnd;  
  CFBitmap *m_bmpWaveForm;
  CFBitmap *m_bmpSaving;  
  CFBitmap *m_bmpLoading;
  CFBitmap *m_bmpZoomPat;    
  CFButton *m_btnRec;
  CFButton *m_btnPause;
  CFButton *m_btnPlay;
  CFButton *m_btnStop;
  CFButton *m_btnMarkIn;
  CFButton *m_btnMarkOut;
  CFButton *m_btnPlayFrom;
  CFButton *m_btnPlayTill;
  CFButton *m_btnPlayIn;
  CFButton *m_btnPlayOut;
  CFButton *m_btnYouk;  
  CFLabel *m_posLabel;
  CFLabel *m_inLabel;
  CFLabel *m_outLabel;
  CFLabel *m_totLabel;
  CFLabel *m_selLabel;
  CFLabel *m_unselLabel;
  CFMarker *m_inMarker;
  CFMarker *m_outMarker;
  CFMarker *m_posMarker;
  CFMarker *m_selMarker;  
  CFZoomSlider *m_zoomSlider;
  enum ACTIONS m_actionLast;
  HPEN m_hWavePen;
  HBRUSH m_hFillBrush;
  COLORREF m_colWaveBack;
  BOOL m_bFormChanged;  
  BOOL m_bJump;
  DWORD m_dwJumpPos;
  DWORD m_dwJumpBlockCount;  
  DWORD m_dwPlayStartBlock;
  DWORD m_dwPlayLengthSize;  
  HANDLE m_hPlayThread;
  volatile BOOL m_bRunThread;
  int m_iMarkerWidth;  
  BOOL m_bCreated;
  BOOL m_bInfo;
  DWORD m_dwUndoMarker1;
  DWORD m_dwUndoMarker2;
  DWORD m_dwUndoPosMarker;
  DWORD m_dwOldVolume;
};

#endif // !defined(AFX_FWAVEPANEL_H__51F5E5D1_636C_4AB7_A72F_99A3981EB651__INCLUDED_)
