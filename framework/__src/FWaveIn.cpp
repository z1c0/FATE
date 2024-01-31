// FWaveIn.cpp: implementation of the CFWaveIn class.
//////////////////////////////////////////////////////////////////////
#include "FWaveIn.h"
#include "FateApp.h"


//------------------------------------------------------------------------------
CFWave* CFWaveIn::m_wave;
WAVEFORMATEX CFWaveIn::m_waveFmt;
HWAVEIN CFWaveIn::m_hwi;
PWAVEHDR CFWaveIn::m_pWaveHdr[MAX_BUFFERS]; 
char* CFWaveIn::m_pSaveBuffer;
DWORD CFWaveIn::m_dwDataLength;
WAVEIN_STATUS CFWaveIn::m_flagStatus;
DataArrivedCallBackFunc CFWaveIn::m_callback;
LPVOID CFWaveIn::m_pvUserData;
WNDCLASS CFWaveIn::m_wc;
HWND CFWaveIn::m_hWnd;
DWORD CFWaveIn::m_dwRecordedTime;
int CFWaveIn::m_iBufferCount;
DWORD CFWaveIn::m_dwBufferSize;
DWORD CFWaveIn::m_dwBufferLength;
BOOL CFWaveIn::m_bRecordToFile;
LPCTSTR CFWaveIn::m_pszFullFileName;
BOOL CFWaveIn::m_bInit;
BOOL CFWaveIn::m_bPaused;


//--------------------------------------------------------------------------------
BOOL CFWaveIn::Record(LPCTSTR pszFilename)
{
  if (!m_bInit) return (FALSE); // check initiation
  
  m_bRecordToFile = TRUE;
  m_pszFullFileName = pszFilename;
  return(Record());
}

