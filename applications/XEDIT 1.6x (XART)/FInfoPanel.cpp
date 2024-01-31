// FInfoPanel.cpp: implementation of the CFInfoPanel class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FInfoPanel.h"


//--------------------------------------------------------------------------------
CFInfoPanel::CFInfoPanel()
{
}

//--------------------------------------------------------------------------------
CFInfoPanel::~CFInfoPanel()
{
  delete(m_bmpFlabInfo);
}

//--------------------------------------------------------------------------------
BOOL CFInfoPanel::StylusDown(int xPos, int yPos)
{
  if (m_app->GetCaptureComp() == this) {
    m_app->ReleaseCaptureComp();
    InvalidateRect(m_hWnd, NULL, FALSE);
    return(TRUE);
  }
  return(FALSE);
}
