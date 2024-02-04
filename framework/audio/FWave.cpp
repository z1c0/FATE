// FWave.cpp: implementation of the CFWave class.
//////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include "FWave.h"
#include "../base/FateApp.h"


//--------------------------------------------------------------------------------
CFWave::CFWave()
{
  m_pWaveData= NULL;
  m_pUndo    = NULL;
  Init();
}

//--------------------------------------------------------------------------------
CFWave::CFWave(LPCTSTR pszFileName)
{
  m_pWaveData= NULL;
  m_pUndo    = NULL;
  Init();
  Load(pszFileName);
}

//--------------------------------------------------------------------------------
CFWave::~CFWave()
{
  // safe-release wave data
  if (m_hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hFile);
    m_hFile= INVALID_HANDLE_VALUE;
  }
  
  if (m_pWaveData) {
    free(m_pWaveData);
    m_pWaveData = NULL;
  }

  if (m_pUndo) {
    free(m_pUndo);
    m_pUndo = NULL;
  }
}

//--------------------------------------------------------------------------------
void CFWave::Init()
{
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
  m_hFile= INVALID_HANDLE_VALUE;
  m_dwFilePos= 0;

  m_dwBlockCount= 0;
  m_dwWaveTime  = 0; //total wave time in seconds
  m_ulFileLength= 0;
  
  memset(&m_wavefmt, 0, sizeof(WAVEFORMATEX));
  
  if (m_pWaveData) free(m_pWaveData);
  m_pWaveData= NULL;
  if (m_pUndo) free(m_pUndo);
  m_pUndo= NULL;
 
  m_dwBlocksRemoved= 0;
}

