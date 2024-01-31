// FMMWave.cpp: implementation of the CFMMWave class.
//////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include "FMMWave.h"


//--------------------------------------------------------------------------------
CFMMWave::CFMMWave()
{
  Init();
  Clear();
}

//--------------------------------------------------------------------------------
CFMMWave::CFMMWave(LPCTSTR pszFileName)
{
  Init();
  Clear();
  Load(pszFileName);
}

//--------------------------------------------------------------------------------
CFMMWave::~CFMMWave()
{
  Clear();
}

//--------------------------------------------------------------------------------
/// Initializes new CFMMWave object. 
/// NOTE: Intended to be only called by constructor!
void CFMMWave::Init()
{
  m_hFile      = INVALID_HANDLE_VALUE;
  m_hFileToMap = INVALID_HANDLE_VALUE;
  m_hMappedFile= NULL;
  m_pMappedData= NULL;
  m_pWaveData  = NULL;
  m_pSampleIt  = NULL;
  m_bChanged   = FALSE;
  for (int i=0; i<UNDO_REDO_STEPS; i++) m_samples[i]= NULL;
  m_iCurrList= 0;    // current sample list to use
  m_pszErr= NULL;
}


//--------------------------------------------------------------------------------
/// Completely clears and resets a CFMMWave object.
void CFMMWave::Clear()
{
  // clear all data  
  m_bLoaded= FALSE;
  m_szFileName[0]= 0;
  
  m_usFormatTag    = 0;
  m_usChannels     = 0;
  m_ulSampleRate   = 0;
  m_ulBytesPerSec  = 0;
  m_usBlockAlign   = 0;
  m_usBitsPerSample= 0;
  m_iBytesPerSample= 0;
  
  // record to file values
  if (m_hFile != INVALID_HANDLE_VALUE) CloseHandle(m_hFile);
  m_dwFilePos = 0;

  // reset file mapping handles
  if (m_pMappedData != NULL) UnmapViewOfFile(m_pMappedData);
  if (m_hMappedFile != NULL) CloseHandle(m_hMappedFile);
  // m_hFileToMap must not be closed via CloseHandle!

  m_dwBlockCount= 0;
  m_dwWaveTime  = 0;  // total wave time in seconds
  m_ulFileLength= 0;
  
  memset(&m_wavefmt, 0, sizeof(WAVEFORMATEX));
  
  m_pWaveData= NULL;
  
  // safe-delete sample lists
  for (int i=0; i<UNDO_REDO_STEPS; i++) {
    if (m_samples[i]) {
      delete(m_samples[i]);
      m_samples[i]= NULL;
    }
  }

  m_bChanged   = FALSE;
  m_iCurrList  = 0;
  m_iNrOfCuts  = 0;
  m_iNrOfRedoes= 0;
  Init();
}

