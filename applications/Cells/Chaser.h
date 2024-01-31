// Chaser.h: interface for the CChaser class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CHASER_H__F2FD8759_F2A8_4F0B_8340_C0868A0CD07D__INCLUDED_)
#define AFX_CHASER_H__F2FD8759_F2A8_4F0B_8340_C0868A0CD07D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cell.h"

//--------------------------------------------------------------------------------
class CChaser : public CCell
{
public:
	CChaser();
	virtual ~CChaser();
  virtual bool Create(const CFBitmap& destbmp);
  virtual void Move(int x, int y);
protected:
  int CCW(POINT p0, POINT p1, POINT p2);
};

#endif // !defined(AFX_CHASER_H__F2FD8759_F2A8_4F0B_8340_C0868A0CD07D__INCLUDED_)
