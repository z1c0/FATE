// FRec.h: interface for the CFRec class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FREC_H__97303869_83FB_4329_B988_F60BF390A316__INCLUDED_)
#define AFX_FREC_H__97303869_83FB_4329_B988_F60BF390A316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFateComponent.h"
#include "FLinkedList.h"

//--------------------------------------------------------------------------------
class CFRec : public IFateComponent  
{
  struct RECSTRUCT
  {
    char szData[1024];
    int iFull;
  };

public:

	CFRec();
	virtual ~CFRec();

  BOOL StartRecording();
  BOOL StopRecording();
  BOOL PauseRecording();
  BOOL StartPlayback();
  BOOL StopPlayback();
  BOOL PausePlayback();

protected:

  CFLinkedList<RECSTRUCT*> m_RecList;
};

#endif // !defined(AFX_FREC_H__97303869_83FB_4329_B988_F60BF390A316__INCLUDED_)
