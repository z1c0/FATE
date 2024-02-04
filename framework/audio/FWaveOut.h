// FWaveOut.h: interface for the CFWaveOut class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FWAVEOUT_H__6F9E2F46_5CFD_4EFC_B942_1A7C0BCF9D8E__INCLUDED_)
#define AFX_FWAVEOUT_H__6F9E2F46_5CFD_4EFC_B942_1A7C0BCF9D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FWave.h"
#include "FMMWave.h"

//--------------------------------------------------------------------------------
#define WAVEHDR_COUNT   2

//--------------------------------------------------------------------------------
class CFWaveOut  
{
public:
	static BOOL Initialize();
  static BOOL Finalize();
  static BOOL Open(LPWAVEFORMATEX pWaveFmt);
  static BOOL Open(CFWave *wave) { return(Open(wave->GetWaveFormat())); };
  static BOOL Open(CFMMWave *wave) { return(Open(wave->GetWaveFormat())); };
  static BOOL Close();  
  static BOOL Play(CFWave *wave);
  static BOOL Play(char *pData, DWORD dwSize);
  static BOOL Pause();
  static BOOL Restart();
  static BOOL Reset();  
  static void GetPosition(MMTIME *pmmtime) { waveOutGetPosition(m_hwo, pmmtime, sizeof(MMTIME));};
  static BOOL IsPlaying();
  static BOOL IsPaused() { return(m_bPaused); };  
  static BOOL SetVolume(DWORD dwVol);
  static BOOL GetVolume(LPDWORD pdwVol);
  static BOOL IsBufferDone(int iNr) { return((m_WaveHdrs[iNr].dwFlags & WHDR_DONE) == WHDR_DONE); };
  static void MarkBufferAsDone(int iNr) { m_WaveHdrs[iNr].dwFlags|= WHDR_DONE; };
private:
  CFWaveOut() {};
  ~CFWaveOut() {};
  static BOOL CleanUp();  
  static LRESULT CALLBACK PlayProc(HWND hwnd, UINT uMsg, WPARAM dwInstance, LPARAM dwParam);	
  static HWAVEOUT m_hwo;
  static WAVEHDR m_WaveHdrs[WAVEHDR_COUNT];
	static WAVEFORMATEX m_waveFmt;
  static UINT m_uDevId;  
  static DWORD m_dwBufferSize;
  static BOOL m_bPlay;
  static BOOL m_bPaused;
  static DWORD m_dwVol;
  static DWORD m_dwOldVol;  
  static int m_bInit;
  static UINT m_uiHdrIndex;
};

#endif // !defined(AFX_FWAVEOUT_H__6F9E2F46_5CFD_4EFC_B942_1A7C0BCF9D8E__INCLUDED_)
