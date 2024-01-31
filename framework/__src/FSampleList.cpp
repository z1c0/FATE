// FSampleList.cpp: implementation of the CFSampleList class.
//////////////////////////////////////////////////////////////////////
#include "FSampleList.h"

//--------------------------------------------------------------------------------
CFSampleList::CFSampleList()
{
  // create new list with two dummy-elements
  m_dwBlockCount= 0;
  m_iSize= 0;
  m_pHead= new FSampleListNODE;
  m_pTail= new FSampleListNODE;
  memset(m_pHead, 0, sizeof(FSampleListNODE));
  memset(m_pTail, 0, sizeof(FSampleListNODE));
  m_pHead->pPrev= NULL;
  m_pHead->pNext= m_pTail;
  m_pTail->pPrev= m_pHead;
  m_pTail->pNext= NULL;

  // create iterator
  m_pIt= new CFSampleListIterator;
  m_pIt->m_pHead= m_pHead;
  m_pIt->m_pTail= m_pTail;
  m_pIt->m_pCurr= m_pHead->pNext;
}

//--------------------------------------------------------------------------------
CFSampleList::~CFSampleList()
{
  // delete iterator
  delete(m_pIt);

  // clean data elements
  Clear();

  // remove dummy-elements
  delete(m_pHead);
  delete(m_pTail);
}

//--------------------------------------------------------------------------------  
BOOL CFSampleList::Append(const DWORD dwBlockStart, const DWORD dwBlockEnd)
{
  FSampleListNODE *pTemp= new FSampleListNODE;
  if (!pTemp) return(FALSE);

  // set data
  pTemp->dwBlockStart= dwBlockStart;
  pTemp->dwBlockEnd  = dwBlockEnd;
  // set pointers
  pTemp->pPrev= m_pTail->pPrev;
  pTemp->pNext= m_pTail;  
  m_pTail->pPrev->pNext= pTemp;
  m_pTail->pPrev= pTemp;
  // increase size counter
  m_iSize++;

  return(TRUE);
}

//--------------------------------------------------------------------------------  
void CFSampleList::Clear()
{
  FSampleListNODE *pTemp= m_pHead->pNext;
  FSampleListNODE *pOld= m_pHead->pNext;

  // delete all data nodes
  while((pTemp->pNext)) {
    pOld= pTemp;
    pTemp= pTemp->pNext;
    delete(pOld);
  }
  
  // link the dummy nodes again
  m_pHead->pNext= m_pTail;
  m_pTail->pPrev= m_pHead;

  // reset size counter
  m_iSize= 0;
}

//------------------------------------------------------------------------------
CFSampleListIterator* CFSampleList::GetIterator() 
{ 
  m_pIt->Reset(); 
  return(m_pIt); 
};


//--------------------------------------------------------------------------------
/// Performs cut on the current samples.
BOOL CFSampleList::Cut(DWORD dwFrom, DWORD dwTo)
{
  FSampleListNODE *pTemp;
  FSampleListNODE *pCurr;
  DWORD dwSave;
  DWORD dwCurrLeft= 0;
  DWORD dwCurrRight;
  DWORD dwToCut= dwTo - dwFrom;

  // find position in sample list
  m_pIt->Reset();
  // find position in sample list
  while(dwFrom >= dwCurrLeft + m_pIt->GetBlocksLen()) {
    dwCurrLeft+= m_pIt->GetBlocksLen();
    m_pIt->Next();
  }

  while (dwToCut) {
    dwCurrRight= dwCurrLeft + m_pIt->GetBlocksLen();
    
    if ((dwFrom > dwCurrLeft)&&(dwTo < dwCurrRight)) {  // CASE 1
      // change data of current sample node
      dwSave= m_pIt->GetBlockEnd();
      m_pIt->SetBlockEnd(dwFrom - dwCurrLeft + m_pIt->GetBlockStart());
  
      // create new node
      pTemp= new FSampleListNODE;
      if (!pTemp) return(FALSE);

      // set data
      pTemp->dwBlockStart= dwTo - dwCurrLeft + m_pIt->GetBlockStart();
      pTemp->dwBlockEnd  = dwSave;
  
      // set pointers
      pCurr= m_pIt->GetCurrent();
      pCurr->pNext->pPrev= pTemp;
      pTemp->pPrev= pCurr;
      pTemp->pNext= pCurr->pNext;  
      pCurr->pNext= pTemp;      
  
      // increase size counter
      m_iSize++;

      dwToCut= 0;
    
    } else if ((dwFrom > dwCurrLeft)&&(dwTo > dwCurrRight)) {  // CASE 2
      // change data of current sample node
      dwSave= m_pIt->GetBlocksLen();
      m_pIt->SetBlockEnd(dwFrom - dwCurrLeft + m_pIt->GetBlockStart());

      // adapt "samples to cut", "left border" and "cut-from value"
      dwToCut-= (dwCurrRight - dwFrom);     
      dwFrom= dwCurrRight;
      dwCurrLeft+= dwSave;
      
      // get next sample node
      m_pIt->Next();      
    
    } else if ((dwFrom == dwCurrLeft)&&(dwTo >= dwCurrRight)) {   // CASE 3
      // adapt values
      dwToCut-= (dwCurrRight - dwCurrLeft); 
      dwFrom= dwCurrRight;
      dwCurrLeft+= m_pIt->GetBlocksLen();

      // set pointers

      pCurr= m_pIt->GetCurrent();
      m_pIt->SetCurrent(pCurr->pNext);
      pCurr->pPrev->pNext= pCurr->pNext;
      pCurr->pNext->pPrev= pCurr->pPrev;      
      delete(pCurr);

      // decrease size counter
      m_iSize--;

    } else if ((dwFrom == dwCurrLeft)&&(dwTo < dwCurrRight)) {  // CASE 4
      // change data of current sample node
      m_pIt->SetBlockStart(m_pIt->GetBlockStart() + dwToCut);

      dwToCut= 0;
    }
  }

  return(TRUE);
}

//--------------------------------------------------------------------------------
CFSampleList* CFSampleList::Clone()
{
  FSampleListNODE *pNode;
  CFSampleList* pList= new CFSampleList();
  
  // iterate through all nodes and deep copy them
  pNode= m_pHead->pNext;
  while((pNode->pNext)) {
    pList->Append(pNode->dwBlockStart, pNode->dwBlockEnd);
    pNode= pNode->pNext;
  }
  pList->SetBlockCount(m_dwBlockCount);

  return(pList);
}