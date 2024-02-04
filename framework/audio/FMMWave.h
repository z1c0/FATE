// FMMWave.h: interface for the CFMMWave class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMMWave_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_)
#define AFX_FMMWave_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <mmsystem.h>
#include <mmreg.h>
#include <windows.h>
#include "FSampleList.h"


//------------------------------------------------------------------------------
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM  0x0001
#endif

//--------------------------------------------------------------------------------
/// How many Undo/Redo steps are allowed?
#define UNDO_REDO_STEPS   16

//--------------------------------------------------------------------------------
// Class for PCM (uncompressed ".wav") files.
class CFMMWave  
{
public:
	CFMMWave();
	CFMMWave(LPCTSTR pszFileName);
	~CFMMWave();
	void Clear();
  void Init();
	BOOL Undo();
	BOOL Redo();
  BOOL Load(LPCTSTR pszFileName);
  BOOL Load(char *pData, DWORD dwLen);
  BOOL Save() { return(SaveAs(m_szFileName)); };
  BOOL SaveAs(LPCTSTR pszFileName);	
  BOOL FileRecordingStart(LPCTSTR pszFileName);
  BOOL FileRecordingAdd(char *pBuffer, DWORD dwBuffSize);
  BOOL FileRecordingStop();
  LPCTSTR GetFileName() { return(m_szFileName); };
  ULONG GetFileLength() { return(m_ulFileLength); };
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
  DWORD GetSamples(char *pData, DWORD dwPos, DWORD dwCount);
  int GetSampleAt(DWORD dwBlockIndex);
  int operator[](DWORD dwBlockIndex) { return(GetSampleAt(dwBlockIndex)); };
  void ResetIterator() { m_pSampleIt->Reset(); };
  BOOL IsChanged() { return(m_bChanged); };
  LPCTSTR GetErr() { return(m_pszErr); };
private:
	BOOL ReadFmtChunk(char *pBuffer, LPDWORD pdwBuffPos);
  BOOL ReadDataChunk(char *pBuffer, LPDWORD pdwBuffPos);
	ULONG ReadULONG(char *pBuffer, LPDWORD pdwBuffPos);
	USHORT ReadUSHORT(char *pBuffer, LPDWORD pdwBuffPos);
	BOOL SkipChunk(char *pBuffer, LPDWORD pdwBuffPos);
  BOOL WriteFmtChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos);
	BOOL WriteDataChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos);
  BOOL WriteULONG(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, ULONG usVal);
	BOOL WriteUSHORT(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, USHORT usVal);
  CFSampleList *m_samples[UNDO_REDO_STEPS];  
  CFSampleListIterator *m_pSampleIt;
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
  HANDLE m_hFile;
  DWORD m_dwFilePos;
  HANDLE m_hFileToMap;
  HANDLE m_hMappedFile;
  char *m_pMappedData;
  WAVEFORMATEX m_wavefmt;  
  char *m_pWaveData; 
  TCHAR *m_pszErr;
  int m_iCurrList;
  int m_iNrOfCuts;
  int m_iNrOfRedoes;  
  BOOL m_bChanged;
};

#endif // !defined(AFX_FMMWave_H__099A488F_0774_4CB7_850F_3BC0C205B2F4__INCLUDED_)
