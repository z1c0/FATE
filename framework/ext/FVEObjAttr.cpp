#include "FVEObjAttr.h"

//--------------------------------------------------------------------------------
CFVEObjAttr::CFVEObjAttr(const TCHAR* szName, unsigned char* pData, unsigned long ulSize)
{
  _tcsncpy(m_szName, szName, MAX_ATTR_NAME_LEN - 1);
  m_szName[MAX_ATTR_NAME_LEN]= 0;
  m_ulSize= ulSize;
  m_pData = new unsigned char[m_ulSize];
  memcpy(m_pData, pData, m_ulSize);
}

//--------------------------------------------------------------------------------
CFVEObjAttr::~CFVEObjAttr()
{
  SAFE_DELETE(m_pData);
}
