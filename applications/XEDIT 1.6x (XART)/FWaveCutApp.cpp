// FWaveCutter.cpp: implementation of the CFWaveCutter class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FWaveCutApp.h"
#include "Resource.h"


//--------------------------------------------------------------------------------
// Global function for creating the one and only application object.
CFateApp* CreateFateApp()
{
  return(new CFWaveCutApp());
}

//--------------------------------------------------------------------------------
CFWaveCutApp::CFWaveCutApp() : CFateApp(DM_LANDSCAPE_FLIPPED)
{
  // inits
  m_bMoveSlide= FALSE;
  m_iLevel= 4;   
  m_dwVolumes[0]= 0xFFFFFFFF;
  m_dwVolumes[1]= 0xDDDDDDDD;
  m_dwVolumes[2]= 0xBBBBBBBB;
  m_dwVolumes[3]= 0x99999999;
  m_dwVolumes[4]= 0x77777777;
  m_dwVolumes[5]= 0x55555555;
  m_dwVolumes[6]= 0x33333333;
  m_dwVolumes[7]= 0x11111111;
  m_dwVolumes[8]= 0x00000000;
}

//--------------------------------------------------------------------------------
CFWaveCutApp::~CFWaveCutApp()
{
  EnableSuspend(TRUE);
  delete(m_bmpTitle);
  delete(m_bmpVol);
  delete(m_bmpSlide);
  delete(m_bmpLevel);
  delete(m_bmpMask);
  delete(m_bmpCheat);
  delete(m_fileLabel);
  delete(m_wavePanel);
  delete(m_loadPanel);
  delete(m_savePanel);
  delete(m_buttonPanel);
  delete(m_recordPanel);
  delete(m_btnLoad);
  delete(m_btnSave);
  delete(m_btnUndo);
  delete(m_btnRedo);
  delete(m_btnCutOut);
  delete(m_btnCutIn);
  delete(m_btnZoomIn);
  delete(m_btnZoomOut);
  delete(m_btnInfo);
  delete(m_btnQuit);
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::InitFateApp()
{ 
  INIT_REMOTE_CONSOLE(TEXT("COM"), 1);
  REMOTE_CONSOLE(TEXT("xEdit says hello"));

#ifndef _XEDIT_DEMO
#ifdef _WIN32_WCE
  // check for serial number
  if (!SerialOK()) return(FALSE);
#endif
#endif

  // disable suspend so the application won't freeze
  EnableSuspend(FALSE);

  m_bmpTitle= new CFBitmap(m_hdc);
#ifdef _XEDIT_DEMO  
  if (!m_bmpTitle->Load(IDB_TITLE_TRIAL)) return(FALSE);
#else 
  if (!m_bmpTitle->Load(IDB_TITLE)) return(FALSE);
#endif
  
  // create and add label for filename
  m_fileLabel= new CFLabel(TEXT(" "), 245, 17);
  Add(m_fileLabel);
  m_fileLabel->SetVisible(TRUE);

  // create and add panel which displays the waves
  m_wavePanel= new CFWavePanel();
  
  // create and add panel for buttons
  m_buttonPanel= new CFPanel();
  Add(m_buttonPanel);
  m_buttonPanel->SetVisible(TRUE);
  
  // create "load" button
  CFBitmap *bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_LOAD)) return(FALSE);
  CFBitmap *bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_LOAD_P)) return(FALSE);
  m_btnLoad= new CFButton(bmpDef, bmpPress);
  m_btnLoad->SetID(ID_BTN_APP_OPEN);
  m_btnLoad->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnLoad);
  
  // create "save" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_SAVE)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_SAVE_P)) return(FALSE);
  m_btnSave= new CFButton(bmpDef, bmpPress);;
  m_btnSave->SetID(ID_BTN_APP_SAVE);
  m_btnSave->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnSave);
  
  // create "undo" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_UNDO)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_UNDO_P)) return(FALSE);
  m_btnUndo= new CFButton(bmpDef, bmpPress);
  m_btnUndo->SetID(ID_BTN_APP_UNDO);
  m_btnUndo->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnUndo);
  
  // create "redo "button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_REDO)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_REDO_P)) return(FALSE);
  m_btnRedo= new CFButton(bmpDef, bmpPress);
  m_btnRedo->SetID(ID_BTN_APP_REDO);
  m_btnRedo->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnRedo);
  
  // create "cutout" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_CUTOUT)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_CUTOUT_P)) return(FALSE);
  m_btnCutOut= new CFButton(bmpDef, bmpPress);
  m_btnCutOut->SetID(ID_BTN_APP_CUTOUT);
  m_btnCutOut->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnCutOut);
  
  // create "cutin" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_CUTIN)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_CUTIN_P)) return(FALSE);
  m_btnCutIn= new CFButton(bmpDef, bmpPress);
  m_btnCutIn->SetID(ID_BTN_APP_CUTIN);
  m_btnCutIn->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnCutIn);
  
  // create "zoomin" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_ZOOMIN)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_ZOOMIN_P)) return(FALSE);
  m_btnZoomIn= new CFButton(bmpDef, bmpPress);
  m_btnZoomIn->SetID(ID_BTN_APP_ZOOMIN);
  m_btnZoomIn->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnZoomIn);
  
  // create "zoomout" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_ZOOMOUT)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_ZOOMOUT_P)) return(FALSE);
  m_btnZoomOut= new CFButton(bmpDef, bmpPress);
  m_btnZoomOut->SetID(ID_BTN_APP_ZOOMOUT);
  m_btnZoomOut->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnZoomOut);
  
  // create "info" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_INFO)) return(FALSE);
  bmpPress= new CFBitmap(m_hdc);
  if (!bmpPress->Load(IDB_INFO_P)) return(FALSE);
  m_btnInfo= new CFButton(bmpDef, bmpPress);
  m_btnInfo->SetID(ID_BTN_APP_INFO);
  m_btnInfo->SetVisible(TRUE);
  m_buttonPanel->Add(m_btnInfo);

  // create "quit" button
  bmpDef= new CFBitmap(m_hdc);
  if (!bmpDef->Load(IDB_QUIT)) return(FALSE);
  m_btnQuit= new CFButton(bmpDef);
  m_btnQuit->SetID(ID_BTN_APP_QUIT);
  m_btnQuit->SetVisible(TRUE);
  Add(m_btnQuit);  
  
  // add the wave-edit panel
  Add(m_wavePanel);
  if (!m_wavePanel->Create()) return(FALSE);
  m_wavePanel->SetVisible(TRUE);

  // create recording panel
  TCHAR szFilePath[256];

  _tcscpy(szFilePath, m_app->GetAppPath());
  _tcscat(szFilePath, TEXT("recorded"));
  //check if the recorded directory exists, if not, create
  if(GetFileAttributes((LPCTSTR)&szFilePath) == 0xFFFFFFFF) { // if file doesnt exists
    CreateDirectory((LPCTSTR)&szFilePath, NULL); 
  }
  m_recordPanel= new CFRecordPanel(szFilePath);
  Add(m_recordPanel);
  if (!m_recordPanel->Create()) return(FALSE);
  m_recordPanel->SetVisible(FALSE);
  
  // create panel for wave loading dialog
  m_loadPanel= new CFLoadPanel(szFilePath);
  Add(m_loadPanel);
  if (!m_loadPanel->Create()) return(FALSE);
  m_loadPanel->Refresh();
  m_loadPanel->SetVisible(FALSE);

  //check if the edited directory exists, if not, create
  _tcscpy(szFilePath, m_app->GetAppPath());
  _tcscat(szFilePath, TEXT("edited"));
  //check if the edited directory exists, if not, create
  if(GetFileAttributes((LPCTSTR)&szFilePath) == 0xFFFFFFFF) { // if file doesnt exists
    CreateDirectory((LPCTSTR)&szFilePath, NULL); 
  }

  m_savePanel= new CFSavePanel(szFilePath);
  Add(m_savePanel);
  if (!m_savePanel->Create()) return(FALSE);
  m_savePanel->Refresh();
  m_savePanel->SetVisible(FALSE);
  
  // create level bar
  m_bmpLevel= new CFBitmap(m_hdc);
  if (!m_bmpLevel->Load(IDB_LEVEL)) return(FALSE);

  // create mask
  m_bmpMask= new CFBitmap(m_hdc);
  if (!m_bmpMask->Load(IDB_MASK)) return(FALSE);

  // create bitmap for correcting title whem volume changes
  m_bmpCheat= new CFBitmap(m_hdc);
  if (!m_bmpCheat->Load(IDB_CHEAT)) return(FALSE);

  // create volume bar
  m_bmpVol= new CFBitmap(m_hdc);
  if (!m_bmpVol->Load(IDB_VOL)) return(FALSE);

  // create volume slide
  m_bmpSlide= new CFBitmap(m_hdc);
  if (!m_bmpSlide->Load(IDB_SLIDE)) return(FALSE);

  UpdatePos();
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::ActivateFateApp()
{ 
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::CloseFateApp()
{
  return(TRUE);
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::Draw()
{
  ClearScreen(RGB(51, 102, 153));
  m_bmpTitle->Blit();
  if (m_bDisplayLevel) {
    m_bmpVol->Blit();
    m_bmpSlide->SetY(m_iSlideStart + (m_iLevel * m_iVolStep) - m_iSlideCenter);
    m_bmpSlide->SaveUnder();
    m_bmpSlide->TransBlit(RGB(255, 0, 0));
    m_bmpLevel->Blit();
    m_bmpMask->SaveUnder();
    m_bmpMask->Blit();
    m_bmpCheat->Blit();
  }
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::StylusDown(int xPos, int yPos)
{
  // TODO: remove later!
  //if (m_bmpTitle->PointInside(xPos, yPos)) SaveDoubleBuffer(TEXT("\\screenshot.bmp"));

  // volume slider tapped?
  if (m_bmpSlide->PointInside(xPos, yPos) && !m_bMoveSlide) {
    m_bMoveSlide= TRUE;
    return(TRUE);
  }
  //volume bar tapped?
  if (m_bmpVol->PointInside(xPos, yPos)) {  
      m_bmpSlide->RestoreUnder();
      m_iLevel= (yPos -  m_iSlideStart) / m_iVolStep;
      if(m_iLevel < 0) m_iLevel = 0;
      if(m_iLevel > 8) m_iLevel = 8;
      m_bmpSlide->SetY(m_iSlideStart + (m_iLevel * m_iVolStep) - m_iSlideCenter);
      m_bmpSlide->SaveUnder();
      m_bmpSlide->TransBlit(RGB(255, 0, 0));
      m_wavePanel->SetVolume(m_dwVolumes[m_iLevel]);
      return(TRUE);
    }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::StylusMove(int xPos, int yPos)
{
  // volume change?
  if ((m_bMoveSlide)&&(m_bmpVol->PointInside(xPos, yPos) || m_bmpSlide->PointInside(xPos, yPos))) {
    m_bmpSlide->RestoreUnder();
    m_iLevel= (yPos -  m_iSlideStart) / m_iVolStep;
    if(m_iLevel < 0) m_iLevel = 0;
    if(m_iLevel > 8) m_iLevel = 8;
    m_bmpSlide->SetY(m_iSlideStart + (m_iLevel * m_iVolStep) - m_iSlideCenter);
    m_bmpSlide->SaveUnder();
    m_bmpSlide->TransBlit(RGB(255, 0, 0));
    m_wavePanel->SetVolume(m_dwVolumes[m_iLevel]);
    return(TRUE);
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::StylusUp(int xPos, int yPos)
{
  m_bMoveSlide= FALSE;
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::KeyDown(int vkKey)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::KeyUp(int vkKey)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
// Method is called by file-open-panel. 
// Parameter "bLoaded" specifies wether OK or Cancel was pressed.
void CFWaveCutApp::FileLoaded(BOOL bLoaded)
{
  // load file?
  if (bLoaded) {
    LPCTSTR pFileName= m_loadPanel->GetFileToLoad();
    if (pFileName) {
      m_wavePanel->LoadWave(pFileName);
    }
  }
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::SetFileName(LPCTSTR pFileName)
{
  if (pFileName) {
    m_fileLabel->SetText(pFileName);
  }
  m_fileLabel->Draw();
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::ChangeLevel8Bit(int iVal)
{
  m_bmpMask->RestoreUnder();
  m_bmpMask->SetY(m_iPosMask - ((iVal * m_iLevelHeight) >> 7));
  m_bmpMask->SaveUnder();
  m_bmpMask->Blit();
  m_bmpCheat->Blit();
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::ChangeLevel16Bit(int iVal)
{
  m_bmpMask->RestoreUnder();
  m_bmpMask->SetY(m_iPosMask - ((iVal * m_iLevelHeight)>> 15));
  m_bmpMask->SaveUnder();
  m_bmpMask->Blit();
  m_bmpCheat->Blit();
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::ResetLevel()
{
  m_bmpMask->RestoreUnder();
  m_bmpMask->SetY(m_iPosMask);
  m_bmpMask->SaveUnder();
  m_bmpMask->Blit();
  m_bmpCheat->Blit();
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::SerialOK()
{
  HMODULE hMod;
  PROCESS_INFORMATION pi;
  HANDLE hFile;
  TCHAR szSN[64];
  char szCode[64];
  char *pDLLSN;
  DWORD dwBytesRead;   
  GetFunc Get= NULL;
  
  memset(szSN  , 0, 64 * sizeof(TCHAR)); 
  memset(szCode, 0, 64 * sizeof(char)); 

  // start CreateAssetFile.exe  
  if (::CreateProcess(TEXT("\\windows\\CreateAssetFile.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi)) { 
    // wait until CreateAssetFile.exe will be finished
    ::WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
 
    // read data from cpqAssetData.dat file
    hFile= CreateFile(TEXT("\\windows\\cpqAssetData.dat"), GENERIC_READ, 
                      FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0); 
 
    if (hFile != INVALID_HANDLE_VALUE) { 
      SetFilePointer(hFile, 976, NULL, FILE_BEGIN); 
      ReadFile(hFile, szSN, 64, &dwBytesRead, NULL); 
      CloseHandle(hFile);
    }
  
  } else {    
    // NO assetfile created .. try another way!
    typedef BOOL (*_iPAQGetSerialNumber)(TCHAR *lpszValue);
    _iPAQGetSerialNumber iPAQGetSerialNumber= NULL;
    HMODULE hModIpaq= LoadLibrary(TEXT("ipaqutil.dll"));
    if (!hModIpaq) {
      MessageBox(m_hWnd, TEXT("Device is not an iPaq!"), TEXT(""), MB_OK);
      return(FALSE);
    } 

    iPAQGetSerialNumber= (_iPAQGetSerialNumber)GetProcAddress(hModIpaq, TEXT("iPAQGetSerialNumber"));
    if (!iPAQGetSerialNumber(szSN)) {
      MessageBox(m_hWnd, TEXT("Could not verify serial number!"), TEXT(""), MB_OK);
      FreeLibrary(hModIpaq);
      return(FALSE);
    }  
  }

  // do encoding
  for (UINT i=0; i< _tcslen(szSN); i++) {
    int iTemp= i % 4;
    switch(iTemp) {
      case 0:
        szCode[i]= (char)szSN[i] ^ 0xDC;
        break;

      case 1:
        szCode[i]= (char)szSN[i] ^ 0xAC;
        break;

      case 2:
        szCode[i]= (char)szSN[i] ^ 0xBE;
        break;

      case 3:
        szCode[i]= (char)szSN[i] ^ 0xBA;
        break;
    }
  
  }
  // get serial number from library
  hMod= LoadLibrary(TEXT("xedit.dll"));
  if (!hMod) {
    MessageBox(m_hWnd, TEXT("Library xedit.dll not found!"), TEXT(""), MB_OK);    
    return(FALSE);
  } 

  Get= (GetFunc)GetProcAddress(hMod, TEXT("?Get@@YAHPAPAD@Z"));
  if (!Get) {
    MessageBox(m_hWnd, TEXT("Library xedit.dll is not valid!"), TEXT(""), MB_OK);    
    FreeLibrary(hMod);
    return(FALSE);
  }
  pDLLSN= (char*)malloc(64 * sizeof(char));
  Get(&pDLLSN);
  FreeLibrary(hMod);

  for (i=0; i<strlen(szCode); i++) {
    if (szCode[i] != pDLLSN[i]) {
      MessageBox(m_hWnd, TEXT("Illegal copy of xEdit!"), TEXT(""), MB_OK);
      free(pDLLSN);
      return(FALSE);
    }
  }

  free(pDLLSN);
  return(TRUE);
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {    
    case ID_BTN_APP_OPEN:
      if (!CFWaveOut::IsPlaying()) {
        m_buttonPanel->SetVisible(FALSE);
        m_wavePanel->SetVisible(FALSE);
        m_fileLabel->SetVisible(FALSE);
        HideVolumeLevel(TRUE);
        m_loadPanel->SetVisible(TRUE);  
      }
      return(TRUE);
    
    case ID_BTN_APP_SAVE:
      if ((!CFWaveOut::IsPlaying())&&(m_wavePanel->GetWave()->IsValid())) {
        m_buttonPanel->SetVisible(FALSE);
        m_wavePanel->SetVisible(FALSE);
        m_fileLabel->SetVisible(FALSE);
        HideVolumeLevel(TRUE);
        m_savePanel->SetFileToSave(m_wavePanel->GetWave()->GetFileName());
        m_savePanel->SetVisible(TRUE);
      }
      return(TRUE);

    case ID_BTN_APP_QUIT:
      m_wavePanel->Stop();
      Exit();
      return(TRUE);
      
    // SWITCH TO RECORDING MODE
    case ID_BTN_WAVE_REC: 
      if (!CFWaveOut::IsPlaying()) {
        m_buttonPanel->SetVisible(FALSE);
        m_wavePanel->SetVisible(FALSE);
        CFWaveIn::Reset();
        ShowFileLabel(FALSE);
        m_recordPanel->SetNextFileName();
        m_recordPanel->UpdateRecordLabels();
        m_recordPanel->SetVisible(TRUE);
      }
      return(TRUE);

    case ID_BTN_LOAD_OK:
      FileLoaded(TRUE);
      m_loadPanel->SetVisible(FALSE);
      m_wavePanel->SetVisible(TRUE);
      m_fileLabel->SetVisible(TRUE);
      m_buttonPanel->SetVisible(TRUE);
      HideVolumeLevel(FALSE);
      break;

    case ID_BTN_LOAD_CANCEL:
      FileLoaded(FALSE);
      m_loadPanel->SetVisible(FALSE);
      m_wavePanel->SetVisible(TRUE);
      m_fileLabel->SetVisible(TRUE);
      m_buttonPanel->SetVisible(TRUE);
      HideVolumeLevel(FALSE);
      break;
  
    case ID_BTN_SAVEDIR_OK:
      m_wavePanel->SaveWave(m_savePanel->GetFileToSave());
      m_savePanel->SetVisible(FALSE);
      m_wavePanel->SetVisible(TRUE);
      m_fileLabel->SetVisible(TRUE);
      m_buttonPanel->SetVisible(TRUE);
      HideVolumeLevel(FALSE);
      break;

    case ID_BTN_SAVEDIR_CANCEL:
      m_savePanel->SetVisible(FALSE);
      m_wavePanel->SetVisible(TRUE);
      m_fileLabel->SetVisible(TRUE);
      m_buttonPanel->SetVisible(TRUE);
      HideVolumeLevel(FALSE);
      break;

    // SWITCH TO EDIT MODE
    case ID_BTN_REC_EDIT:
      if(m_recordPanel->StopRecording()) {
        ResetLevel();
        m_recordPanel->SetVisible(FALSE);
        m_buttonPanel->SetVisible(TRUE);
        ShowFileLabel(TRUE);
        m_wavePanel->SetVisible(TRUE);
        // check if there was a wave recorded => IsSaved() returns TRUE in this case
        if (m_recordPanel->IsSaved()) {
          m_wavePanel->CloseOldWave();
          m_wavePanel->LoadWave(m_recordPanel->GetFileName());
          m_loadPanel->SetFilePath(m_recordPanel->GetFilePath());
        }
      }
      break;
      
    default:
      return(FALSE);
      break;
  }
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
BOOL CFWaveCutApp::ButtonPressed(DWORD dwBtnID)
{
  return(FALSE);  // event not handled
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::UpdatePos()
{
  int iBtnsY= 0;
  
  m_bmpTitle->SetX(0);
  m_bmpTitle->SetY(0);

  m_fileLabel->SetX(5);
  m_fileLabel->SetY(m_bmpTitle->GetBottom() + 2);

  iBtnsY= m_fileLabel->GetBottom() + 2;

  m_btnLoad->SetX(5);
  m_btnLoad->SetY(iBtnsY);

  m_btnSave->SetX(m_btnLoad->GetRight() + 5);
  m_btnSave->SetY(iBtnsY);
  
  m_btnUndo->SetX(m_btnSave->GetRight() + 5);
  m_btnUndo->SetY(iBtnsY);
  
  m_btnRedo->SetX(m_btnUndo->GetRight() + 5);
  m_btnRedo->SetY(iBtnsY);
  
  m_btnCutOut->SetX(m_btnRedo->GetRight() + 5);
  m_btnCutOut->SetY(iBtnsY);
  
  m_btnCutIn->SetX(m_btnCutOut->GetRight() + 5);
  m_btnCutIn->SetY(iBtnsY);
  
  m_btnZoomIn->SetX(m_btnCutIn->GetRight() + 5);
  m_btnZoomIn->SetY(iBtnsY);
  
  m_btnZoomOut->SetX(m_btnZoomIn->GetRight() + 5);
  m_btnZoomOut->SetY(iBtnsY);
  
  m_btnInfo->SetX(m_btnZoomOut->GetRight() + 5);
  m_btnInfo->SetY(iBtnsY);
  
  m_btnQuit->SetX(m_app->GetWidth() - m_btnQuit->GetWidth()-2);
  m_btnQuit->SetY(2);
  
  m_wavePanel->SetX(0);
  m_wavePanel->SetY(m_btnInfo->GetBottom() + 2);
  
  m_recordPanel->SetX(0);
  m_recordPanel->SetY(m_bmpTitle->GetBottom() + 2);
  
  m_loadPanel->SetX(0);
  m_loadPanel->SetY(50);

  m_savePanel->SetX(0);
  m_savePanel->SetY(50);

  m_bmpLevel->SetX(270);
  m_bmpLevel->SetY(m_bmpTitle->GetBottom() + 20);
  m_iLevelHeight= m_bmpLevel->GetHeight();
  m_iPosMask= (m_bmpLevel->GetY()); 

  m_bmpCheat->SetX(m_bmpLevel->GetX());
  m_bmpCheat->SetY(m_bmpTitle->GetY());
  
  m_bmpMask->SetX(m_bmpLevel->GetX() + 3);
  m_bmpMask->SetY(m_iPosMask);
  
  m_bmpVol->SetX(m_bmpLevel->GetRight() + 15);
  m_bmpVol->SetY(m_bmpLevel->GetY());

  m_iSlideStart = m_bmpVol->GetY() + 5;
  m_iSlideEnd = m_bmpVol->GetBottom() - 5;
  m_iSlideCenter = m_bmpSlide->GetHeight() / 2;
  m_iVolStep= (m_iSlideEnd - m_iSlideStart) / 8;
  m_bmpSlide->SetX(m_bmpVol->GetX());

  m_wavePanel->SetVolume(m_dwVolumes[m_iLevel]);
  
  m_bDisplayLevel = TRUE;
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::SetFileLabelText(LPCTSTR pText)
{
  m_fileLabel->SetText(pText);
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::HideVolumeLevel(BOOL bHide)
{
  m_bDisplayLevel = !bHide;
}

//--------------------------------------------------------------------------------
void CFWaveCutApp::ShowFileLabel(BOOL bShow)
{
  m_fileLabel->SetVisible(bShow);
}

