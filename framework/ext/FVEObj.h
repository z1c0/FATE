#ifndef __FVEOBJ__H__
#define __FVEOBJ__H__


#include "FVEObjAttr.h"
#include "../util/FLinkedList.h"

//------------------------------------------------------------------------------
class CFVEObjManager;

//--------------------------------------------------------------------------------
class CFVEObj  
{
public:

	CFVEObj(unsigned long ulID);
	virtual ~CFVEObj();
  unsigned long GetID() { return(m_ulID); };
  void SetName(const TCHAR* pszName);
  const TCHAR* GetName() { return(m_szName); };
  
  // position 
  void SetPosX(float fPosX); 
  float GetPosX() { return (m_fPosX); };
  void SetPosY(float fPosY); 
  float GetPosY() { return (m_fPosY); };
  void SetPosZ(float fPosZ); 
  float GetPosZ() { return (m_fPosZ); };
  
  // orientation
  void SetAzim(float fAzim); 
  float GetAzim() { return (m_fAzim); };
  void SetElev(float fElev); 
  float GetElev() { return (m_fElev); };
  void SetRoll(float fRoll); 
  float GetRoll() { return (m_fRoll); };
  
  // scale
  void SetScaleX(float fScaleX); 
  float GetScaleX() { return (m_fScaleX); };
  void SetScaleY(float fScaleY); 
  float GetScaleY() { return (m_fScaleY); };
  void SetScaleZ(float fScaleZ); 
  float GetScaleZ() { return (m_fScaleZ); };
  
  // extra attributes
  void SetExtraAttr(const TCHAR* pszName, unsigned char* pData, unsigned long dwSize);
  CFVEObjAttr* GetExtraAttr(int i) { return(m_ListAttr[i]); };
  CFVEObjAttr* GetExtraAttr(const TCHAR* pszName);
  int GetExtraAttrCount() { return(m_ListAttr.GetSize()); };
  
  // communication
  void SetAutoPost(bool bAutoPost) { m_bAutoPost= bAutoPost; };  
  bool RequestValues();
  bool SendPosition();
  bool SendOrientation();
  bool SendScale();
  bool SendExtra(const TCHAR* pszName);
  bool SendAll();

protected:
  
  CFLinkedList<CFVEObjAttr*> m_ListAttr;
  unsigned long m_ulID;
  TCHAR m_szName[MAX_OBJ_NAME_LEN];
  float m_fPosX;
  float m_fPosY;
  float m_fPosZ;
  float m_fAzim;
  float m_fElev;
  float m_fRoll;
  float m_fScaleX;
  float m_fScaleY;
  float m_fScaleZ;
  bool m_bAutoPost;

private:
  
  CFVEObjManager *m_pMgr;
};

#endif // __FVEOBJ__H__
