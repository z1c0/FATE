// FRemoteButton.cpp: implementation of the CFRemoteButton class.
//////////////////////////////////////////////////////////////////////
#include "FRemoteButton.h"

//--------------------------------------------------------------------------------
CFRemoteButton::CFRemoteButton(LPCTSTR pCaption) : CFButton(pCaption), IFRemoteWidget()
{
}

//--------------------------------------------------------------------------------
CFRemoteButton::~CFRemoteButton()
{
}

//--------------------------------------------------------------------------------
bool CFRemoteButton::Action()
{
  return(false);
}