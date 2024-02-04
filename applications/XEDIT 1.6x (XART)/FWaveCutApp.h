// FWaveCutter.h: interface for the CFWaveCutter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FWAVECUTTER_H__1F709245_43D2_4FD9_849E_BF86B88BFE6B__INCLUDED_)
#define AFX_FWAVECUTTER_H__1F709245_43D2_4FD9_849E_BF86B88BFE6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../framework/include/fate.h"
#include "FWavePanel.h"
#include "FLoadPanel.h"
#include "FSavePanel.h"
#include "FDirPanel.h"
#include "FRecordPanel.h"


//--------------------------------------------------------------------------------
/// needed dll-function pointer
typedef BOOL (*GetFunc)(char **ppSN);

//--------------------------------------------------------------------------------
class CFWaveCutApp : public CFateApp  
{
public:
	void HideVolumeLevel(bool bHide);	
  void ShowFileLabel(bool bShow);  
  void SetFileLabelText(LPCTSTR pText);
  CFWaveCutApp();
  virtual ~CFWaveCutApp();  
  virtual bool InitFateApp();
  virtual bool ActivateFateApp();
  virtual bool CloseFateApp();  
  virtual bool ButtonPressed(DWORD dwBtnID);
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
  virtual bool StylusUp(int xPos, int yPos);
  virtual bool KeyDown(int vkKey);
  virtual bool KeyUp(int vkKey);  
  virtual void Draw();  
  void FileLoaded(BOOL bLoaded);
  void SetFileName(LPCTSTR pFileName);
  LPCTSTR GetFileName() { return(m_fileLabel->GetText()); };  
  void ChangeLevel8Bit(int iVal);
  void ChangeLevel16Bit(int iVal);
  void ResetLevel();
private:  
	void UpdatePos();
  bool SerialOK();
  CFLabel *m_fileLabel;
  CFBitmap *m_bmpTitle;
  CFBitmap *m_bmpVol;
  CFBitmap *m_bmpSlide;
  CFBitmap *m_bmpLevel;
  CFBitmap *m_bmpMask;
  CFBitmap *m_bmpCheat;
  CFPanel *m_buttonPanel;
  CFWavePanel *m_wavePanel;  
  CFLoadPanel *m_loadPanel;
  CFSavePanel *m_savePanel;  
  CFRecordPanel *m_recordPanel;  
  CFButton *m_btnLoad;
  CFButton *m_btnSave;
  CFButton *m_btnUndo;
  CFButton *m_btnRedo;
  CFButton *m_btnCutOut;
  CFButton *m_btnCutIn;
  CFButton *m_btnZoomIn;
  CFButton *m_btnZoomOut;
  CFButton *m_btnInfo;
  CFButton *m_btnQuit;  
  int m_iLevel;
  int m_iVolStep;
  int m_iPosMask;
  int m_iLevelHeight;
  int m_iSlideStart;
  int m_iSlideEnd;
  int m_iSlideCenter;  
  BOOL m_bMoveSlide;
  BOOL m_bDisplayLevel;
  DWORD m_dwVolumes[9];
};

#endif // !defined(AFX_FWAVECUTTER_H__1F709245_43D2_4FD9_849E_BF86B88BFE6B__INCLUDED_)
