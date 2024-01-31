// Game.h: interface for the CGame class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_GAME_H__30821ECC_4ADC_441D_8608_3920397FF2FA__INCLUDED_)
#define AFX_GAME_H__30821ECC_4ADC_441D_8608_3920397FF2FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
#define MAX_NR_OF_CELLS    64
#define BOUND_L            20
#define BOUND_U            20
#define BOUND_R           300
#define BOUND_D           220

class CCell;

//--------------------------------------------------------------------------------
class CGame : public CFateApp  
{
public:
	CGame();
	virtual ~CGame();
  bool InitFateApp();
  void Draw();
  virtual bool ButtonReleased(DWORD dwBtnID);
  virtual bool StylusDown(int iPosX, int iPosY);
  
  virtual void Idle();

private:

  bool CreateCells();
  CFBitmap *m_bmpPitch;
  CFButton *m_btnQuit;
  CCell* m_pCells[MAX_NR_OF_CELLS];
  DWORD m_dwOld;
  DWORD m_dwFrames;
  int m_iChase;
};

#endif // !defined(AFX_GAME_H__30821ECC_4ADC_441D_8608_3920397FF2FA__INCLUDED_)
