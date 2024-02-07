// FVRena.h: interface for the CFVRena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FVRENA_H__FAFA267E_68D0_4902_91E9_E1FB95FC87C5__INCLUDED_)
#define AFX_FVRENA_H__FAFA267E_68D0_4902_91E9_E1FB95FC87C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../framework/include/fate.h"
#include "FVRPanel.h"
#include "FPPTPanel.h"
#include "FScreens.h"

//--------------------------------------------------------------------------------
/// Constants
#define MAX_RENDER_NODES  6
#define MAX_VIDEO_NODES   3
#define ROTATION_STEP     5
#define ROT_X             0x01
#define ROT_Z             0x02
#define MODEL_STATE_ROT   0x01
#define MODEL_STATE_SIZE  0x02


//--------------------------------------------------------------------------------
class CFVRena : public CFateApp  
{
public:
	CFVRena();
	virtual ~CFVRena();
  virtual bool InitFateApp();
	virtual bool ActivateFateApp();
	virtual bool CloseFateApp();
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
  virtual bool StylusUp(int xPos, int yPos) { return(FALSE); };
  virtual bool KeyDown(PdaKey key);
  virtual bool KeyUp(PdaKey key) { return(FALSE); };
  virtual void Draw();
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool ItemListSelected(DWORD dwListID, ITEMLISTENTRY* pEntry);
  virtual bool DropListExpanded(DWORD dwListID);
  virtual bool DropListCollapsed(DWORD dwListID);
  virtual bool Timer(unsigned long id);

  BOOL AddItem(int iNr, TCHAR *pszItem, TCHAR *pszAddInfo);
  int GetMediaType() { return(m_iMediaType); };
  void SetMediaType(int iMediaType) { m_iMediaType= iMediaType; };
  
  bool SendToMediaHost(int iScreen, char *pMsg);
  bool SendToMatrixHost(char *pMsg);
  
  void ActivateControls(bool bActivate) { m_panelCtls->SetVisible(bActivate); };
  void EnableController(bool bEnabled) { m_cont->SetEnabled(bEnabled); };
  void ShowVR() { m_panelCtls->SetVisible(FALSE); m_panelVR->SetVisible(TRUE); };
  BOOL SendCaveMenuChange(int iNr);
  BOOL SendTrackerReset();
  BOOL ModelLoad(char *pszModel);
  BOOL ModelRotate(int iAxis, int  iAngle);
  BOOL ModelSize(float fSize);
  CFInetAddr* GetModelAddr() { return(m_addrModel[0]); };

private:

  void DisplayIntro();
  BOOL ReadConfigFile(LPCTSTR pszFileName);
  void Error(LPCTSTR pszErrMsg);
  BOOL FillItemListsFromServer();
  BOOL m_bIntro;

  CFVRPanel *m_panelVR;
  CFPPTPanel *m_panelPPT[MAX_VIDEO_NODES];
  CFScreens *m_screens;
  
  CFPanel *m_panelCtls;
  
  CFItemList *m_aItemLists[4];
  
  CFButton *m_btnClose;
  
  CFLabel *m_labelInfo;
  CFLabel *m_labelTime;
  
  CFController *m_cont;
  
  CFBitmap *m_bmpTitle;
  CFBitmap *m_bmpDrag;
  CFBitmap *m_bmpPPT[2];
  CFBitmap *m_bmpVideo[2];
  CFBitmap *m_bmpPic[2];
  CFBitmap *m_bmpVR[2];
  CFBitmap *m_bmp3D[2];
  
  CFInetAddr *m_addrVideo[MAX_VIDEO_NODES];      // one socket-address for each windows player
  CFInetAddr *m_addrMatrix;                      // socket address for matrix switch
  CFInetAddr *m_addrVR[MAX_RENDER_NODES];        // socket addresses for arsbox hosts
  CFInetAddr *m_addrModel[MAX_RENDER_NODES];     // socket addresses for arsbox hosts
  CFInetAddr *m_addrTrack;
  
  CFUDPSocket *m_sockTrack;
  
  SYSTEMTIME m_time;
  
  int m_iMediaType;  // PPT, Video, Pic, Ext
  int m_iListIndent;
  int m_iMaxItems;
  int m_iOldX;
  int m_iOldY;
  BOOL m_bDropListEx;
  int m_iVrHostCount;
  int m_iModelHostCount;
  BOOL m_bModelView;
  float m_fModelSize;
  int m_iModelRotX;
  int m_iModelRotZ;
  int m_iModelState;
  BOOL m_bFirst;
  BOOL m_bHave3DList;
};

#endif // !defined(AFX_FVRENA_H__FAFA267E_68D0_4902_91E9_E1FB95FC87C5__INCLUDED_)
