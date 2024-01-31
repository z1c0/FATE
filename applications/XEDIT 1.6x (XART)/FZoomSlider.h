// FZoomSlider.h: interface for the CFZoomSlider class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FZOOMSLIDER_H__2BD79D99_6ED7_42F9_B83C_04C8F615B1EB__INCLUDED_)
#define AFX_FZOOMSLIDER_H__2BD79D99_6ED7_42F9_B83C_04C8F615B1EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FATE/FBitmap.h"

class CFWavePanel;

class CFZoomSlider  
{
public:
	void ZoomOut(DWORD dwPos);
	void ZoomIn(DWORD dwPos);
	void ChangeZoom(DWORD dwZoomStart, DWORD dwZoomEnd);
	CFZoomSlider(CFWavePanel *parent, int iWindowWidth);
	~CFZoomSlider();
	BOOL Create(CFBitmap *bmpImg);
  BOOL PointInside(int iPosX, int iPosY);
  BOOL RestoreUnder() { return(m_bmpImg->RestoreUnder());};
  BOOL SaveUnder() { return(m_bmpImg->SaveUnder());};
  CFBitmap* GetImage() { return(m_bmpImg); };
	void SetPosX(int iPosX);
  void SetPosY(int iPosY) { m_iPosY= iPosY; m_bmpImg->SetY(iPosY); };
  int GetPosX() { return(m_iPosX); };
  void Draw() { m_bmpImg->Blit(); };
  int GetWidth() { return(m_iWidth); };
  
  int GetHeight() { return(m_iHeight); };
//  int GetWaveWidth() { return(m_iWaveWidth); };
//  int GetWaveOffset() { return(m_iWaveOffset); };
  
  DWORD GetZoomBlockStart() { return(m_dwZoomStartBlock); };
  DWORD GetZoomBlockEnd() { return(m_dwZoomEndBlock); };
  DWORD GetZoomBlockCount() { return(m_dwZoomBlockCount); };
  void Reset(DWORD dwBlockCount);
private:
  CFWavePanel *m_parent;
  CFBitmap *m_bmpImg;
  
  DWORD m_dwBlockCount;
  
  DWORD m_dwZoomStartBlock;
  DWORD m_dwZoomEndBlock;
  DWORD m_dwZoomBlockCount;
  
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;
  
  // int m_iWaveOffset;
  // int m_iWaveWidth;
  
  int m_iMargin;
  int m_iWindowWidth;
};

#endif // !defined(AFX_FZOOMSLIDER_H__2BD79D99_6ED7_42F9_B83C_04C8F615B1EB__INCLUDED_)
