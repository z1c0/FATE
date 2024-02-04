// FWave.h: interface for the CFWave class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FWAVE_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_)
#define AFX_FWAVE_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include <mmreg.h>
#include <windows.h>

#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM  0x0001
#endif

//--------------------------------------------------------------------------------
// Class for PCM (uncompressed ".wav") files.
class CFWave  
{
public:
	void Clear();
	BOOL Undo();
	CFWave();
	CFWave(LPCTSTR pszFileName);
	~CFWave();

  BOOL Load(LPCTSTR pszFileName);
  BOOL Save() { return(SaveAs(m_szFileName)); };
  BOOL SaveAs(LPCTSTR pszFileName);
	
  BOOL FileRecordingStart(LPCTSTR pszFileName);
  BOOL FileRecordingAdd(char *pBuffer, DWORD dwBuffSize);
  BOOL FileRecordingStop();

  LPCTSTR GetFileName() { return(m_szFileName); };
  ULONG GetFileLength() { return(m_ulFileLength); };

  BOOL ConvertToMono();
  BOOL Cut(DWORD dwLeft, DWORD dwRight, BOOL bInside= TRUE);
  
  BOOL IsValid() { return(m_bLoaded); };
  
  USHORT GetFormat() { return(m_usFormatTag); };
  USHORT GetChannels() { return(m_usChannels); };
  ULONG GetSampleRate() { return(m_ulSampleRate); };  
  ULONG GetBytesPerSec() { return(m_ulBytesPerSec); };  
  USHORT GetBlockAlign() { return(m_usBlockAlign); };
  USHORT GetBitsPerSample() { return(m_usBitsPerSample); };
  int GetBytesPerSample() { return(m_iBytesPerSample); };  

  DWORD GetBlockCount() { return(m_dwBlockCount); };
  DWORD GetWaveTime() { return(m_dwWaveTime); };

  void SetWaveFormat(LPWAVEFORMATEX pWavefmt);
  WAVEFORMATEX* GetWaveFormat()	{return (&m_wavefmt); };
  
  BOOL SetWaveData(char *pWave, ULONG ulSampleDataLength); 
  char *GetWaveData() { return(m_pWaveData); };
private:
  void Init();
	BOOL ReadFmtChunk(char *pBuffer, LPDWORD pdwBuffPos);
  BOOL ReadDataChunk(char *pBuffer, LPDWORD pdwBuffPos);
	ULONG ReadULONG(char *pBuffer, LPDWORD pdwBuffPos);
	USHORT ReadUSHORT(char *pBuffer, LPDWORD pdwBuffPos);
	BOOL SkipChunk(char *pBuffer, LPDWORD pdwBuffPos);
  BOOL WriteFmtChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos);
	BOOL WriteDataChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos);
  BOOL WriteULONG(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, ULONG usVal);
	BOOL WriteUSHORT(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, USHORT usVal);
  HANDLE m_hFile;
  BOOL m_bLoaded;  
  TCHAR m_szFileName[1024];
  ULONG m_ulFileLength;  
  USHORT m_usFormatTag;
  USHORT m_usChannels;
  ULONG m_ulSampleRate;
  ULONG m_ulBytesPerSec;
  USHORT m_usBlockAlign;
  USHORT m_usBitsPerSample;
  int m_iBytesPerSample;  
  DWORD m_dwBlockCount;
  DWORD m_dwWaveTime;
  DWORD m_dwBlocksRemoved;  
  DWORD m_dwFilePos;
  WAVEFORMATEX m_wavefmt;  
  char *m_pWaveData; 
  char *m_pUndo;
  
};

#endif // !defined(AFX_FWAVE_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_)
