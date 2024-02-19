#include "IdApp.h"
#include "resource.h"
#include <winioctl.h>
#include <stdio.h>
#include <TCHAR.h>


//--------------------------------------------------------------------------------
extern "C" bool KernelIoControl(
	DWORD dwIoControlCode, 
	LPVOID lpInBuf, 
	DWORD nInBufSize, 
	LPVOID lpOutBuf, 
	DWORD nOutBufSize, 
	LPDWORD lpBytesReturned);

typedef struct _DEVICE_ID 
{
	DWORD	dwSize;
	DWORD	dwPreSetIdOffset;
	DWORD	dwPreSetIdBytes;
	DWORD	dwPlatformIDOffset;
	DWORD	dwPlatformIDBytes;
} DEVICE_ID, *PDEVICE_ID;

typedef struct __PROCESSOR_INFO 
{
  WORD wVersion; 
  WCHAR szProcessorCore[40];
  WORD wCoreRevision;
  WCHAR szProcessorName[40];
  WORD wProcessorRevision;
  WCHAR szCatalogNumber[100];
  WCHAR szVendor[100];
  DWORD dwInstructionSet;
  DWORD dwClockSpeed;
} PROCESSOR_INFO, *PPROCESSOR_INFO;

#define IOCTL_HAL_REBOOT             CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_DEVICEID       CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROCESSOR_INFORMATION  CTL_CODE(FILE_DEVICE_HAL, 25, METHOD_BUFFERED, FILE_ANY_ACCESS)




//--------------------------------------------------------------------------------
CFateApp* CreateFateApp()
{
  return(new CIdApp());
}


//--------------------------------------------------------------------------------
CIdApp::CIdApp() : CFateApp(DM_LANDSCAPE_FLIPPED)
{
  m_pBtnQuit= NULL;
  m_pBtnGenerate= NULL;
  m_pBtnSave= NULL;
  m_pBmpBack= NULL;
  m_pLabelID= NULL;
  m_pLabelStatus= NULL;

  m_iLenID= 0;
  m_szID[0]= 0;
}

//--------------------------------------------------------------------------------
CIdApp::~CIdApp()
{
  SAFE_DELETE(m_pBtnQuit);
  SAFE_DELETE(m_pBtnGenerate);
  SAFE_DELETE(m_pBtnSave);
  SAFE_DELETE(m_pBmpBack);
  SAFE_DELETE(m_pLabelID);
  SAFE_DELETE(m_pLabelStatus);
}

//--------------------------------------------------------------------------------
bool CIdApp::InitFateApp()
{
  // load background bitmap
  m_pBmpBack = new CFBitmap(m_pSystem->GetDoubleBuffer());
  if (!m_pBmpBack->Load(IDB_BACK)) {
    return (false);
  }

  // QUIT button
  m_pBtnQuit= new CFButton(TEXT("X"), 19, 19);
  m_pBtnQuit->SetId(ID_BTN_QUIT);
  m_pBtnQuit->SetX(GetWidth() - m_pBtnQuit->GetWidth());
  m_pBtnQuit->SetY(0);
  Add(*m_pBtnQuit);

  // "Generate ID" button
  m_pBtnGenerate= new CFButton(TEXT("Generate ID"), 90, 25);
  m_pBtnGenerate->SetId(ID_BTN_GENERATE);
  m_pBtnGenerate->SetX(5);
  m_pBtnGenerate->SetY(m_pBtnQuit->GetBottom() + 15);
  Add(*m_pBtnGenerate);

  // ID label  
  m_pLabelID= new CFLabel(310, 30);
  m_pLabelID->SetX(m_pBtnGenerate->GetX());
  m_pLabelID->SetY(m_pBtnGenerate->GetBottom() + 10);
  Add(*m_pLabelID);

  // "Save" button  
  m_pBtnSave= new CFButton(TEXT("Save ID"), 90, 25);
  m_pBtnSave->SetEnabled(false);
  m_pBtnSave->SetId(ID_BTN_SAVE);
  m_pBtnSave->SetX(m_pBtnGenerate->GetX());
  m_pBtnSave->SetY(m_pLabelID->GetBottom() + 30);
  Add(*m_pBtnSave);

  // status label  
  m_pLabelStatus= new CFLabel(310, 30);
  m_pLabelStatus->SetColBack(RGB(192, 192, 192));
  m_pLabelStatus->SetColBorder(RGB(150, 150, 150));
  m_pLabelStatus->SetX(m_pBtnSave->GetX());
  m_pLabelStatus->SetY(m_pBtnSave->GetBottom() + 10);
  Add(*m_pLabelStatus);

  return true;
}

//--------------------------------------------------------------------------------
void CIdApp::Draw()
{
  m_pBmpBack->Blit();
}

//--------------------------------------------------------------------------------
bool CIdApp::ButtonReleased(DWORD dwBtnID)
{
  switch(dwBtnID) {
    case ID_BTN_QUIT:
      Exit();
      return(true);

    case ID_BTN_GENERATE:
      GetID();
      return(true);

    case ID_BTN_SAVE:
      SaveID();
      return(true);

    default:
      return(false);
  }
}

//--------------------------------------------------------------------------------
void CIdApp::GetID()
{
#ifdef _WIN32_WCE

  DWORD dwOut= 0;
  unsigned int i;
  char szBuff[256]= {0};
  PDEVICE_ID pdi= (PDEVICE_ID)szBuff;
  bool bRet;
  TCHAR szAux[32]= {0};

  pdi->dwSize= 256;  
  bRet= KernelIoControl(IOCTL_HAL_GET_DEVICEID, NULL, 0, szBuff, 256, &dwOut);

  if (pdi->dwPreSetIdOffset) {
    for (i=0; i < pdi->dwPreSetIdBytes; i++) {
      _stprintf(szAux, TEXT("%02X"), *((PBYTE)pdi + pdi->dwPreSetIdOffset + i));
      _tcscat(m_szID, szAux);
    }
  }

  if (pdi->dwPlatformIDOffset) {
    for (i=0; i < pdi->dwPlatformIDBytes; i++) {
      _stprintf(szAux, TEXT("%02X"), *((PBYTE)pdi + pdi->dwPlatformIDOffset + i));
      _tcscat(m_szID, szAux);
    }
  }    
  m_iLenID= (pdi->dwPreSetIdOffset + pdi->dwPlatformIDOffset) * 2;

#else 

  sprintf(m_szID, "BABE00001234");
  m_iLenID= strlen(m_szID);

#endif  // _WIN32_WCE
  
  m_pBtnSave->SetEnabled(true);
  m_pLabelID->SetText(m_szID);
}

//--------------------------------------------------------------------------------
void CIdApp::SaveID()
{
  HANDLE hFile;
  DWORD dwWritten= 0;
  TCHAR szPath[256];
  char c;

  _tcscpy(szPath, GetAppPath());
  _tcscat(szPath, TEXT("ID.txt"));

  hFile= CreateFile(szPath, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    for (int i=0; i<m_iLenID; i++) {
      c= (char)m_szID[i];
      WriteFile(hFile, &c, 1, &dwWritten, NULL);
    }
    CloseHandle(hFile);
    m_pLabelStatus->SetColBorder(RGB(0, 0, 0));
    m_pLabelStatus->SetText(TEXT(" ID saved to file \"ID.txt\""));
  }
}
