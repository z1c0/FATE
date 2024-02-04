// FRecordPanel.h: interface for the CFRecordPanel class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FRECORDPANEL_H__0C95036B_0684_45A8_A1A3_4AD46EDF54BF__INCLUDED_)
#define AFX_FRECORDPANEL_H__0C95036B_0684_45A8_A1A3_4AD46EDF54BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--------------------------------------------------------------------------------
#define MIN_SPACE_FREE 204800 // minimum free space on file system while recording


//--------------------------------------------------------------------------------
class CFRecordPanel : public CFPanel  
{
friend void RecordCallbackFunc(char *pData, DWORD dwSize, LPVOID pvUserData);
public:
	DWORD GetFreeRecSpace();
	virtual bool ButtonPressed(DWORD dwBtnID) { return(FALSE); };
  virtual bool ButtonReleased(DWORD dwBtnID); 
	virtual bool DropListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  CFRecordPanel(LPCTSTR pFilePath);
	virtual ~CFRecordPanel();  
  void SetNextFileName();
  void SetFilePath(LPCTSTR pFilePath);
  LPCTSTR GetFilePath() { return (LPCTSTR)&m_szFilePath; };
  LPCTSTR GetFileName() { return(m_szFullFileName); };
  LPCTSTR GetOldFileName() { return(m_pOldFileName); };
  void SetOldFileName(LPCTSTR pOldFileName) { m_pOldFileName= pOldFileName; };  
  BOOL Create();  
  virtual void SetX(int iPosX);
  virtual void SetY(int iPosY);
  BOOL StartRecording();
  BOOL PauseRecording();
  BOOL ResumeRecording();
  BOOL StopRecording();  
  BOOL IsSaved() { return m_bSaved; };
	void UpdateRecordLabels();
  void DirSelected(bool bSelected);
private:
	void SetLabelsVisible(bool bVisible);
  void UpdatePos();
  CFButton *m_btnRec;
  CFButton *m_btnEdit;
  CFButton *m_btnPause;
  CFButton *m_btnDir;
  CFLabel *m_dirLabel;
  CFLabel *m_totLabel;
  CFLabel *m_statLabel;
  CFLabel *m_remainingLabel;
  CFLabel *m_sizeLabel;
  CFLabel *m_freeLabel;
  CFDropList *m_samplingRateList;
  CFDropList *m_bitDepthList;
  CFDropList *m_channelsList;  
  CFDirPanel *m_dirPanel;
  BOOL m_bCreated;
  BOOL m_bSaved;
  LPCTSTR m_pOldFileName;
  TCHAR m_szFilePath[MAX_PATH];
  TCHAR m_szFullFileName[MAX_PATH + 256];
  WAVEFORMATEX m_waveFmt;
};

#endif // !defined(AFX_FRECORDPANEL_H__0C95036B_0684_45A8_A1A3_4AD46EDF54BF__INCLUDED_)
