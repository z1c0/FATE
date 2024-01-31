// FWaveIn.h: interface for the CFWaveIn class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FWAVEIN_H__911E61F2_7A3D_4D89_8A0B_9D69EEA15715__INCLUDED_)
#define AFX_FWAVEIN_H__911E61F2_7A3D_4D89_8A0B_9D69EEA15715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FWave.h"

//--------------------------------------------------------------------------------
typedef void (*DataArrivedCallBackFunc)(char *pData, DWORD dwSize, LPVOID pvUserData);
#define MAX_BUFFERS  4 // maximum number of buffers


//--------------------------------------------------------------------------------
enum WAVEIN_STATUS 
{
  WAVEIN_READY,
  WAVEIN_RECORDING,
  WAVEIN_ERROR
};

//--------------------------------------------------------------------------------
class CFWaveIn  
{
public:
	static BOOL Initialize();
  static BOOL Reset();
  static BOOL Finalize();  
  static void SetWaveFormat(LPWAVEFORMATEX pWavefmt);
  static void SetDefaultWaveFormat();
  static void SetBufferLength(DWORD dwBufferLength);
  static void GetWaveFormat(LPWAVEFORMATEX pWavefmt) {memcpy(pWavefmt, &m_waveFmt, sizeof(BYTE)*18);};
  static BOOL Record(LPCTSTR pszFilename);
  static BOOL Record();
  static BOOL Stop();
  static BOOL Pause();
  static BOOL IsRecording() { return(m_flagStatus == WAVEIN_RECORDING); };
  static BOOL IsPaused() { return(m_bPaused); };
  static BOOL IsError() { return(m_flagStatus == WAVEIN_ERROR); };
  static BOOL IsFileRecording() { return(m_bRecordToFile); };  
  static void InstallCallBack(DataArrivedCallBackFunc callback, LPVOID pvUserData = NULL);   
  static CFWave* GetWave() { return(m_wave); };  
  static DWORD GetRecordedTime() { return(m_dwRecordedTime); };  
  static DWORD GetRecordedSize() { return(m_dwDataLength); };
private:  
  CFWaveIn() {};
  ~CFWaveIn() {};
  static void CalculateBuffers();
  static BOOL CleanUpBuffers();  
  static LRESULT CALLBACK RecordProc(HWND hwnd, UINT uMsg, WPARAM dwInstance, LPARAM dwParam);  
	static BOOL StopRecording();
	static BOOL StartRecording();
	static BOOL OpenInputDevice();	
  static CFWave *m_wave;
  static WAVEFORMATEX m_waveFmt;	
  static HWAVEIN m_hwi;
	static PWAVEHDR m_pWaveHdr[MAX_BUFFERS]; 	
  static DWORD m_dwBufferLength;
  static DWORD m_dwBufferSize;
	static int m_iBufferCount;	
  static char *m_pSaveBuffer;
  static BOOL m_bPaused;
	static DWORD m_dwDataLength;
  static DWORD m_dwRecordedTime;  
  static DataArrivedCallBackFunc m_callback;
  static LPVOID m_pvUserData;
  static WNDCLASS	m_wc;
	static HWND m_hWnd;
  static WAVEIN_STATUS m_flagStatus;
  static BOOL m_bInit;
  static BOOL m_bRecordToFile;
  static LPCTSTR m_pszFullFileName;
};

#endif // !defined(AFX_FWAVEIN_H__911E61F2_7A3D_4D89_8A0B_9D69EEA15715__INCLUDED_)


