// FRecordPanel.cpp: implementation of the CFRecordPanel class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FRecordPanel.h"
#include "../../framework/audio/FWaveIn.h"
#include "Resource.h"
#include <stdlib.h>

//--------------------------------------------------------------------------------
// Callback function for arrived sample data.
void RecordCallbackFunc(char *pData, DWORD dwSize, LPVOID pvUserData)
{
  int iVal = 0;
  int iValMax = 0;

  DWORD dwIndex = 0;
  
  // cast back parameter
  CFRecordPanel *pRecordPanel= (CFRecordPanel*)pvUserData;
  CFWaveCutApp *pApp= (CFWaveCutApp*)pRecordPanel->GetParent();  

  // get some values
  DWORD dwBlockAlign = CFWaveIn::GetWave()->GetBlockAlign();
  WORD wChannels = CFWaveIn::GetWave()->GetChannels();
  int iBytesPerSample = CFWaveIn::GetWave()->GetBytesPerSample();

  DWORD dwBlocks = 30; // number of blocks to examine
  if ((dwSize / (dwBlockAlign)) > dwBlocks)
    dwBlocks = dwSize / (dwBlockAlign);

  // disable doble buffering
  pApp->EnableFateLoop(FALSE);  

  // did we get valid data?
  if (pData) {

    // update level display
    if (iBytesPerSample == 1) { // 8 bits audio 
  
      for(DWORD i=0;i<dwBlocks;i++) {
        dwIndex = i * dwBlockAlign;
        iVal = -(BYTE)pData[dwIndex] + 128;
        if (iVal > iValMax)
          iValMax = iVal;
      }
    
      pApp->ChangeLevel8Bit(iValMax);
      iValMax = 0;

    } else { //16 bits audio
    
      for(DWORD i=0;i<dwBlocks;i++) {
        dwIndex = i * dwBlockAlign;
        iVal = pData[dwIndex] + pData[dwIndex + 1] * 256;
        if (iVal > iValMax)
          iValMax = iVal;
      }
    
      pApp->ChangeLevel16Bit(iValMax);
      iValMax = 0;
    }
  
    // update panel information
    pRecordPanel->UpdateRecordLabels();

    //Draw Buffer
    pApp->DrawDoubleBuffer();

  }

  // re-enable back double buffering
  pApp->EnableFateLoop(TRUE);  

  // stop if space has run out
  if((pRecordPanel->GetFreeRecSpace() <= 0)||(!CFWaveIn::IsRecording())||(!pData)) {
    pRecordPanel->StopRecording();
  }

  // DEMO LIMITATION
#ifdef _XEDIT_DEMO
  if (CFWaveIn::GetRecordedTime() >= XEDIT_DEMO_TIME_LIMIT - 1100) {
    pRecordPanel->StopRecording();
  }
#endif
}


//--------------------------------------------------------------------------------
// CFRecordPanel - class members
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
CFRecordPanel::CFRecordPanel(LPCTSTR pFilePath)
{
  _tcscpy(m_szFilePath, pFilePath);
  m_bCreated= FALSE;
  m_bSaved= FALSE;
}

//--------------------------------------------------------------------------------
CFRecordPanel::~CFRecordPanel()
{
  CFWaveIn::Finalize();
  delete(m_btnRec);
  delete(m_btnPause);
  delete(m_btnEdit);
  delete(m_btnDir);
  delete(m_dirLabel);
  delete(m_totLabel);
  delete(m_remainingLabel);
  delete(m_statLabel);
  delete(m_sizeLabel);
  delete(m_freeLabel);
  delete(m_dirPanel);
  delete(m_samplingRateList);
  delete(m_bitDepthList);
  delete(m_channelsList);
}

//--------------------------------------------------------------------------------
void CFRecordPanel::SetFilePath(LPCTSTR pFilePath)
{
  _tcscpy(m_szFilePath, pFilePath);
  m_bCreated= FALSE;
  m_bSaved= FALSE;
}

