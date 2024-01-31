// FMarker.h: interface for the CFMarker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMARKER_H__11B15C3F_84E4_42C4_9476_392C8DB76EC8__INCLUDED_)
#define AFX_FMARKER_H__11B15C3F_84E4_42C4_9476_392C8DB76EC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../FATE/FBitmap.h"

class CFWavePanel;

class CFMarker  
{
public:	
  CFMarker(CFWavePanel *parent);
	~CFMarker();	
  BOOL Create(CFBitmap *bmpImg);
  BOOL PointInside(int iPosX, int iPosY);  
  BOOL RestoreUnder() { if (m_bSet) return(m_bmpImg->RestoreUnder()); return(FALSE);};
  BOOL SaveUnder() { if (m_bSet) return(m_bmpImg->SaveUnder()); return(FALSE); };  
  CFBitmap* GetImage() { return(m_bmpImg); };	
  void SetPosX(int iPosX);
  void SetPosY(int iPosY) { m_iPosY= iPosY; m_bmpImg->SetY(iPosY); };
  int GetPosX() { return(m_iPosX); };  
  void Draw() { if (m_bSet) m_bmpImg->Blit(); };
  int GetWidth() { return(m_iWidth); };
  int GetHeight() { return(m_iHeight); };  
  void SetBlockPos(DWORD dwBlockPos);
  DWORD GetBlockPos() { return(m_dwBlockPos); };
  DWORD GetTimePos();  
  void Set();
  void Unset() { m_bSet= FALSE; m_dwBlockPos = 0; };
  BOOL IsSet() { return(m_bSet); };  
private:
  CFWavePanel *m_parent;
  CFBitmap *m_bmpImg;  
  DWORD m_dwBlockPos;  
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;
  int m_iMargin;  
  BOOL m_bSet;
};

#endif // !defined(AFX_FMARKER_H__11B15C3F_84E4_42C4_9476_392C8DB76EC8__INCLUDED_)
