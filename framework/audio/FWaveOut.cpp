// FWaveOut.cpp: implementation of the CFWaveOut class.
//////////////////////////////////////////////////////////////////////
#include "FWaveOut.h"


HWAVEOUT     CFWaveOut::m_hwo;
WAVEHDR      CFWaveOut::m_WaveHdrs[WAVEHDR_COUNT];
UINT         CFWaveOut::m_uiHdrIndex;
UINT         CFWaveOut::m_uDevId;
DWORD        CFWaveOut::m_dwBufferSize;
BOOL         CFWaveOut::m_bPlay;
BOOL         CFWaveOut::m_bPaused;
BOOL         CFWaveOut::m_bInit;
DWORD        CFWaveOut::m_dwVol;
DWORD        CFWaveOut::m_dwOldVol;
WAVEFORMATEX CFWaveOut::m_waveFmt;


//--------------------------------------------------------------------------------
BOOL CFWaveOut::Initialize()
{
  if (m_bInit) return(TRUE);
    
  m_hwo= NULL;
  m_dwBufferSize= 0;
  m_bPlay= FALSE;
  m_bPaused= FALSE;
  memset(m_WaveHdrs, 0, WAVEHDR_COUNT * sizeof(WAVEHDR));
  m_uiHdrIndex= 0;
  m_dwVol= m_dwOldVol= 0x33333333;  
  m_bInit = TRUE;
  
  return (TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWaveOut::Finalize()
{
  if(m_bInit) {
    if (m_hwo) {
      Close();
    }
    return (TRUE);
  }
  return (FALSE);
}

//--------------------------------------------------------------------------------
/// Opens a wave-device for playing. The parameter "pWaveFmt" specifies the format
/// of the wave data which has to be played. 
/// If the device is already opened, the call to "Open" will fail.
/// Return value FALSE indicates failure, TRUE indicates success.
BOOL CFWaveOut::Open(LPWAVEFORMATEX pWaveFmt)
{
	if (!m_bInit) return (FALSE);
  
  memcpy((LPWAVEFORMATEX)&m_waveFmt, pWaveFmt, sizeof(WAVEFORMATEX));
  
  MMRESULT mmres;
  // already opened?
  if (m_hwo) return(FALSE);

  // open audio device
  for (m_uDevId=0; m_uDevId<waveOutGetNumDevs(); m_uDevId++) {
		mmres = waveOutOpen(&m_hwo, m_uDevId, pWaveFmt, NULL, 0, CALLBACK_NULL );
		if (mmres == MMSYSERR_NOERROR) {
			break;
		}
	} 

  return(mmres == MMSYSERR_NOERROR);
}

//--------------------------------------------------------------------------------
/// Closes an opened wave device.
BOOL CFWaveOut::Close()
{
  MMRESULT mmres;

  m_bPlay = FALSE;
  m_bPaused = FALSE;

  if (m_hwo) {
    CleanUp();
    mmres= waveOutClose(m_hwo);
    m_hwo= NULL;
    return(mmres == MMSYSERR_NOERROR);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Unprepares the waveheader structures.
BOOL CFWaveOut::CleanUp()
{
  MMRESULT mmres;

  if (m_hwo) {    
    for (int i=0; i<WAVEHDR_COUNT; i++) {
      mmres= waveOutUnprepareHeader(m_hwo, &m_WaveHdrs[i], sizeof(WAVEHDR));
    }
    
    memset(&m_WaveHdrs, 0, WAVEHDR_COUNT * sizeof(WAVEHDR));
    m_uiHdrIndex= 0;
    return(mmres == MMSYSERR_NOERROR);    
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Starts playback of a wave-file. Return values indicate succes 
BOOL CFWaveOut::Play(CFWave *wave)
{
  // try to open device for playing
  return(Play(wave->GetWaveData(), wave->GetBlockCount() * wave->GetBlockAlign()));
}

//--------------------------------------------------------------------------------
/// Starts playback of raw wave data.
/// NOTE: The samples in "pData" must match the format structure formerly specified
/// when the device was opened.
/// Argument "dwSize" specified the size of the "pData" buffer
/// If the device is closed the call to "Play" will fail.
BOOL CFWaveOut::Play(char *pData, DWORD dwSize)
{
  if(!m_bInit) return (FALSE);
  
  // was wave device  opened?
  if (!m_hwo) return (FALSE);
  
  m_dwBufferSize = dwSize;
  m_bPlay= FALSE;
  m_bPaused= FALSE;
  
  m_uiHdrIndex= (m_uiHdrIndex + 1) % WAVEHDR_COUNT;

	m_WaveHdrs[m_uiHdrIndex].lpData         = pData;
  m_WaveHdrs[m_uiHdrIndex].dwBufferLength = dwSize;
	m_WaveHdrs[m_uiHdrIndex].dwUser         = 0;
	m_WaveHdrs[m_uiHdrIndex].dwFlags        = 0;
	m_WaveHdrs[m_uiHdrIndex].dwLoops        = 0;
	m_WaveHdrs[m_uiHdrIndex].dwBytesRecorded= 0;
	m_WaveHdrs[m_uiHdrIndex].lpNext         = NULL;
	m_WaveHdrs[m_uiHdrIndex].reserved       = 0;

	// prepare the wave header
  if (waveOutPrepareHeader(m_hwo, &m_WaveHdrs[m_uiHdrIndex], sizeof(WAVEHDR)) != MMSYSERR_NOERROR) {
		return(FALSE);
	}  
  
  // start playback
  if (waveOutWrite(m_hwo, &m_WaveHdrs[m_uiHdrIndex], sizeof(WAVEHDR)) != MMSYSERR_NOERROR) 
    return(FALSE);
  else { 
    m_bPlay= TRUE;
    m_bPaused= FALSE;
    return(TRUE);
  }
}

//--------------------------------------------------------------------------------
/// Pauses current playback.
BOOL CFWaveOut::Pause()
{
  if ((m_bPlay)&&(m_hwo)) {
    if (waveOutReset(m_hwo) == MMSYSERR_NOERROR) {
      m_bPaused= TRUE;
      return(TRUE);
    }
  }
  m_bPaused= FALSE;
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Determines whether the device is currently playing a sample.
BOOL CFWaveOut::IsPlaying()
{
  if (m_bPlay) {
    if (m_WaveHdrs[m_uiHdrIndex].dwFlags & WHDR_DONE) {  // check if the buffer is done.
      m_bPlay = FALSE;
    }
  }
  return(m_bPlay);
}

//--------------------------------------------------------------------------------
/// Sets volume.
BOOL CFWaveOut::SetVolume(DWORD dwVol)
{
  // has volume changed?
  if (m_dwVol == dwVol) 
    return(TRUE);
  else {
    m_dwOldVol= m_dwVol;
    m_dwVol= dwVol;
    return(waveOutSetVolume((HWAVEOUT)m_uDevId, dwVol) == MMSYSERR_NOERROR);
  }
}

//--------------------------------------------------------------------------------
/// Retrieves the current volume of the wave-out device.
BOOL CFWaveOut::GetVolume(LPDWORD pdwVol)
{
  return(waveOutGetVolume((HWAVEOUT)m_uDevId, pdwVol) == MMSYSERR_NOERROR);
}

//--------------------------------------------------------------------------------
BOOL CFWaveOut::Restart()
{
  if (m_bPaused&&m_bPlay) {
    if (waveOutRestart(m_hwo) == MMSYSERR_NOERROR) {
      m_bPaused= FALSE;
      m_bPlay = FALSE;
      return(TRUE);
    }
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFWaveOut::Reset()
{
  BOOL bCont = TRUE;
  if (waveOutReset(m_hwo) == MMSYSERR_NOERROR) {
    
    m_bPlay = FALSE;
    m_bPaused = FALSE;
    CleanUp();
    return(TRUE);  
  }
  CleanUp();
  return(FALSE);
}