//--------------------------------------------------------------------------------
/// Pauses or resumes the recording process.
BOOL CFWaveIn::Pause()
{
  if (!m_bInit) return (FALSE); // check initiation

  m_bPaused= !m_bPaused;
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Starts the recording process.
BOOL CFWaveIn::Record()
{
  if (!m_bInit) return (FALSE); // check initiation
  
  BOOL bResult=FALSE;
  // SET VARIABLES AND ALLOCATE MEMORY
  
  // safe-delele old CFWave object and allocate a new one
  SAFE_DELETE(m_wave);
  m_wave= new CFWave();
  m_wave->SetWaveFormat(&m_waveFmt); 
  
  // allocate wavehdr blocks
  for (int i = 0; i < m_iBufferCount; i++) {
    m_pWaveHdr[i] = (WAVEHDR*)malloc(sizeof(WAVEHDR));
    if (m_pWaveHdr[i]) {
      m_pWaveHdr[i]->lpData         = (char*)malloc(m_dwBufferSize);
      m_pWaveHdr[i]->dwBufferLength = m_dwBufferSize;
      m_pWaveHdr[i]->dwBytesRecorded= 0;
      m_pWaveHdr[i]->dwUser         = 0;
      m_pWaveHdr[i]->dwFlags        = 0;
      m_pWaveHdr[i]->dwLoops        = 1;
      m_pWaveHdr[i]->lpNext         = NULL;
      m_pWaveHdr[i]->reserved       = 0;
    }
  }
  
  //allocate save buffer
  m_pSaveBuffer= NULL;
  m_iBufferCount = MAX_BUFFERS;
  m_dwDataLength= 0;

  // OPEN DEVICE AND START RECORDING
  if(OpenInputDevice()) {
    m_flagStatus= WAVEIN_RECORDING;
    bResult= StartRecording();
  } else {
    CleanUpBuffers();
    m_flagStatus= WAVEIN_READY;
    return(FALSE);
  }
  return(bResult);
}


//--------------------------------------------------------------------------------
/// Stops the recording process.
BOOL CFWaveIn::Stop()
{
  BOOL bRes;
  if (!m_bInit) return (FALSE); // check initiation
  bRes = StopRecording();
  return(bRes);
}

//--------------------------------------------------------------------------------
/// Installs a callback function which will be called each time an new buffer was
/// filled by the wavein-device.
/// The argument "LPVOID pvUser" is passed on to the callback function.
void CFWaveIn::InstallCallBack(DataArrivedCallBackFunc callback, LPVOID pvUserData)
{
  m_callback= callback;
  m_pvUserData= pvUserData; // fixed data
}

//--------------------------------------------------------------------------------
/// Opens the Audio Input Device
BOOL CFWaveIn::OpenInputDevice()
{
  if (!m_bInit) return (FALSE); // check initiation

  //open wave device
  if (waveInOpen(&m_hwi, WAVE_MAPPER, &m_waveFmt, (DWORD)m_hWnd, 0, CALLBACK_WINDOW) != MMSYSERR_NOERROR) {
    m_flagStatus= WAVEIN_READY;
    return(TRUE);
  }
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Starts Recording
BOOL CFWaveIn::StartRecording()
{
  if (!m_bInit) return (FALSE); // check initiation
  
  BOOL bResult;
  MMRESULT rc;

  // prepare and add buffers
  for (int i=0; i < m_iBufferCount; i++) {
    rc = waveInPrepareHeader(m_hwi, m_pWaveHdr[i], sizeof(WAVEHDR));
    if (rc == MMSYSERR_NOERROR) {
      waveInAddBuffer(m_hwi, m_pWaveHdr[i], sizeof(WAVEHDR));
    } else {
      StopRecording();
      return (FALSE);
    }
  }
  
  // if record to file, start file recording 
  if (m_bRecordToFile) 
    bResult = m_wave->FileRecordingStart(m_pszFullFileName);
  if (!bResult) return FALSE;

  // start recording
  rc = waveInStart(m_hwi);
  if (rc == MMSYSERR_NOERROR) 
    return (TRUE);
  else
    return (FALSE);
}

//--------------------------------------------------------------------------------
/// Stops Recording
BOOL CFWaveIn::StopRecording()
{  
  // stop recording
  waveInReset(m_hwi);

  m_bPaused= FALSE;

  // unprepare buffers
  for (int i=0; i<m_iBufferCount; i++) 
    waveInUnprepareHeader(m_hwi, m_pWaveHdr[i], sizeof(WAVEHDR));

  if (m_bRecordToFile) { //if record to file, stop file recording
    m_wave->FileRecordingStop();
  } else { //else move memory buffer into object
    // pass big buffer to wave object
    if ((m_pSaveBuffer)&&(m_dwDataLength)) {
      m_wave->SetWaveData(m_pSaveBuffer, m_dwDataLength); //copy blocks to CFWave object
    }
  }

  CleanUpBuffers();

  m_flagStatus= WAVEIN_READY;

  // close waveIn
  if (waveInClose(m_hwi) != MMSYSERR_NOERROR)
    return(FALSE);
  
  m_hwi= NULL;
 
  m_dwRecordedTime = m_wave->GetWaveTime();
  
  m_bRecordToFile= FALSE;  
  m_pszFullFileName = NULL;
  m_flagStatus= WAVEIN_READY;

  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFWaveIn::SetBufferLength(DWORD dwBufferLength) 
{
  m_dwBufferLength = dwBufferLength;
  CalculateBuffers();
};

//--------------------------------------------------------------------------------
/// Cleans Up Buffers
BOOL CFWaveIn::CleanUpBuffers()
{
  for (int i=0; i < m_iBufferCount; i++) {
    if (m_pWaveHdr[i] != NULL) {
      free (m_pWaveHdr[i]->lpData);
      m_pWaveHdr[i]->lpData= NULL;
      free (m_pWaveHdr[i]);
      m_pWaveHdr[i]= NULL;
    }
  }
  if (m_pSaveBuffer != NULL ) {
    free(m_pSaveBuffer);
    m_pSaveBuffer = NULL;
  }
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Resets some variables
BOOL CFWaveIn::Reset()
{
  m_flagStatus= WAVEIN_READY;
  SAFE_DELETE(m_wave);
  m_pSaveBuffer= NULL;
  m_dwDataLength= 0;
  m_dwRecordedTime = 0;
  m_bRecordToFile = FALSE;  // record to memory by default
  m_pszFullFileName = NULL;
  return(TRUE);
}


//--------------------------------------------------------------------------------
/// Initialize the device
BOOL CFWaveIn::Initialize()
{
  if (m_bInit) return FALSE; // dont init twice!
  // inits
  m_flagStatus     = WAVEIN_READY;
  m_wave           = NULL;
  m_pSaveBuffer    = NULL;
  m_dwDataLength   = 0;
  m_dwRecordedTime = 0;
  m_bRecordToFile  = FALSE;  // record to memory by default
  m_pszFullFileName= NULL;
  m_bPaused        = FALSE;
  
  // preferred buffer lenght in ms
  m_dwBufferLength= 200; 
  m_iBufferCount= MAX_BUFFERS;
  
  // set default-wave-format
  SetDefaultWaveFormat();

  CalculateBuffers();

  // make sure memory is clean
  for (int i = 0; i < m_iBufferCount; i++) {
    m_pWaveHdr[i] = NULL; 
  }

  //init and register window to handle buffer fill messages by the driver
  m_wc.style			  = CS_HREDRAW | CS_VREDRAW;
  m_wc.lpfnWndProc	= (WNDPROC)RecordProc;
  m_wc.cbClsExtra		= 0;
  m_wc.cbWndExtra		= 0;
  m_wc.hInstance		= 0; 
  m_wc.hIcon			  = NULL;
  m_wc.hCursor			= 0;
  m_wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
  m_wc.lpszMenuName	= 0;
  m_wc.lpszClassName= TEXT("RecordEventWnd"); 
	
  RegisterClass(&m_wc);

  // this window is necesary to handle device events
  // to save data and preapare new buffers
  m_hWnd= CreateWindow(TEXT("RecordEventWnd"), TEXT(""), WS_POPUP, 0, 0,
                       GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
			                 NULL, NULL, 0, NULL);
  m_bInit = TRUE;
  return(TRUE);
}

//--------------------------------------------------------------------------------
/// Inits the WAVEFORMATEX structure with default values.
void CFWaveIn::SetDefaultWaveFormat()
{
  HKEY hKey;
  PBYTE pBuffer= NULL;
  DWORD dwDataSize= 0;
  DWORD dwType; 
  LONG lResult;

  m_waveFmt.wFormatTag     = WAVE_FORMAT_PCM;    // PCM 
  m_waveFmt.nChannels      = 1; 
  m_waveFmt.nSamplesPerSec = 11025;
  m_waveFmt.wBitsPerSample = 8;
  m_waveFmt.nBlockAlign    = m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample / 8);
  m_waveFmt.nAvgBytesPerSec= m_waveFmt.nSamplesPerSec * m_waveFmt.nBlockAlign;

  // get defaults from registry
  RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Multimedia\\Audio\\WaveFormats"), 0, 0, &hKey );
  RegQueryValueEx(hKey, TEXT("WceName"), 0, &dwType,(PBYTE)NULL, &dwDataSize); // get size of key
  pBuffer= (BYTE*)malloc(dwDataSize);
  lResult= RegQueryValueEx(hKey, TEXT("WceName"), 0, &dwType, (PBYTE)pBuffer, &dwDataSize); 
  RegCloseKey(hKey); 

  if (lResult == ERROR_SUCCESS)
    memcpy(&m_waveFmt, pBuffer, dwDataSize);
  free(pBuffer);

  // calculate optimum buffer number and size
  CalculateBuffers();
}

//--------------------------------------------------------------------------------
/// Set Manually WAVEFORMATEX structure.
void CFWaveIn::SetWaveFormat(LPWAVEFORMATEX pWavefmt) { 
  HKEY hKey;
  LONG lResult;
  
  // copy structure
  memcpy(&m_waveFmt, pWavefmt, sizeof(WAVEFORMATEX)); 

  // calculate optimum buffer number and size
  CalculateBuffers();

  // save settings in registry
  lResult= RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Multimedia\\Audio\\WaveFormats"), 0, 0, &hKey);
  if (lResult == ERROR_SUCCESS) {
    RegSetValueEx(hKey, TEXT("WceName"), 0, REG_BINARY, (BYTE*)pWavefmt, sizeof(BYTE)*18); 
    RegCloseKey(hKey); 
  }
}


//--------------------------------------------------------------------------------
/// Initialize the device
BOOL CFWaveIn::Finalize()
{
  if (m_bInit) {
    if(IsRecording()) {
      Stop();
    }
    SAFE_DELETE(m_wave);
    CleanUpBuffers();
    m_bInit = FALSE;
    return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------------------
/// Window Message Handler for Buffer is Done Event
/// This Funcions pass the filled buffer to the Temporal Buffer and prepares it to receive
///  new data
LRESULT CALLBACK CFWaveIn::RecordProc(HWND hwnd, UINT uMsg, WPARAM dwInstance, LPARAM dwParam)
{
  // cast back parameters 
  PWAVEHDR pWaveHdr= (PWAVEHDR)dwParam;
  char* pTempBuffer = NULL;
  BOOL bResult = FALSE;

  switch (uMsg) {
    case MM_WIM_DATA :
      // check if we are in PAUSE mode
      if (m_bPaused) {
        //prepare buffer
        if (waveInPrepareHeader(m_hwi, pWaveHdr, sizeof(WAVEHDR)) == MMSYSERR_NOERROR) {
          waveInAddBuffer(m_hwi, pWaveHdr, sizeof(WAVEHDR));
        } else {
          //m_flagStatus= WAVEIN_ERROR;          
          Stop();
          return(DefWindowProc(hwnd, uMsg, dwInstance, dwParam));
        }
        break;
      }

      // avoid race conditions ... when "RecordProc" is called after "Stop"
      if (!m_hwi) {
        m_callback(NULL, 0, m_pvUserData);
        break;  
      }

      // calculate recorded time
      m_dwRecordedTime = (m_dwDataLength + pWaveHdr->dwBytesRecorded) / (m_wave->GetBytesPerSec()/1000);
      
      // callback specified?
      if (m_callback) {
        m_callback(pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, m_pvUserData);
      }

      if (m_bRecordToFile) { //if record to file, add buffer to file
        bResult= m_wave->FileRecordingAdd(pWaveHdr->lpData, pWaveHdr->dwBytesRecorded);
        if (!bResult) {
          //m_flagStatus= WAVEIN_ERROR;          
          m_wave->FileRecordingStop();
          Stop();
          return(DefWindowProc(hwnd, uMsg, dwInstance, dwParam));
        }
      } else { //else add data buffer to big memory buffer
        //allocate buffer
        pTempBuffer= (char*)realloc(m_pSaveBuffer, m_dwDataLength + pWaveHdr->dwBytesRecorded);
        if (pTempBuffer == NULL) {
          //m_flagStatus= WAVEIN_ERROR;          
          Stop();
          return(DefWindowProc(hwnd, uMsg, dwInstance, dwParam));
        }
        //pass data
        m_pSaveBuffer= pTempBuffer;
        memcpy(m_pSaveBuffer + m_dwDataLength, pWaveHdr->lpData, pWaveHdr->dwBytesRecorded);
      }

      m_dwDataLength+= pWaveHdr->dwBytesRecorded;

      //prepare buffer
      if (waveInPrepareHeader(m_hwi, pWaveHdr, sizeof(WAVEHDR)) == MMSYSERR_NOERROR) {
        waveInAddBuffer(m_hwi, pWaveHdr, sizeof(WAVEHDR));
      } else {
        //m_flagStatus= WAVEIN_ERROR;          
        Stop();
        return(DefWindowProc(hwnd, uMsg, dwInstance, dwParam));
      }
      break;

    case MM_WIM_CLOSE:
      break;

    case MM_WIM_OPEN:
      break;
    default:
      break;
  }
  return(DefWindowProc(hwnd, uMsg, dwInstance, dwParam));
}

//--------------------------------------------------------------------------------
// Calculate Optimum Buffer Size acording to existing Buffer Length
void CFWaveIn::CalculateBuffers()
{
  m_dwBufferSize = (m_dwBufferLength * m_waveFmt.nAvgBytesPerSec) / 1000;  
  // aproximate to closest 2^n, good for processing with FFT
  // m_dwBufferSize = pow(2, ceil(log(m_dwBufferSize)/log(2)));   
}