//--------------------------------------------------------------------------------
/// Loads a wave-file from the specified location.
BOOL CFMMWave::Load(LPCTSTR pszFileName)
{
  DWORD dwSize= 0;
  
  // clear the wave
  Clear();

#ifdef _WIN32_WCE
  m_hFileToMap= CreateFileForMapping(pszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);
  dwSize= GetFileSize(m_hFileToMap, NULL);
    
  if (m_hFileToMap != INVALID_HANDLE_VALUE) {
	  m_hMappedFile= CreateFileMapping(m_hFileToMap, NULL, PAGE_READWRITE, 0, 0, 0);
    if (m_hMappedFile) {
		  m_pMappedData= (char*)MapViewOfFile(m_hMappedFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		  if (m_pMappedData) {
     	  if (!Load(m_pMappedData, dwSize)) 
          return(FALSE);

      } else {
        m_pszErr= TEXT("MapViewOfFile failed");
        return(FALSE);
      }

    } else {
      m_pszErr= TEXT("CreateFileMapping failed");
      return(FALSE);
    }

  } else {
    m_pszErr= TEXT("CreateFileForMapping failed");
    return(FALSE);
  }
#else
  return(FALSE);
#endif
  
  // set the filename and we are done
  _tcscpy(m_szFileName, pszFileName);

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Loads a wave-file from the specified memory.
BOOL CFMMWave::Load(char *pData, DWORD dwLen)
{
  DWORD dwBuffPos= 0;
  BOOL bParse= TRUE;
  BOOL bRet= FALSE;

  // validate "RIFF" sequence
  if (memcmp(&pData[dwBuffPos], "RIFF", 4)) {
    m_pszErr= TEXT("No RIFF sequence found");
    // reset values
    Clear();
    return(FALSE);
  }
  dwBuffPos+= 4;

  // read file-length
  m_ulFileLength= ReadULONG(pData, &dwBuffPos);
  m_ulFileLength+= 8;
  if (m_ulFileLength != dwLen) {
    // just correct this error ... don't fail
    dwBuffPos-= 4;
    WriteULONG(pData, dwLen, &dwBuffPos, dwLen - 8);
  }

  // validate "WAVE" sequence
  if (memcmp(&pData[dwBuffPos], "WAVE", 4)) {
    m_pszErr= TEXT("No WAVE sequence found");
    // reset values
    Clear();
    return(FALSE);
  }
  dwBuffPos+= 4;

  // parse the chunks
  while (bParse) {
    // what kind of chunk was read?
    if (!memcmp(&pData[dwBuffPos], "fmt ", 4)) {
      // "fmt " chunk
      dwBuffPos+= 4;
      bRet= ReadFmtChunk(pData, &dwBuffPos);
        
    } else if (!memcmp(&pData[dwBuffPos], "data", 4)) {      
      // "data" chunk
      dwBuffPos+= 4;
      bRet= ReadDataChunk(pData, &dwBuffPos);
      // after reading the data cunk we are done
      bParse= FALSE;
      
    } else {
      // unknown or unsupported chunk encountered        
      dwBuffPos+= 4;
      bRet= SkipChunk(pData, &dwBuffPos);
    }

    // encountered a problem?
    if (!bRet) {
      // reset values
      Clear();
      return(FALSE);
    }
  }    
   
  // finally, set flag that loading succeeded
  m_bLoaded= TRUE;
  // wave was loaded successfully, so create new list for valid sample data
  m_samples[m_iCurrList]= new CFSampleList();
  //insert first block in sample block list
  m_samples[m_iCurrList]->Append(0, m_dwBlockCount);
  m_samples[m_iCurrList]->SetBlockCount(m_dwBlockCount);
  m_pSampleIt= m_samples[m_iCurrList]->GetIterator();

  return(TRUE);  
}

//--------------------------------------------------------------------------------
/// Auxiliary method which reads an ULONG number from the specified buffer starting
/// at "dwBuffPos".
ULONG CFMMWave::ReadULONG(char *pBuffer, LPDWORD pdwBuffPos)
{  
  ULONG ulRet= 0;

  if (pBuffer) {
    ulRet= (BYTE)pBuffer[*pdwBuffPos + 3];
    for (int i=2; i >= 0; i--) {
      ulRet<<= 8;
      ulRet|= (BYTE)pBuffer[*pdwBuffPos + i];
    }
  }
  *pdwBuffPos+= 4;
  return(ulRet);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which reads an USHORT number from the specified buffer 
/// starting at "dwBuffPos".
USHORT CFMMWave::ReadUSHORT(char *pBuffer, LPDWORD pdwBuffPos)
{
  USHORT usRet= 0;

  if (pBuffer) {
    usRet= (BYTE)pBuffer[*pdwBuffPos + 1];
    usRet<<= 8;
    usRet|= (BYTE)pBuffer[*pdwBuffPos];
  }
  *pdwBuffPos+= 2;
  return(usRet);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which parses a "fmt" chunk and stores the extracted data in
/// the according member variables.
BOOL CFMMWave::ReadFmtChunk(char *pBuffer, LPDWORD pdwBuffPos)
{
  DWORD dwRet= 0;
  DWORD dwBytesTotal= 0;
  ULONG ulFmtDataLength= 0;

  // read length of "fmt" data (should be 16)
  ulFmtDataLength= ReadULONG(pBuffer, pdwBuffPos);
  
  // read encoding tag - only PCM (uncompressed format) is supported
  m_usFormatTag= ReadUSHORT(pBuffer, pdwBuffPos);
  if (m_usFormatTag != WAVE_FORMAT_PCM) {
    m_pszErr= TEXT("Wave format is not PCM");
    return(FALSE);
  }
  m_wavefmt.wFormatTag= m_usFormatTag;
  dwBytesTotal+= 2;  // increase byte counter

  // read channel information (mono / stereo)
  m_usChannels= ReadUSHORT(pBuffer, pdwBuffPos);
  if ((m_usChannels != 1)&&(m_usChannels != 2)) {
    m_pszErr= TEXT("Illegal value for channels");
    return(FALSE);
  } 
  m_wavefmt.nChannels= m_usChannels;
  dwBytesTotal+= 2;  // increase byte counter

  // read sampling rate
  m_ulSampleRate= ReadULONG(pBuffer, pdwBuffPos);
  if (!m_ulSampleRate) {
    m_pszErr= TEXT("Sampling rate cannot be zero");
    return(FALSE);
  } 
  m_wavefmt.nSamplesPerSec= m_ulSampleRate;
  dwBytesTotal+= 4;  // increase byte counter

  // read bytes per second
  m_ulBytesPerSec= ReadULONG(pBuffer, pdwBuffPos);
  if (!m_ulBytesPerSec) {
    m_pszErr= TEXT("Bytes per second cannot be zero");
    return(FALSE);
  } 
  m_wavefmt.nAvgBytesPerSec= m_ulBytesPerSec;
  dwBytesTotal+= 4;  // increase byte counter

  // read block align
  m_usBlockAlign= ReadUSHORT(pBuffer, pdwBuffPos);
  m_wavefmt.nBlockAlign= m_usBlockAlign;
  dwBytesTotal+= 2;  // increase byte counter

  // read bits per sample
  m_usBitsPerSample= ReadUSHORT(pBuffer, pdwBuffPos);
  if (!m_usBitsPerSample) {
    m_pszErr= TEXT("Bits per sample cannot be zero");
    return(FALSE);
  } 
  m_wavefmt.wBitsPerSample= m_usBitsPerSample;
  m_iBytesPerSample= m_usBitsPerSample / 8;
  dwBytesTotal+= 2;  // increase byte counter

  // skip any additional bytes if there
  if (dwBytesTotal != ulFmtDataLength) 
    *pdwBuffPos+= ulFmtDataLength - dwBytesTotal;

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which parses a "data" chunk and stores the extracted data in
/// the according member variables.
BOOL CFMMWave::ReadDataChunk(char *pBuffer, LPDWORD pdwBuffPos)
{
  ULONG ulSampleDataLength= 0;

  // read length of the sample data
  ulSampleDataLength= ReadULONG(pBuffer, pdwBuffPos);
  if (!ulSampleDataLength) {
    m_pszErr= TEXT("Length of sample data cannot be zero");
    return(FALSE);
  }

  // store number of blocks in wave
  m_dwBlockCount= ulSampleDataLength / (m_usBlockAlign);

  // calculate time-length of wave
  m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
  m_ulFileLength= ulSampleDataLength + 44;

  // save pointer to samples
  m_pWaveData= pBuffer + *pdwBuffPos;

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which skips a chunk which is unknown or not supported.
BOOL CFMMWave::SkipChunk(char *pBuffer, LPDWORD pdwBuffPos)
{
  ULONG ulChunkDataLength= 0;

  // read length of chunk
  ulChunkDataLength= ReadULONG(pBuffer, pdwBuffPos);

  // skip chunk data
  *pdwBuffPos+= ulChunkDataLength;
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// If current wave file is valid it will be saved to the specified location.
BOOL CFMMWave::SaveAs(LPCTSTR pszFileName)
{
  // is this a valid (correctly loaded) wave file?
  if (m_bLoaded) {
    TCHAR szFileName[1024];
    HANDLE hFile;
    char *pBuffer= m_pMappedData;
    DWORD dwBuffPos= 0;
    DWORD dwBytesWritten= 0;

    // write "RIFF" sequence
    if (dwBuffPos + 4 > m_ulFileLength) {
      Clear();
      return(FALSE);
    }
    memcpy(&pBuffer[dwBuffPos], "RIFF", 4);
    dwBuffPos+= 4;

    // write file length - 8
    if (!WriteULONG(pBuffer, m_ulFileLength, &dwBuffPos, m_ulFileLength - 8)) {
      Clear();
      return(FALSE);
    }
  
    // write "WAVE" sequence
    if (dwBuffPos + 4 > m_ulFileLength) {
      Clear();
      return(FALSE);
    }
    memcpy(&pBuffer[dwBuffPos], "WAVE", 4);
    dwBuffPos+= 4;
  
    // write the fmt chunk
    if (!WriteFmtChunk(pBuffer, m_ulFileLength, &dwBuffPos)) {
      Clear();
      return(FALSE);
    }

    // write the data chunk
    if (!WriteDataChunk(pBuffer, m_ulFileLength, &dwBuffPos)) {
      Clear();
      return(FALSE);
    }

    // release memory mapping data
    UnmapViewOfFile(m_pMappedData); m_pMappedData= NULL;
    CloseHandle(m_hMappedFile);     m_hMappedFile= NULL;
    m_hFileToMap= INVALID_HANDLE_VALUE;

    // set correct file size
    hFile= CreateFile(m_szFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
                      FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(hFile, m_ulFileLength, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);
    CloseHandle(hFile);

    // backup file name for reloading
    _tcscpy(szFileName, m_szFileName);
    // clear the wave and reload it
    Clear();
    Load(szFileName);	  

    return(TRUE);  
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes a "FMT" chunk to the specified buffer and 
/// increases the position index for the buffer. A check on available buffer 
/// size is done.
BOOL CFMMWave::WriteFmtChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos) 
{
  if (pBuffer) {    
    
    // write "fmt " sequence
    if (*pdwBuffPos + 4 > m_ulFileLength) {
      free(pBuffer);
      pBuffer = NULL;      
      return(FALSE);
    }
    memcpy(&pBuffer[*pdwBuffPos], "fmt ", 4);
    *pdwBuffPos+= 4;

    // write length of the fmt data (=16)
    if (!WriteULONG(pBuffer, m_ulFileLength, pdwBuffPos, 16)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write encoding tag (0x01 ... PCM)
    if (!WriteUSHORT(pBuffer, m_ulFileLength, pdwBuffPos, 1)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write channels (1 = mono, 2 = stereo)
    if (!WriteUSHORT(pBuffer, m_ulFileLength, pdwBuffPos, (USHORT)m_usChannels)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }  
    
    // write sample rate
    if (!WriteULONG(pBuffer, m_ulFileLength, pdwBuffPos, m_ulSampleRate)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write bytes per second
    if (!WriteULONG(pBuffer, m_ulFileLength, pdwBuffPos, m_ulBytesPerSec)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write block align  
    if (!WriteUSHORT(pBuffer, m_ulFileLength, pdwBuffPos, m_usBlockAlign)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write bits per sample align
    if (!WriteUSHORT(pBuffer, m_ulFileLength, pdwBuffPos, m_usBitsPerSample)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }

    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes a "DATA" chunk to the specified buffer and 
/// increases the position index for the buffer. A check on available buffer 
/// size is done.
BOOL CFMMWave::WriteDataChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos) 
{
  if (pBuffer) {
    DWORD dwSampleData= 0;
    DWORD dwDestPos;

    // write "data" sequence
    if (*pdwBuffPos + 4 > m_ulFileLength) {
      return(FALSE);
    }
    memcpy(&pBuffer[*pdwBuffPos], "data", 4);
    *pdwBuffPos+= 4;

    // write size of sample data
    dwSampleData= m_dwBlockCount * m_usBlockAlign;
    if (!WriteULONG(pBuffer, m_ulFileLength, pdwBuffPos, dwSampleData)) {
      return(FALSE); 
    }

    // write actual samples
    if (*pdwBuffPos + dwSampleData > m_ulFileLength) return(FALSE);
    
    // get samples from sample list
    dwDestPos= *pdwBuffPos;
    ResetIterator();
    while(m_pSampleIt->HasMore()) {
      memcpy(&pBuffer[dwDestPos], 
             &m_pWaveData[m_pSampleIt->GetBlockStart() * m_usBlockAlign], 
             m_pSampleIt->GetBlocksLen() * m_usBlockAlign);
      dwDestPos+= (m_pSampleIt->GetBlocksLen() * m_usBlockAlign);
      m_pSampleIt->Next();
    }
    *pdwBuffPos+= dwSampleData;
    
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes an ULONG to the specified buffer and increases
/// position index for the buffer. A check on available buffer size is done.
BOOL CFMMWave::WriteULONG(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, ULONG ulVal)
{  
  if ((pBuffer)||(*pdwBuffPos + 4 <= dwBuffSize)) {
    BYTE val;

    for (int i=0; i<4; i++) {
      val= (BYTE)(ulVal & 0x000000FF);
      pBuffer[*pdwBuffPos + i]= val;
      ulVal>>= 8;    
    }    
    *pdwBuffPos+= 4;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes an USHORT to the specified buffer and increases
/// position index for the buffer. A check on available buffer size is done.
BOOL CFMMWave::WriteUSHORT(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, USHORT usVal)
{
  if ((pBuffer)||(*pdwBuffPos + 2 <= dwBuffSize)) {
    BYTE val;

    for (int i=0; i<2; i++) {
      val= (BYTE)(usVal & 0x00FF);
      pBuffer[*pdwBuffPos + i]= val;
      usVal>>= 8;    
    }    
    *pdwBuffPos+= 2;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Does a cutting operation on the current wave file.
/// Parameters "dwLeft" and "dwRight" specifies the bounds within the sample data
/// where the cut will happen.
/// Parameter "bInside" has following meaning:
/// TRUE: the sample data within the bounds will be cut 
/// FALSE: the sample data outside the bounds will be cut.
BOOL CFMMWave::Cut(DWORD dwLeft, DWORD dwRight, BOOL bInside /* = TRUE */)
{
  // backup current list
  CFSampleList *pCurrList= m_samples[m_iCurrList];
  CFSampleList *pUndoList= m_samples[m_iCurrList]->Clone();
  
  // is this a valid (correctly loaded) wave file?
  if (m_bLoaded) {   
    // block if borders need to be swapped
    if (dwRight < dwLeft) {
      DWORD dwSwap= dwLeft;
      dwLeft= dwRight;
      dwRight= dwSwap;
    }

    // check if borders are correct
    if ((dwLeft < 0)||(dwRight > m_dwBlockCount)) return(FALSE);
    if ((dwLeft == dwRight)) return(FALSE);
    if ((dwLeft == 0)&&(dwRight == m_dwBlockCount)) return(FALSE);
    
    // is cutting done inside or outside the borders?
    if (bInside) {
      dwLeft++;  // block of left border will remain in buffer
      if (dwLeft == dwRight) return(TRUE);

      m_samples[m_iCurrList]->Cut(dwLeft, dwRight);
      // correct some values
      m_dwBlockCount-= (dwRight - dwLeft);
      m_ulFileLength = m_dwBlockCount * m_usBlockAlign + 44;
      m_dwWaveTime   = (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    
    } else {  // OUTSIDE CUT
      dwRight++;  //  block of right border will remain in buffer
      
      // this will actuallay result in 2 cuts
      m_samples[m_iCurrList]->Cut(0, dwLeft);
      // correct some values
      m_dwBlockCount-= dwLeft;
      m_ulFileLength = m_dwBlockCount * m_usBlockAlign * m_usChannels + 44;
      m_dwWaveTime   = (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
      
      dwRight-= dwLeft;
      m_samples[m_iCurrList]->Cut(dwRight, m_dwBlockCount - 1);
      // correct some values
      m_dwBlockCount= dwRight;
      m_ulFileLength= m_dwBlockCount * m_usBlockAlign * m_usChannels + 44;
      m_dwWaveTime  = (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    }
    // set modified flag
    m_bChanged= TRUE;

    // correct number of samples
    pCurrList->SetBlockCount(m_dwBlockCount);
    // swap the lists
    m_samples[m_iCurrList]= pUndoList;
    m_iCurrList= (m_iCurrList + 1) % UNDO_REDO_STEPS;
    if (m_samples[m_iCurrList]) delete(m_samples[m_iCurrList]);
    m_samples[m_iCurrList]= pCurrList;
    m_iNrOfCuts= min(m_iNrOfCuts + 1, UNDO_REDO_STEPS - 1);
    m_iNrOfRedoes= 0;
    // get pointer to current iterator
    m_pSampleIt= m_samples[m_iCurrList]->GetIterator();
    return(TRUE);
  }
  delete(pUndoList);
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// "Undoes" the last cutting operations unsing the stored sample lists.
BOOL CFMMWave::Undo()
{
  // is this a valid (correctly loaded) wave file?
  if ((m_bLoaded)&&(m_iNrOfCuts)) {
    m_iNrOfCuts--;
    m_iNrOfRedoes++;
    m_iCurrList--;
    if (m_iCurrList < 0) m_iCurrList= UNDO_REDO_STEPS - 1;
    // correct some values
    m_dwBlockCount= m_samples[m_iCurrList]->GetBlockCount();
    m_ulFileLength= m_dwBlockCount * m_usBlockAlign * m_usChannels + 44;
    m_dwWaveTime  = (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    // get pointer to current iterator
    m_pSampleIt= m_samples[m_iCurrList]->GetIterator();
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// "Redoes" the last cutting operations unsing the stored sample lists.
BOOL CFMMWave::Redo()
{
  // is this a valid (correctly loaded) wave file?
  if ((m_bLoaded)&&(m_iNrOfRedoes)) {
    m_iNrOfCuts++;
    m_iNrOfRedoes--;
    m_iCurrList= (m_iCurrList + 1) % UNDO_REDO_STEPS;
    // correct some values
    m_dwBlockCount= m_samples[m_iCurrList]->GetBlockCount();
    m_ulFileLength= m_dwBlockCount * m_usBlockAlign * m_usChannels + 44;
    m_dwWaveTime  = (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    // get pointer to current iterator
    m_pSampleIt= m_samples[m_iCurrList]->GetIterator();
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
void CFMMWave::SetWaveFormat(LPWAVEFORMATEX pWavefmt)
{
  memcpy(&m_wavefmt, pWavefmt, sizeof(WAVEFORMATEX));
  m_usFormatTag    = m_wavefmt.wFormatTag;
  m_usChannels     = m_wavefmt.nChannels;
  m_ulSampleRate   = m_wavefmt.nSamplesPerSec;
  m_ulBytesPerSec  = m_wavefmt.nAvgBytesPerSec;
  m_usBlockAlign   = m_wavefmt.nBlockAlign;
  m_usBitsPerSample= m_wavefmt.wBitsPerSample;
  m_iBytesPerSample= m_usBitsPerSample / 8;
}

//--------------------------------------------------------------------------------
BOOL CFMMWave::FileRecordingStart(LPCTSTR pszFileName) 
{
  BOOL bRet;
  
  char *pBuffer= NULL;
  DWORD dwBuffPos= 0;
  DWORD dwBytesWritten= 0;
  DWORD dwBufferSize = 44; // header size
  m_ulFileLength = 44;
  
  // first create the file
  m_hFile = CreateFile(pszFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                       FILE_FLAG_WRITE_THROUGH|FILE_FLAG_RANDOM_ACCESS, NULL);
  if (m_hFile == INVALID_HANDLE_VALUE) {
    return(FALSE);
  }

  // allocate a buffer for caching the wave data
  pBuffer= (char*)malloc(dwBufferSize);
  if (!pBuffer) return(FALSE);
  
  // CACHE RIFF CHUNK
  // write "RIFF" sequence
  if (dwBuffPos + 4 > dwBufferSize) {
    free(pBuffer);
    pBuffer = NULL;
    return(FALSE);
  }
  memcpy(&pBuffer[dwBuffPos], "RIFF", 4);
  dwBuffPos+= 4;

  // write file length - 8
  if (!WriteULONG(pBuffer, dwBufferSize, &dwBuffPos, 0)) {
    free(pBuffer);
    pBuffer = NULL;
    return(FALSE);
  }

  // write "WAVE" sequence
  if (dwBuffPos + 4 > dwBufferSize) {
    free(pBuffer);
    pBuffer = NULL;
    return(FALSE);
  }
  memcpy(&pBuffer[dwBuffPos], "WAVE", 4);
  dwBuffPos+= 4;

  // CACHE FMT CHUNK
  // write the fmt chunk
  if (!WriteFmtChunk(pBuffer, dwBufferSize, &dwBuffPos)) {
    free(pBuffer);
    pBuffer = NULL;
    return(FALSE);
  }

  // CACHE DATA CHUNK
  // write "data" sequence
  if (dwBuffPos + 4 > dwBufferSize) {
    return(FALSE);
  }
  memcpy(&pBuffer[dwBuffPos], "data", 4);
  dwBuffPos+= 4;

  // write size of sample data
  if (!WriteULONG(pBuffer, m_ulFileLength, &dwBuffPos, 0)) {
    return(FALSE); 
  }
  
  // WRITE CACHE TO FILE
  bRet= WriteFile(m_hFile, pBuffer, dwBufferSize, &dwBytesWritten, NULL);
  if ((!bRet)||(dwBufferSize != dwBytesWritten)) {
    CloseHandle(m_hFile);
    free(pBuffer);
    pBuffer = NULL;
    return(FALSE);
  }
  
  // free cache buffer
  free(pBuffer);
  pBuffer = NULL;

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFMMWave::FileRecordingAdd(char *pBuffer, DWORD dwBuffSize) {
  
  DWORD dwBytesWritten= 0;
  BOOL bResult = FALSE;

  char pSmallBuffer[4];
  DWORD dwSmallBufferSize = 4;
  DWORD dwSmallBuffPos = 0;
  DWORD dwSmallBytesWritten = 0;
  
  // append data to the file
  bResult= WriteFile(m_hFile, pBuffer, dwBuffSize, &dwBytesWritten, NULL);
  if ((!bResult)||(dwBuffSize != dwBytesWritten)) {
    return(FALSE);
  }

  // update wave values
  m_ulFileLength+=  dwBytesWritten;
  m_dwBlockCount= (m_ulFileLength - 44) / m_usBlockAlign;
  // calculate time-length of wave
  m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);

  // correct chunksize values
  // chunksize = file length - 8
  WriteULONG(pSmallBuffer, dwSmallBufferSize, &dwSmallBuffPos, m_ulFileLength - 8);
  dwSmallBuffPos = 0;
  SetFilePointer(m_hFile, 4, NULL, FILE_BEGIN);
  WriteFile(m_hFile, pSmallBuffer, dwSmallBufferSize, &dwSmallBytesWritten, NULL);
  
  // chunksize1 = 16
  WriteULONG(pSmallBuffer, dwSmallBufferSize, &dwSmallBuffPos, 16);
  dwSmallBuffPos = 0;
  SetFilePointer(m_hFile, 16, NULL, FILE_BEGIN);
  WriteFile(m_hFile, pSmallBuffer, dwSmallBufferSize, &dwSmallBytesWritten, NULL);
  
  // chunksize2 = file length - 44
  WriteULONG(pSmallBuffer, dwSmallBufferSize, &dwSmallBuffPos, m_ulFileLength - 44);
  dwSmallBuffPos = 0;
  SetFilePointer(m_hFile, 40, NULL, FILE_BEGIN);
  WriteFile(m_hFile, pSmallBuffer, dwSmallBufferSize, &dwSmallBytesWritten, NULL);

  // move file pointer to end
  SetFilePointer(m_hFile, 0, NULL, FILE_END);

  FlushFileBuffers(m_hFile);

  return(TRUE);
}


//--------------------------------------------------------------------------------
BOOL CFMMWave::FileRecordingStop() 
{
  // flush the data buffers
  FlushFileBuffers(m_hFile);
  
  // finally close file
  CloseHandle(m_hFile);

  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Fills the buffer "pData" with the number of samples specified by "dwCount".
/// Return value: number of actual BLOCKS in "pData"
DWORD CFMMWave::GetSamples(char *pData, DWORD dwPos, DWORD dwCount)
{
  DWORD dwBlocks   = 0;
  DWORD dwCopy     = 0;
  DWORD dwOffs     = 0;
  DWORD dwDestPos  = 0;
  DWORD dwSourcePos= 0;

  // determine number of blocks we really can retrieve
  dwBlocks= min(dwCount, m_dwBlockCount - dwPos);
 
  // find position in sample list
  while(dwPos >= m_pSampleIt->GetSkip() + m_pSampleIt->GetBlocksLen()) {
    m_pSampleIt->IncSkip(m_pSampleIt->GetBlocksLen());
    m_pSampleIt->Next();
  }

  // copy the samples
  while(dwOffs < dwBlocks) {
    // calculate position in destination buffer
    dwDestPos= dwOffs * m_usBlockAlign;    

    // calculate position in source buffer
    dwSourcePos= (dwPos - m_pSampleIt->GetSkip() + m_pSampleIt->GetBlockStart()) * m_usBlockAlign;

    dwCopy= min(m_pSampleIt->GetBlocksLen(), dwBlocks);    

    memcpy(pData + dwDestPos, &m_pWaveData[dwSourcePos], dwCopy * m_usBlockAlign);
    m_pSampleIt->IncOffs(dwCopy);
    dwOffs+= dwCopy;
    if (dwBlocks != dwOffs) m_pSampleIt->Next();
  }
  
  return(dwBlocks);
}

//--------------------------------------------------------------------------------
/// Returns the sample value for the given position.
int CFMMWave::GetSampleAt(DWORD dwBlockIndex)
{
  int iRet= 0;
  char szData[2];

  if (dwBlockIndex < m_dwBlockCount) {
    GetSamples(szData, dwBlockIndex, 1);

    if (m_usBitsPerSample == 8) {
      iRet= -(BYTE)szData[0] + 128;    
    } else {
      iRet= szData[0] + (szData[1] << 8);
    }
    return(iRet);
  }
  return(iRet);
}

