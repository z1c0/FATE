// FSampleList.h: interface for the CFSampleList class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FSampleList_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_)
#define AFX_FSampleList_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

//--------------------------------------------------------------------------------
/// Forward declaration.
class CFSampleListIterator;

//--------------------------------------------------------------------------------
/// List datastructure.
class CFSampleList  
{
public:
  struct FSampleListNODE
  {
    DWORD dwBlockStart;
    DWORD dwBlockEnd;
    struct FSampleListNODE *pNext;
    struct FSampleListNODE *pPrev;
  };
	
  CFSampleList();
  ~CFSampleList();

  BOOL IsEmpty() const { return(m_iSize == 0); };
  void Clear();
  BOOL Append(const DWORD dwBlockStart, const DWORD dwBlockEnd);
  CFSampleListIterator* GetIterator();
  BOOL Cut(DWORD dwFrom, DWORD dwTo);
  CFSampleList* Clone();
  void SetBlockCount(DWORD dwCount) { m_dwBlockCount= dwCount; };
  DWORD GetBlockCount() { return(m_dwBlockCount); };

private:

  FSampleListNODE *m_pHead;
  FSampleListNODE *m_pTail;
  CFSampleListIterator *m_pIt;
  BOOL m_iSize;
  DWORD m_dwBlockCount;
};



//--------------------------------------------------------------------------------
/// Iterator for list.
class CFSampleListIterator
{
  friend class CFSampleList;

public:

  CFSampleListIterator() {};
  BOOL HasMore() { return(m_pCurr != m_pTail); };
  DWORD GetBlockStart() { return(m_pCurr->dwBlockStart); };
  DWORD GetBlockEnd() { return(m_pCurr->dwBlockEnd); };
  DWORD GetBlocksLen()  { return(m_pCurr->dwBlockEnd - m_pCurr->dwBlockStart); };
  void SetBlockStart(DWORD dwStart) { m_pCurr->dwBlockStart= dwStart; };
  void SetBlockEnd(DWORD dwEnd) { m_pCurr->dwBlockEnd= dwEnd; }
  DWORD GetSkip() { return(m_dwSkip); };
  DWORD GetOffs() { return(m_dwOffs); };
  void IncSkip(DWORD dwInc) { m_dwSkip+= dwInc; };
  void IncOffs(DWORD dwInc) { m_dwOffs+= dwInc; };
  void Next() { m_pCurr= m_pCurr->pNext; m_iPos++; m_dwOffs= 0; };
  void Reset() { m_pCurr= m_pHead->pNext; m_iPos= 0; m_dwSkip= 0; m_dwOffs= 0;};
  
  CFSampleList::FSampleListNODE* GetCurrent() { return(m_pCurr); };
  void SetCurrent(CFSampleList::FSampleListNODE *pNode) { m_pCurr= pNode; };

private:

  CFSampleList::FSampleListNODE *m_pCurr;       /// pointer to current position in list
  CFSampleList::FSampleListNODE *m_pHead;
  CFSampleList::FSampleListNODE *m_pTail;
  int m_iPos;
  DWORD m_dwSkip;
  DWORD m_dwOffs;
};



#endif // !defined(AFX_FSampleList_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_)
