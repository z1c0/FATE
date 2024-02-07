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
bool CFInfoPanel::StylusDown(int xPos, int yPos)
{
  if (m_app->GetCaptureComp() == this) {
    m_app->ReleaseCaptureComp();
    m_pSystem->ForceRedraw();
    return(TRUE);
  }
  return(FALSE);
}
