// PPTRCv2Dlg.h : header file
#if !defined(AFX_PPTRCV2DLG_H__04844A1F_6AF7_4513_9AFA_BF8D51327DF5__INCLUDED_)
#define AFX_PPTRCV2DLG_H__04844A1F_6AF7_4513_9AFA_BF8D51327DF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "msppt.h"


//--------------------------------------------------------------------------------
class CPPTRCv2Dlg : public CDialog
{
// Construction
	DECLARE_DYNCREATE(CPPTRCv2Dlg)
	CPPTRCv2Dlg(CWnd* pParent = NULL);	// standard constructor
public:
// Dialog Data
	//{{AFX_DATA(CPPTRCv2Dlg)
	enum { IDD = IDD_PPTRCV2_DIALOG };
	CListBox	m_ListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPTRCv2Dlg)
	protected:
	virtual void OnFinalRelease();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
  //void WindowSelectionChange(LPDISPATCH Pres);
	//void WindowBeforeRightClick(LPDISPATCH Pres, VARIANT_BOOL* Cancel);
	//void WindowBeforeDoubleClick(LPDISPATCH Pres, VARIANT_BOOL* Cancel);
	void PresentationClose(LPDISPATCH Pres);
	//void PresentationSave(LPDISPATCH Pres);
	//void PresentationOpen(LPDISPATCH Pres);
	//void NewPresentation(LPDISPATCH Pres);
	//void PresentationNewSlide(LPDISPATCH Pres);
	//void WindowActivate(LPDISPATCH Pres,LPDISPATCH Wn);
	//void WindowDeactivate(LPDISPATCH Pres, LPDISPATCH Wn);
	void SlideShowBegin(LPDISPATCH Wn);
	//void SlideShowNextBuild(LPDISPATCH Wn);
  void SlideShowNextSlide(LPDISPATCH Wn);
	void SlideShowEnd(LPDISPATCH Pres);
	//void PresentationPrint(LPDISPATCH Pres);
	//void SlideSelectionChanged(LPDISPATCH SldRange);
	//void ColorSchemeChanged(LPDISPATCH SldRange);
	//void PresentationBeforeSave(LPDISPATCH Pres, VARIANT_BOOL * Cancel);
	//void SlideShowNextClick(LPDISPATCH Wn, LPDISPATCH nEffect);
	// Generated message map functions
	//{{AFX_MSG(CPPTRCv2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
  BOOL SendResponse();
  int OnReceive(char cmd, char *msg);
  void PrepareResponse(char cmd, const char *data, DWORD len= -1);
  void CPPTRCv2Dlg::CreatePreview(long index);
  int RunPPT();
  int ClosePPT();
  BOOL IsPPTRunning();
  int OpenSlideShow(char *file);
  int StopSlideShow();
  int CloseSlideShow();
  BOOL OpenSlideShowExists();
  int ReturnToSlideShow();
  void RunSlideShow();
  int NextSlide();
  int PrevSlide();
  int FirstSlide();
  int LastSlide();
  int GotoSlide(long nr);
  int LastHyperLink();
  int NextHyperLink();
  int HyperLinkMouseClick();
  int WhiteScreen();
  int BlackScreen();
  int HidePointer();
  int ShowMouse();
  int ClearNotes();
  int GotoHidden();
  int TimeOld();
  int TimeNew();
  int MouseMode();
  int ShowArrow();
  int ShowPen();
  int ContextMenu();
  BOOL PrepareDriveList();
  BOOL PrepareFileInfo(char *pszDir);
  BOOL EstabConn();
  BOOL Unregister();
  DWORD m_dwCookie;
  _Application m_appPPT;
  _Presentation m_pres;
  SlideShowView m_view;
  SOCKET m_sockListen;  
  int m_iSize;        
  int m_iWidth;
  int m_iHeight;
  char *m_pResp;
  DWORD m_dwRespLen;
  char m_szImagePath[MAX_PATH];
  char m_szCurrPath[MAX_PATH];
  IConnectionPoint *m_pConnectionPoint;
  BOOL m_bOK;
  sockaddr_in m_addrClient;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPTRCV2DLG_H__04844A1F_6AF7_4513_9AFA_BF8D51327DF5__INCLUDED_)
