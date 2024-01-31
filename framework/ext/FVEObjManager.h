#ifndef __FVEOBJMANAGER__H__
#define __FVEOBJMANAGER__H__

#include "FVEObj.h"
#include "../comm/FInetAddr.h"
#include "ve_protocol.h"


//--------------------------------------------------------------------------------
class CFVEObjManager  
{
public:
	CFVEObjManager();
	virtual ~CFVEObjManager();
  bool Refresh();
  bool Update();
  void Clear();
  bool ReadConfigFile(const TCHAR* pszFileName);
  bool ReadConfigData(char *pszData);
  void SetDestAddr(CFInetAddr *pAddr) 
  { 
    SAFE_DELETE(m_pAddrServer); 
    m_pAddrServer= pAddr; 
  };
  bool SendPosition(CFVEObj *pObj);
  bool SendOrientation(CFVEObj *pObj);
  bool SendScale(CFVEObj *pObj);
  bool SendExtra(CFVEObj *pObj, CFVEObjAttr *pAttr);
  bool SendAll(CFVEObj *pObj);
  bool RequestValues(CFVEObj *pObj);
  bool RequestAllValues();
  int GetObjCount() { return(m_ListObj.GetSize()); };
  CFVEObj* GetObj(int i) { return(m_ListObj.Get(i)); };

protected:
  
  CFLinkedList<CFVEObj*> m_ListObj;
  CFInetAddr *m_pAddrServer;
};

#endif // __FVEOBJMANAGER__H__
