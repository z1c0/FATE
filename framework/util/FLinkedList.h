// FLinkedList.h: interface for the CFLinkedList class.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FLINKEDLIST_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_)
#define AFX_FLINKEDLIST_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_


//--------------------------------------------------------------------------------
/// An entry in the list.
template <class Data>
struct LISTNODE
{
  Data data;
  LISTNODE<Data> *pNext;
  LISTNODE<Data> *pPrev;
};


//--------------------------------------------------------------------------------
/// Forward declaration.
template <class Data>
class CFListIterator;


//--------------------------------------------------------------------------------
/// List datastructure.
template <class Data>
class CFLinkedList  
{
public:
  
  //------------------------------------------------------------------------------
  CFLinkedList()
  {
    // create new list with two dummy-elements
    m_iSize= 0;
    m_pHead= new LISTNODE<Data>;
    m_pTail= new LISTNODE<Data>;
    m_pHead->pPrev= 0x00;
    m_pHead->data= 0;
    m_pHead->pNext= m_pTail;
    m_pTail->pPrev= m_pHead;
    m_pTail->data= 0;
    m_pTail->pNext= 0x00;

    // create iterator
    m_pIt= new CFListIterator<Data>;
    m_pIt->m_pHead= m_pHead;
    m_pIt->m_pTail= m_pTail;
    m_pIt->m_pCurr= m_pHead->pNext;
  };
  
  //------------------------------------------------------------------------------
  ~CFLinkedList()
  {
    // clean data elements
    Clear();

    // remove dummy-elements
    delete(m_pHead);
    delete(m_pTail);

    // delete iterator
    delete(m_pIt);  
  };

  //------------------------------------------------------------------------------
  bool Append(const Data data)
  {
    LISTNODE<Data> *pTemp= new LISTNODE<Data>;
    if (!pTemp) return(false);

    // set pointers
    pTemp->data= data;
    pTemp->pPrev= m_pTail->pPrev;
    pTemp->pNext= m_pTail;  
    m_pTail->pPrev->pNext= pTemp;
    m_pTail->pPrev= pTemp;
    // increase size counter
    m_iSize++;

    return(true);
  };
  
  //------------------------------------------------------------------------------
  bool IsEmpty() const { return(m_iSize == 0); };
  
  //------------------------------------------------------------------------------
  void Clear()
  {
    LISTNODE<Data> *pOld, *pTemp;
    pOld= pTemp= m_pHead->pNext;

    // delete all data nodes
    while(pTemp->pNext) {
      pOld= pTemp;
      pTemp= pTemp->pNext;
      delete(pOld);
    }
  
    // link the dummy nodes again
    m_pHead->pNext= m_pTail;
    m_pTail->pPrev= m_pHead;

    // reset size counter
    m_iSize= 0;
  };

  //------------------------------------------------------------------------------
  /// Inserts the specified data at the given position.
  /// If position i is < 0 the method will do nothing and return FALSE.
  /// If position i is > Size-1 the data will be inserted last.
  bool Insert(int i, const Data data)
  {
    LISTNODE<Data> *pCurr= m_pHead->pNext;
    LISTNODE<Data> *pTemp= new LISTNODE<Data>;
    int j= 0;

    if (!pTemp) return false;

    if (i < 0) return false;
    else if (i > m_iSize - 1) return(Append(data));

    // go to specified position
    while(j++ < i) {
      pCurr= pCurr->pNext;
    }

    // set pointers
    pTemp->data= data;
    pTemp->pPrev= pCurr->pPrev;
    pTemp->pNext= pCurr;  
    pCurr->pPrev->pNext= pTemp;
    pCurr->pPrev= pTemp;
    // Increase size counter.
    m_iSize++;

    return true;
  };

  //------------------------------------------------------------------------------
  int GetSize() { return m_iSize; };

  //------------------------------------------------------------------------------
  Data GetHead() const
  {
    return m_pHead->pNext->data;
  }

  //------------------------------------------------------------------------------
  Data GetTail() const
  {
    return m_pTail->pPrev->data;
  }

  //------------------------------------------------------------------------------
  Data Get(int i) const
  {
    Data ret;
    int j= 0;

    if ((i < 0)||(i > m_iSize - 1)) return(0x00);
  
    m_pIt->Reset();
    while(j++ < i) {
      m_pIt->Next();
    }
    ret= m_pIt->GetData();
    m_pIt->Reset();
    return(ret);
  }

  //------------------------------------------------------------------------------
  bool Delete(int i)
  {
    LISTNODE<Data> *pNode;
    int j= 0;

    if ((i < 0)||(i > m_iSize - 1)) return(false);
  
    pNode= m_pHead->pNext;
    while(j++ < i) {
      pNode= pNode->pNext;
    }
  
    // unlink and delete node
    pNode->pPrev->pNext= pNode->pNext;
    pNode->pNext->pPrev= pNode->pPrev;
    if (pNode->data) delete(pNode->data);
    delete(pNode);
    m_iSize--;

    return(true);
  };

  //------------------------------------------------------------------------------
  bool Delete(const Data data)
  {
    LISTNODE<Data> *pNode;
 
    pNode= m_pHead->pNext;
    while(pNode->data != data) {
      pNode= pNode->pNext;
    }
    // found?
    if (pNode->data != data) return(false);

    // unlink and delete node
    pNode->pPrev->pNext= pNode->pNext;
    pNode->pNext->pPrev= pNode->pPrev;
    if (pNode->data) delete(pNode->data);
    delete(pNode);
    m_iSize--;

    return(true);
  };
 
  //------------------------------------------------------------------------------
  Data operator[](int i) { return(Get(i)); };
  
  //------------------------------------------------------------------------------
  CFListIterator<Data>* GetIterator() { m_pIt->Reset(); return(m_pIt); };

private:

  LISTNODE<Data> *m_pHead;
  LISTNODE<Data> *m_pTail;
  CFListIterator<Data> *m_pIt;
  int m_iSize;
};


//--------------------------------------------------------------------------------
/// Iterator for list.
template <class Data>
class CFListIterator
{
  friend class CFLinkedList<Data>;
public:

  CFListIterator() {};
  bool HasMore() { return ((m_pCurr != m_pTail)&&(m_pCurr != m_pHead)); };
  Data GetData() { return m_pCurr->data; };
  void Next() { m_pCurr = m_pCurr->pNext; };
  void Previous() { m_pCurr = m_pCurr->pPrev; };
  void Reset() { m_pCurr= m_pHead->pNext; };
  void ResetToEnd() { m_pCurr = m_pTail->pPrev; };

private:

  LISTNODE<Data> *m_pCurr;       /// pointer to current position in list
  LISTNODE<Data> *m_pHead;
  LISTNODE<Data> *m_pTail;
};

#endif // !defined(AFX_FLINKEDLIST_H__E2427ADF_F838_464D_924B_87632DFCD4CE__INCLUDED_)