//--------------------------------------------------------------------------------
BOOL CFRecordPanel::Create()
{
  // initialize CFWaveIn
  CFWaveIn::Initialize();
  // install callback that displays changes
  CFWaveIn::InstallCallBack(RecordCallbackFunc, this); 
  // get recording format
  CFWaveIn::GetWaveFormat(&m_waveFmt);
  
  // CREATE BUTTONS
  // Rec button
  CFBitmap *bmpDef= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_BIGREC)) return(FALSE);
  CFBitmap *bmpPress= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_BIGREC_P)) return(FALSE);  
  m_btnRec= new CFButton(bmpDef, bmpPress);
  m_btnRec->SetId(ID_BTN_REC_REC);
  Add(*m_btnRec);
  m_btnRec->SetVisible(TRUE);
  
  // Pause button
  bmpDef= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_BIGPAUSE)) return(FALSE);
  bmpPress= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_BIGPAUSE_P)) return(FALSE);  
  m_btnPause= new CFButton(bmpDef, bmpPress);
  m_btnPause->SetId(ID_BTN_REC_PAUSE);
  Add(*m_btnPause);
  m_btnPause->SetVisible(FALSE);
  
  // Edit button
  bmpDef= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_BIGEDIT)) return(FALSE);
  bmpPress= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_BIGEDIT_P)) return(FALSE);  
  m_btnEdit= new CFButton(bmpDef, bmpPress);
  m_btnEdit->SetId(ID_BTN_REC_EDIT);
  Add(*m_btnEdit);
  m_btnEdit->SetVisible(TRUE);

  // create "dir" button
  bmpDef= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpDef->Load(IDB_DIR)) return(FALSE);
  bmpPress= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!bmpPress->Load(IDB_DIR_P)) return(FALSE);
  m_btnDir= new CFButton(bmpDef, bmpPress);
  m_btnDir->SetId(ID_BTN_APP_DIR);
  m_btnDir->SetVisible(TRUE);
  Add(*m_btnDir);

  // create panel for recording directory selection dialog
  m_dirPanel= new CFDirPanel(m_szFilePath);
  ((CFWaveCutApp*)m_app)->Add(*m_dirPanel);
  if (!m_dirPanel->Create()) return(FALSE);
  m_dirPanel->Refresh();
  m_dirPanel->SetVisible(FALSE);

  // CREATE LABELS
  m_dirLabel= new CFLabel(m_szFilePath, 220, 17);
  m_dirLabel->SetVisible(TRUE);
  Add(*m_dirLabel);
  
  m_totLabel= new CFLabel(TEXT("total: "), 120, 17);
  m_totLabel->SetVisible(TRUE);
  Add(*m_totLabel);
  
  m_remainingLabel= new CFLabel(TEXT("remaining:"), 120, 17);
  m_remainingLabel->SetVisible(TRUE);
  Add(*m_remainingLabel);
  
  m_statLabel = new CFLabel(TEXT("status: ready"), 120, 17);
  m_statLabel->SetVisible(TRUE);
  Add(*m_statLabel);

  m_sizeLabel = new CFLabel(TEXT("size: "), 120, 17);
  m_sizeLabel->SetVisible(TRUE);
  Add(*m_sizeLabel);

  m_freeLabel = new CFLabel(TEXT("free: "), 120, 17);
  m_freeLabel->SetVisible(TRUE);
  Add(*m_freeLabel);


  CFBitmap *cbDropArrowSR= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDropArrowSR->Load(IDB_DROP_ARROW)) return(FALSE);
  CFBitmap *cbUpArrowSR= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbUpArrowSR->Load(IDB_DROP_ARROWUP)) return(FALSE);
  CFBitmap *cbDownArrowSR= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDownArrowSR->Load(IDB_DROP_ARROWDOWN)) return(FALSE);
  m_samplingRateList = new CFDropList(4, 98, cbDropArrowSR, cbUpArrowSR, cbDownArrowSR);
  m_samplingRateList->AddItem(TEXT("rate: 8000  Hz"), TEXT("8000"));
  m_samplingRateList->AddItem(TEXT("rate: 11025 Hz"), TEXT("11025"));
  m_samplingRateList->AddItem(TEXT("rate: 22050 Hz"), TEXT("22050"));
  m_samplingRateList->AddItem(TEXT("rate: 44100 Hz"), TEXT("44100"));
  m_samplingRateList->SetVisible(TRUE);
  m_samplingRateList->SetId(ID_ITEM_REC_RATE_SEL);
  switch(m_waveFmt.nSamplesPerSec) {
    case 8000:
      m_samplingRateList->SetSelectedItem(0);
      break;
    case 11025:
      m_samplingRateList->SetSelectedItem(1);
      break;
    case 22050:
      m_samplingRateList->SetSelectedItem(2);
      break;
    case 44100:
      m_samplingRateList->SetSelectedItem(3);
      break;
  }
  Add(*m_samplingRateList);

  CFBitmap *cbDropArrowBD= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDropArrowBD->Load(IDB_DROP_ARROW)) return(FALSE);
  CFBitmap *cbUpArrowBD= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbUpArrowBD->Load(IDB_DROP_ARROWUP)) return(FALSE);
  CFBitmap *cbDownArrowBD= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDownArrowBD->Load(IDB_DROP_ARROWDOWN)) return(FALSE);
  m_bitDepthList = new CFDropList(2, 98, cbDropArrowBD, cbUpArrowBD, cbDownArrowBD);
  m_bitDepthList->AddItem(TEXT("bits: 8"), TEXT("8"));
  m_bitDepthList->AddItem(TEXT("bits: 16"), TEXT("16"));
  m_bitDepthList->SetVisible(TRUE);
  m_bitDepthList->SetId(ID_ITEM_REC_BITS_SEL);
  switch(m_waveFmt.wBitsPerSample) {
    case 8:
      m_bitDepthList->SetSelectedItem(0);
      break;
    case 16:
      m_bitDepthList->SetSelectedItem(1);
      break;
  }
  Add(*m_bitDepthList);

  CFBitmap *cbDropArrowC= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDropArrowC->Load(IDB_DROP_ARROW)) return(FALSE);
  CFBitmap *cbUpArrowC= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbUpArrowC->Load(IDB_DROP_ARROWUP)) return(FALSE);
  CFBitmap *cbDownArrowC= new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!cbDownArrowC->Load(IDB_DROP_ARROWDOWN)) return(FALSE);
  m_channelsList = new CFDropList(2, 98, cbDropArrowC, cbUpArrowC, cbDownArrowC);
  m_channelsList->AddItem(TEXT("channels: 1"), TEXT("1"));
  m_channelsList->AddItem(TEXT("channels: 2"), TEXT("2"));
  m_channelsList->SetVisible(TRUE);
  m_channelsList->SetId(ID_ITEM_REC_CHAN_SEL);
  switch(m_waveFmt.nChannels) {
    case 1:
      m_channelsList->SetSelectedItem(0);
      break;
    case 2:
      m_channelsList->SetSelectedItem(1);
      break;
  }
  Add(*m_channelsList);
  
  m_bCreated= TRUE;
  UpdatePos();
  UpdateRecordLabels();

  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFRecordPanel::SetX(int iPosX)
{
  m_iPosX= iPosX;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFRecordPanel::SetY(int iPosY)
{
  m_iPosY= iPosY;
  if (m_bCreated) UpdatePos();
}

//--------------------------------------------------------------------------------
void CFRecordPanel::UpdatePos()
{
  m_btnRec->SetX(m_iPosX + 5);
  m_btnRec->SetY(m_app->GetHeight() - m_btnRec->GetHeight() - 1);
  m_btnPause->SetX(m_btnRec->GetX());
  m_btnPause->SetY(m_btnRec->GetY());
  m_btnEdit->SetX(m_btnPause->GetRight() + 5);
  m_btnEdit->SetY(m_btnRec->GetY());

  m_dirLabel->SetX(m_iPosX + 5);
  m_dirLabel->SetY(m_iPosY);

  m_dirPanel->SetX(m_iPosX + 5);
  m_dirPanel->SetY(50);

  m_btnDir->SetX(m_dirLabel->GetRight() + 5);
  m_btnDir->SetY(m_iPosY);

  m_totLabel->SetX(130);
  m_totLabel->SetY(m_iPosY + 41);
  
  m_remainingLabel->SetX(m_totLabel->GetX());
  m_remainingLabel->SetY(m_totLabel->GetBottom() + 2);
  
  m_sizeLabel->SetX(m_totLabel->GetX());
  m_sizeLabel->SetY(m_remainingLabel->GetBottom() + 2);

  m_freeLabel->SetX(m_totLabel->GetX());
  m_freeLabel->SetY(m_sizeLabel->GetBottom() + 2);

  m_samplingRateList->SetX(m_iPosX + 5);
  m_samplingRateList->SetY(m_totLabel->GetY());

  m_bitDepthList->SetX(m_iPosX + 5);
  m_bitDepthList->SetY(m_remainingLabel->GetY());
  
  m_channelsList->SetX(m_iPosX + 5);
  m_channelsList->SetY(m_sizeLabel->GetY());

  m_statLabel->SetX(m_iPosX + 5);
  m_statLabel->SetY(m_freeLabel->GetY());
}

//--------------------------------------------------------------------------------
BOOL CFRecordPanel::StartRecording()
{
  BOOL bRes;
  LPCTSTR pDirName, pTemp, pCheck;
  HANDLE hSearch;
  WIN32_FIND_DATA fd;

  if (CFWaveIn::IsRecording()) return(FALSE);
  m_bSaved= FALSE;
  pDirName= GetFileName();

  // get first directory of path
  pTemp= _tcschr(pDirName + 1, '\\');
  pCheck= pDirName;
  if (pTemp) {
    TCHAR szDir[64];
    int iPos= pTemp - pDirName;
    _tcsncpy(szDir, pDirName, iPos);
    szDir[iPos]= 0;
    pCheck= szDir;
  } 
  // check if directory is on a storage card
  hSearch= FindFirstFile(pCheck, &fd);
  if (hSearch != INVALID_HANDLE_VALUE) {
    if ((fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)&&(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
      CFWaveIn::SetBufferLength(1000);  // big buffer for slow storage card I/O
    } else {
      CFWaveIn::SetBufferLength(200);  // default audio buffer  
    }
    FindClose(hSearch);
  }
  
  bRes = CFWaveIn::Record(pDirName); // set CFWaveIn to record to file
  if (bRes) {
    m_samplingRateList->SetDropEnabled(FALSE);
    m_bitDepthList->SetDropEnabled(FALSE);
    m_channelsList->SetDropEnabled(FALSE);
    m_btnRec->SetVisible(FALSE);
    m_btnPause->SetVisible(TRUE);
    return (TRUE);
  } else 
    return(FALSE);
}

//--------------------------------------------------------------------------------
BOOL CFRecordPanel::PauseRecording()
{
  if (!CFWaveIn::IsRecording()) return(FALSE);
  
  m_btnPause->SetVisible(FALSE);
  m_btnRec->SetVisible(TRUE);
  CFWaveIn::Pause();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFRecordPanel::ResumeRecording()
{
  if (!CFWaveIn::IsRecording()) return(FALSE);
  
  m_btnRec->SetVisible(FALSE);
  m_btnPause->SetVisible(TRUE);
  CFWaveIn::Pause();

  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFRecordPanel::StopRecording()
{
  if (CFWaveIn::Stop()) {
    m_samplingRateList->SetDropEnabled(TRUE);
    m_bitDepthList->SetDropEnabled(TRUE);
    m_channelsList->SetDropEnabled(TRUE);
    m_btnPause->SetVisible(FALSE);
    m_btnRec->SetVisible(TRUE);
    m_bSaved= TRUE; // wave is always saved because we are recording to a file
    return(TRUE);
  }
  return(!CFWaveIn::IsRecording());
}

//--------------------------------------------------------------------------------
// Retrieves number offset for next free file name.
void CFRecordPanel::SetNextFileName()
{
  BOOL bFound= FALSE;
  int index= 0;
  TCHAR szFileName[MAX_PATH];
  m_bSaved= FALSE;

  // search for next available file number
  while(!bFound) {
    _stprintf(szFileName, TEXT("%s\\REC%d.WAV"), m_szFilePath, index);
    if (GetFileAttributes(szFileName) == 0xFFFFFFFF) {
      bFound= TRUE;
    } else {
      index++;
    }
  }
  _tcscpy(m_szFullFileName, szFileName);
}

//--------------------------------------------------------------------------------
bool CFRecordPanel::DropListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry)
{
  LPTSTR szStop;
  switch(dwListID) {
    case ID_ITEM_REC_RATE_SEL:
      m_waveFmt.nSamplesPerSec = (WORD)_tcstol(pEntry->pszAddInfo, &szStop, 10); 
      m_waveFmt.nAvgBytesPerSec= m_waveFmt.nSamplesPerSec * m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      m_waveFmt.nBlockAlign    = m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      CFWaveIn::SetWaveFormat(&m_waveFmt);
      UpdateRecordLabels();
      break;
    
    case ID_ITEM_REC_BITS_SEL:
      m_waveFmt.wBitsPerSample = (WORD)_tcstol(pEntry->pszAddInfo, &szStop, 10); 
      m_waveFmt.nAvgBytesPerSec= m_waveFmt.nSamplesPerSec * m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      m_waveFmt.nBlockAlign    = m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      CFWaveIn::SetWaveFormat(&m_waveFmt);
      UpdateRecordLabels();
      break;
    
    case ID_ITEM_REC_CHAN_SEL:
      m_waveFmt.nChannels = (WORD)_tcstol(pEntry->pszAddInfo, &szStop, 10); 
      m_waveFmt.nAvgBytesPerSec= m_waveFmt.nSamplesPerSec * m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      m_waveFmt.nBlockAlign    = m_waveFmt.nChannels * (m_waveFmt.wBitsPerSample >> 3);
      CFWaveIn::SetWaveFormat(&m_waveFmt);
      UpdateRecordLabels();
      break;

    default:
      return(FALSE); // event not handled
      break;
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
bool CFRecordPanel::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {

    case ID_BTN_REC_REC: 
      // start recording or resume record?
      if (CFWaveIn::IsPaused()) {
        ResumeRecording();
      
      } else {
        if (GetFreeRecSpace() > 0) {
          SetNextFileName();
          UpdateRecordLabels();
          StartRecording();
        }
      }
      return(TRUE);

    case ID_BTN_REC_PAUSE:
      PauseRecording();
      ((CFWaveCutApp*)m_app)->ResetLevel();
      UpdateRecordLabels();
      return(TRUE);

    case ID_BTN_APP_DIR:
      if (CFWaveIn::IsRecording()) break;
      SetLabelsVisible(FALSE);
      ((CFWaveCutApp*)m_app)->HideVolumeLevel(TRUE);
      m_dirPanel->SetVisible(TRUE);  
      return(TRUE);

    case ID_BTN_DIR_OK:
      m_dirPanel->SetVisible(FALSE);  
      ((CFWaveCutApp*)m_app)->HideVolumeLevel(FALSE);
      SetLabelsVisible(TRUE);
      DirSelected(TRUE);
      SetNextFileName();
      UpdateRecordLabels();
      return(TRUE);

    case ID_BTN_DIR_CANCEL:
      m_dirPanel->SetVisible(FALSE);  
      ((CFWaveCutApp*)m_app)->HideVolumeLevel(FALSE);
      SetLabelsVisible(TRUE);
      DirSelected(FALSE);
      return(TRUE);

    default:
      return(FALSE); // event not handled
      break;
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFRecordPanel::UpdateRecordLabels()
{
  TCHAR szBuff[128];

  m_dirLabel->SetText(m_szFullFileName);

  if(CFWaveIn::IsRecording()) {
    _stprintf(szBuff, TEXT("status: recording"));
    m_statLabel->SetText(szBuff);
  } else {
    _stprintf(szBuff, TEXT("status: ready"));
    m_statLabel->SetText(szBuff);
    m_samplingRateList->SetDropEnabled(TRUE);
    m_bitDepthList->SetDropEnabled(TRUE);
    m_channelsList->SetDropEnabled(TRUE);
    m_btnPause->SetVisible(FALSE);
    m_btnRec->SetVisible(TRUE);
  }

  DWORD dwFreeSpace;
  dwFreeSpace= GetFreeRecSpace();

  _stprintf(szBuff, TEXT("total: %d.%03d s"), CFWaveIn::GetRecordedTime()/1000,CFWaveIn::GetRecordedTime()%1000);
  m_totLabel->SetText(szBuff);

  _stprintf(szBuff, TEXT("remaining: %d s"), dwFreeSpace/(m_waveFmt.nAvgBytesPerSec));
  m_remainingLabel->SetText(szBuff);  

  _stprintf(szBuff, TEXT("size: %d k"), CFWaveIn::GetRecordedSize()>>10);
  m_sizeLabel->SetText(szBuff);  

  _stprintf(szBuff, TEXT("free mem: %d k"), (dwFreeSpace)>>10);
  m_freeLabel->SetText(szBuff);  
}

//--------------------------------------------------------------------------------
DWORD CFRecordPanel::GetFreeRecSpace()
{
  ULARGE_INTEGER ulFreeBytes;
  DWORD dwFreeBytes;
  GetDiskFreeSpaceEx((LPCTSTR)m_szFilePath, &ulFreeBytes, NULL, NULL);
  dwFreeBytes = (DWORD)ulFreeBytes.LowPart; 
  if (dwFreeBytes > (DWORD)MIN_SPACE_FREE) {  // leave some space free
    dwFreeBytes -= (DWORD)MIN_SPACE_FREE;
  } else { 
    dwFreeBytes = 0;
  }
  return dwFreeBytes;
}

//--------------------------------------------------------------------------------
void CFRecordPanel::DirSelected(bool bSelected)
{
  // load file?
  if (bSelected) {
    LPCTSTR pDirName= m_dirPanel->GetDirectory();    
    SetFilePath(pDirName);
    UpdateRecordLabels();
  }
}

//--------------------------------------------------------------------------------
void CFRecordPanel::SetLabelsVisible(bool bVisible)
{
  m_btnRec->SetVisible(bVisible);
  m_btnEdit->SetVisible(bVisible);
  m_btnDir->SetVisible(bVisible);
  m_dirLabel->SetVisible(bVisible);
  m_totLabel->SetVisible(bVisible);
  m_statLabel->SetVisible(bVisible);
  m_remainingLabel->SetVisible(bVisible);
  m_sizeLabel->SetVisible(bVisible);
  m_freeLabel->SetVisible(bVisible);
  m_samplingRateList->SetVisible(bVisible);
  m_bitDepthList->SetVisible(bVisible);
  m_channelsList->SetVisible(bVisible);
}