//--------------------------------------------------------------------------------
/// Loads a wave-file from the specified location.
BOOL CFWave::Load(LPCTSTR pszFileName)
{
  HANDLE hFile= NULL;
  BOOL bRet= FALSE;
  char *pBuffer= NULL;
  DWORD dwBuffPos= 0;
  BOOL bParse= TRUE;
  DWORD dwBytesRead= 0;
  ULONG ulFileLength= 0;
  
  // clear memory
  Clear();

  // try to open the file
  hFile= CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
                    FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    ulFileLength= GetFileSize(hFile, NULL);
    if (ulFileLength == 0xFFFFFFFF) {
      // reset values
      Init();
      return(FALSE);
    }

    // allocate buffer
    pBuffer= (char*)malloc(ulFileLength);
    
    // cache the file in a buffer
    bRet= ReadFile(hFile, pBuffer, ulFileLength, &dwBytesRead, NULL);
    if ((!bRet)||(ulFileLength != dwBytesRead)) {
      CloseHandle(hFile);
      free(pBuffer);
      pBuffer = NULL;
      // reset values
      Init();
      return(FALSE);
    }
    CloseHandle(hFile);
    
    // validate "RIFF" sequence
    if (memcmp(&pBuffer[dwBuffPos], "RIFF", 4)) {
      free(pBuffer);
      pBuffer = NULL;
      // reset values
      Init();
      return(FALSE);
    }
    dwBuffPos+= 4;

    // read file-length
    m_ulFileLength= ReadULONG(pBuffer, &dwBuffPos);
    m_ulFileLength+= 8;
    if (m_ulFileLength != ulFileLength) {
      // just correct this error ... don't fail
      dwBuffPos-= 4;
      WriteULONG(pBuffer, ulFileLength, &dwBuffPos, ulFileLength - 8);
    }
    
    // validate "WAVE" sequence
    if (memcmp(&pBuffer[dwBuffPos], "WAVE", 4)) {
      free(pBuffer);
      pBuffer = NULL;
      // reset values
      Init();
      return(FALSE);
    }
    dwBuffPos+= 4;

    // parse the chunks
    while (bParse) {
      // what kind of chunk was read?
      if (!memcmp(&pBuffer[dwBuffPos], "fmt ", 4)) {
        // "fmt " chunk
        dwBuffPos+= 4;
        bRet= ReadFmtChunk(pBuffer, &dwBuffPos);
        
      } else if (!memcmp(&pBuffer[dwBuffPos], "data", 4)) {      
        // "data" chunk
        dwBuffPos+= 4;
        bRet= ReadDataChunk(pBuffer, &dwBuffPos);
        // after reading the data cunk we are done
        bParse= FALSE;
      
      } else {
        // unknown or unsupported chunk encountered        
        dwBuffPos+= 4;
        bRet= SkipChunk(pBuffer, &dwBuffPos);
      }

      // encountered a problem?
      if (!bRet) {
        free(pBuffer);
        pBuffer = NULL;
        // reset values
        Init();
        return(FALSE);
      }
    }    
    
    free(pBuffer);
    pBuffer = NULL;    
    
    // set the filename and we are done
    _tcscpy(m_szFileName, pszFileName);
    
    // finally, set flag that loading succeeded
    m_bLoaded= TRUE;
    return(TRUE);  
  }
  
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which reads an ULONG number from the specified buffer starting
/// at "dwBuffPos".
ULONG CFWave::ReadULONG(char *pBuffer, LPDWORD pdwBuffPos)
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
USHORT CFWave::ReadUSHORT(char *pBuffer, LPDWORD pdwBuffPos)
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
BOOL CFWave::ReadFmtChunk(char *pBuffer, LPDWORD pdwBuffPos)
{
  DWORD dwRet= 0;
  DWORD dwBytesTotal= 0;
  ULONG ulFmtDataLength= 0;

  // read length of "fmt" data (should be 16)
  ulFmtDataLength= ReadULONG(pBuffer, pdwBuffPos);
  
  // read encoding tag - only PCM (uncompressed format) is supported
  m_usFormatTag= ReadUSHORT(pBuffer, pdwBuffPos);
  if (m_usFormatTag != WAVE_FORMAT_PCM) {
    return(FALSE);
  }
  m_wavefmt.wFormatTag= m_usFormatTag;
  dwBytesTotal+= 2;  // increase byte counter

  // read channel information (mono / stereo)
  m_usChannels= ReadUSHORT(pBuffer, pdwBuffPos);
  if ((m_usChannels != 1)&&(m_usChannels != 2)) {
    return(FALSE);
  } 
  m_wavefmt.nChannels= m_usChannels;
  dwBytesTotal+= 2;  // increase byte counter

  // read sampling rate
  m_ulSampleRate= ReadULONG(pBuffer, pdwBuffPos);
  if (!m_ulSampleRate) {
    return(FALSE);
  } 
  m_wavefmt.nSamplesPerSec= m_ulSampleRate;
  dwBytesTotal+= 4;  // increase byte counter

  // read bytes per second
  m_ulBytesPerSec= ReadULONG(pBuffer, pdwBuffPos);
  if (!m_ulBytesPerSec) {
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
BOOL CFWave::ReadDataChunk(char *pBuffer, LPDWORD pdwBuffPos)
{
  ULONG ulSampleDataLength= 0;

  // read length of the sample data
  ulSampleDataLength= ReadULONG(pBuffer, pdwBuffPos);
  if (!ulSampleDataLength) {
    return(FALSE);
  }

  //read all samples
  if (SetWaveData(&pBuffer[*pdwBuffPos], ulSampleDataLength))
    return(TRUE);
  return(FALSE);

}

//--------------------------------------------------------------------------------
/// Auxiliary method which skips a chunk which is unknown or not supported.
BOOL CFWave::SkipChunk(char *pBuffer, LPDWORD pdwBuffPos)
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
BOOL CFWave::SaveAs(LPCTSTR pszFileName)
{
  // is this a valid (correctly loaded) wave file?
  if (m_bLoaded) {
    HANDLE hFile;
    BOOL bRet= FALSE;
    char *pBuffer= NULL;
    DWORD dwBuffPos= 0;
    DWORD dwBytesWritten= 0;

    // allocate a buffer for caching the wave data
    pBuffer= (char*)malloc(m_ulFileLength);
    if (!pBuffer) return(FALSE);
    
    // write "RIFF" sequence
    if (dwBuffPos + 4 > m_ulFileLength) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
    memcpy(&pBuffer[dwBuffPos], "RIFF", 4);
    dwBuffPos+= 4;

    // write file length - 8
    if (!WriteULONG(pBuffer, m_ulFileLength, &dwBuffPos, m_ulFileLength - 8)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
  
    // write "WAVE" sequence
    if (dwBuffPos + 4 > m_ulFileLength) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
    memcpy(&pBuffer[dwBuffPos], "WAVE", 4);
    dwBuffPos+= 4;
  
    // write the fmt chunk
    if (!WriteFmtChunk(pBuffer, m_ulFileLength, &dwBuffPos)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }

    // write the data chunk
    if (!WriteDataChunk(pBuffer, m_ulFileLength, &dwBuffPos)) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }

    // finally write the cached data to the actual file
    hFile= CreateFile(pszFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
    bRet= WriteFile(hFile, pBuffer, m_ulFileLength, &dwBytesWritten, NULL);
    if ((!bRet)||(m_ulFileLength != dwBytesWritten)) {
      CloseHandle(hFile);
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }

    // close file and release cache buffer
    CloseHandle(hFile);
    free(pBuffer);
    pBuffer = NULL;

    return(TRUE);  
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes a "FMT" chunk to the specified buffer and 
/// increases the position index for the buffer. A check on available buffer 
/// size is done.
BOOL CFWave::WriteFmtChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos) 
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
BOOL CFWave::WriteDataChunk(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos) 
{
  if (pBuffer) {
    DWORD dwSampleData= 0;

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
    if (*pdwBuffPos + dwSampleData > m_ulFileLength) {
      free(pBuffer);
      pBuffer = NULL;
      return(FALSE);
    }
    
    // copy raw samples
    memcpy(&pBuffer[*pdwBuffPos], m_pWaveData, dwSampleData);
    *pdwBuffPos+= dwSampleData;
    
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Auxiliary method which writes an ULONG to the specified buffer and increases
/// position index for the buffer. A check on available buffer size is done.
BOOL CFWave::WriteULONG(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, ULONG ulVal)
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
BOOL CFWave::WriteUSHORT(char *pBuffer, DWORD dwBuffSize, LPDWORD pdwBuffPos, USHORT usVal)
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
BOOL CFWave::Cut(DWORD dwLeft, DWORD dwRight, BOOL bInside /* = TRUE */)
{
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
    if ((dwLeft==dwRight)) return(FALSE);
    if ((dwLeft==0)&&(dwRight==m_dwBlockCount)) return(FALSE);
    
    // is cutting done inside or outside the borders?
    if (bInside) {
      dwLeft++;  // block of left border will remain in buffer
      if (dwLeft == dwRight) return(TRUE);

      // safe-release the undo buffer
      if (m_pUndo) {
        free(m_pUndo);
        m_pUndo = NULL;
      }
      // store old data in undo buffer
      m_pUndo= m_pWaveData;

      // allocate memory for new sample data
      m_pWaveData= (char*)malloc((m_dwBlockCount - dwRight + dwLeft) * m_usBlockAlign);
      if (!m_pWaveData) return(FALSE); 
      
      // copy samples
      memcpy(m_pWaveData, m_pUndo, dwLeft * m_usBlockAlign);
      memcpy(&m_pWaveData[dwLeft * m_usBlockAlign], 
             &m_pUndo[dwRight * m_usBlockAlign], 
             (m_dwBlockCount - dwRight) * m_usBlockAlign);

      // store UNDO information
      m_dwBlocksRemoved= dwRight - dwLeft;
      
      // correct some values
      m_dwBlockCount-= m_dwBlocksRemoved;
      m_ulFileLength= m_dwBlockCount * m_usBlockAlign + 44;
      m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    
    } else {  // OUTSIDE CUT
      dwRight++;  //  block of right border will remain in buffer
      
      // safe-release the undo buffer
      if (m_pUndo) {
        free(m_pUndo);
        m_pUndo = NULL;
      }
      // store old data in undo buffer
      m_pUndo= m_pWaveData;

      // allocate memory for new sample data
      m_pWaveData= (char*)malloc((dwRight - dwLeft) * m_usBlockAlign);
      if (!m_pWaveData) return(FALSE);

      // copy samples
      memcpy(m_pWaveData, &m_pUndo[dwLeft * m_usBlockAlign], 
             (dwRight - dwLeft) * m_usBlockAlign);

      // store UNDO information
      m_dwBlocksRemoved= m_dwBlockCount - dwRight + dwLeft;
      
      // correct some values
      m_dwBlockCount-= m_dwBlocksRemoved;
      m_ulFileLength= m_dwBlockCount * m_usBlockAlign * m_usChannels + 44;
      m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    }
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// "Undoes" the last cutting operations unsing the object's undo-buffer(s).
BOOL CFWave::Undo()
{
  // is this a valid (correctly loaded) wave file?
  if ((m_bLoaded)&&(m_pUndo)) {   
    if (m_pWaveData) {
      free(m_pWaveData);
      m_pWaveData = NULL;
    }
    m_pWaveData= m_pUndo;
    m_pUndo= NULL;
    
    // correct some values
    m_ulFileLength+= m_dwBlocksRemoved * m_usBlockAlign;
    m_dwBlockCount+= m_dwBlocksRemoved;
    m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
    m_dwBlocksRemoved= 0;

    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
void CFWave::SetWaveFormat(LPWAVEFORMATEX pWavefmt)
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
/// Manually sets the sample data of the CFWave-object.
/// NOTE: A call to "SetWaveFormat(LPFORMATEX pWavefmt)" is necessary to ensure
///       that that the resulting CFWave is valid.
BOOL CFWave::SetWaveData(char *pWave, ULONG ulSampleDataLength)
{
  // store number of blocks in wave
  m_dwBlockCount= ulSampleDataLength / (m_usBlockAlign);

  // calculate time-length of wave
  m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);
  m_ulFileLength= ulSampleDataLength + 44;

  // (clear memory
  if (m_pWaveData) {
    free(m_pWaveData);
    m_pWaveData = NULL;
  }
  
  m_pWaveData= (char*)malloc(ulSampleDataLength);
  if (!m_pWaveData) return(FALSE);
  memcpy(m_pWaveData, pWave, ulSampleDataLength);
    
  m_bLoaded= TRUE;
  return(TRUE);
}


//--------------------------------------------------------------------------------
// Convert wave to mono audio format, keeping left channel
// TO DO possibility to select which channel to keep
BOOL CFWave::ConvertToMono()
{
  WAVEFORMATEX wavefmt;
  ULONG ulDataLength;

  //check if the wave file is stereo
  if (m_usChannels == 1)
    return(TRUE);

  //correct wave format
  wavefmt.wFormatTag     = m_wavefmt.wFormatTag;
  wavefmt.nChannels      = 1; 
  wavefmt.nSamplesPerSec = m_wavefmt.nSamplesPerSec;
  wavefmt.wBitsPerSample = m_wavefmt.wBitsPerSample;
  wavefmt.nBlockAlign    = (m_wavefmt.wBitsPerSample / 8);
  wavefmt.nAvgBytesPerSec= wavefmt.nSamplesPerSec * wavefmt.nBlockAlign;
  SetWaveFormat(&wavefmt); //set format
  
  // calculate new data lenght
  ulDataLength = m_dwBlockCount * m_usBlockAlign;

  //allocate new temporal memory
  char *pTempSamples;
  pTempSamples= (char*)malloc(ulDataLength);
 
  // walk through old data and copy only left side info
  int j = 0;  
  for (UINT i= 0; i < ulDataLength * 2; i+= m_usBlockAlign * 2) {           
    memcpy(&pTempSamples[j], &m_pWaveData[i], m_usBlockAlign); // &m_pWaveData[i + 1] to get right channel
    j+= m_usBlockAlign;
  }

  // set new blocks
  SetWaveData(pTempSamples, ulDataLength);

  free(pTempSamples);
  pTempSamples = NULL;
  
  return(TRUE);
}


//--------------------------------------------------------------------------------
BOOL CFWave::FileRecordingStart(LPCTSTR pszFileName) 
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
    m_hFile= INVALID_HANDLE_VALUE;
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
BOOL CFWave::FileRecordingAdd(char *pBuffer, DWORD dwBuffSize) 
{  
  DWORD dwBytesWritten= 0;
  BOOL bResult = FALSE;
  char pSmallBuffer[4]; // small buffer to store size info
  DWORD dwSmallBufferSize = 4;
  DWORD dwSmallBuffPos = 0;
  DWORD dwSmallBytesWritten = 0;
  int iWriteAttempts= 0;
  DWORD dwBytesToWrite= dwBuffSize;
  DWORD dwOldFileSize= GetFileSize(m_hFile, NULL);
  
  // append data to the file
  while(TRUE) {
    bResult= WriteFile(m_hFile, pBuffer, dwBytesToWrite, &dwBytesWritten, NULL);
    if ((!bResult)||(dwBytesToWrite != dwBytesWritten)) {
      dwBytesToWrite-= dwBytesWritten;

      Sleep(10);
      if (iWriteAttempts++ == 5) {
        SetFilePointer(m_hFile, dwOldFileSize, NULL, FILE_BEGIN);
        SetEndOfFile(m_hFile);
        return (FALSE);
      }
    } else break;  // OKE
  }

  // update wave values
  m_ulFileLength+= dwBuffSize;
  m_dwBlockCount= (m_ulFileLength - 44) / (m_usBlockAlign);
  // calculate time-length of wave
  m_dwWaveTime= (m_dwBlockCount * m_usBlockAlign) / (m_ulBytesPerSec/1000);

  // correct chucksize values
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
BOOL CFWave::FileRecordingStop() 
{
  // flush the data buffers
  FlushFileBuffers(m_hFile);
  
  // finally close file
  CloseHandle(m_hFile);
  m_hFile= INVALID_HANDLE_VALUE;

  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFWave::Clear()
{
  // Clear all Data  
  m_bLoaded= FALSE;
  m_szFileName[0]= 0;
  
  m_usFormatTag    = 0;
  m_usChannels     = 0;
  m_ulSampleRate   = 0;
  m_ulBytesPerSec  = 0;
  m_usBlockAlign   = 0;
  m_usBitsPerSample= 0;
  m_iBytesPerSample= 0;
  m_dwFilePos      = 0;
  m_dwBlockCount   = 0;
  m_dwWaveTime     = 0;
  m_ulFileLength   = 0;
  m_dwBlocksRemoved= 0;
  
  // record to file values
  if (m_hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hFile);
    m_hFile = INVALID_HANDLE_VALUE;
  }  
  
  memset(&m_wavefmt, 0, sizeof(WAVEFORMATEX));
  
  SAFE_DELETE(m_pWaveData);
  SAFE_DELETE(m_pUndo);
}
