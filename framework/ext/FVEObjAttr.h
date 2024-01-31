#ifndef __FVEOBJATTR__H__
#define __FVEOBJATTR__H__

#include "../base/FateTypeDefs.h"
#include "ve_protocol.h"

//--------------------------------------------------------------------------------
class CFVEObjAttr  
{
public:
	CFVEObjAttr(const TCHAR* szName, unsigned char* pData, unsigned long ulSize);
	virtual ~CFVEObjAttr();
  const TCHAR* GetName() { return(m_szName); };
  unsigned char* GetData() { return(m_pData); };
  unsigned long GetSize() { return(m_ulSize); };

protected:
  
  TCHAR m_szName[MAX_ATTR_NAME_LEN];
  unsigned char* m_pData;
  unsigned long m_ulSize;
};

#endif  // __FVEOBJATTR__H__
