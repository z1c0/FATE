// FVEObj.cpp: implementation of the CFVEObj class.
//////////////////////////////////////////////////////////////////////
#include "FVEObjManager.h"
#include "FVEObj.h"


//--------------------------------------------------------------------------------
CFVEObj::CFVEObj(unsigned long dwID)
{
  m_ulID= dwID;  
  // inits
  m_szName[0]= 0;
  m_fPosX  = 0.f;
  m_fPosY  = 0.f;
  m_fPosZ  = 0.f;
  m_fAzim  = 0.f;
  m_fElev  = 0.f;
  m_fRoll  = 0.f;
  m_fScaleX= 0.f;
  m_fScaleY= 0.f;
  m_fScaleZ= 0.f;
  m_bAutoPost= true;
  m_pMgr= NULL;
}

//--------------------------------------------------------------------------------
CFVEObj::~CFVEObj()
{
  for (int i=0; i<m_ListAttr.GetSize(); i++) delete(m_ListAttr[i]);
  m_ListAttr.Clear();
}

//--------------------------------------------------------------------------------
void CFVEObj::SetName(LPCTSTR pszName)
{
  _tcsncpy(m_szName, pszName, MAX_OBJ_NAME_LEN - 1);
  m_szName[MAX_OBJ_NAME_LEN]= 0;
}

//--------------------------------------------------------------------------------
void CFVEObj::SetPosX(float fPosX)
{ 
  m_fPosX= fPosX; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendPosition(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetPosY(float fPosY)
{ 
  m_fPosY= fPosY; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendPosition(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetPosZ(float fPosZ)
{ 
  m_fPosZ= fPosZ; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendPosition(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetAzim(float fAzim)
{ 
  m_fAzim= fAzim; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendOrientation(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetElev(float fElev)
{ 
  m_fElev= fElev; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendOrientation(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetRoll(float fRoll)
{ 
  m_fRoll= fRoll; 
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendOrientation(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetScaleX(float fScaleX)
{ 
  m_fScaleX= fScaleX;
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendScale(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetScaleY(float fScaleY)
{ 
  m_fScaleY= fScaleY;
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendScale(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetScaleZ(float fScaleZ)
{ 
  m_fScaleZ= fScaleZ;
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendScale(this);
}

//--------------------------------------------------------------------------------
void CFVEObj::SetExtraAttr(LPCTSTR pszName, BYTE *pData, unsigned long dwSize)
{
  CFVEObjAttr *pAttr= new CFVEObjAttr(pszName, pData, dwSize);

  // addd attribute to list
  m_ListAttr.Append(pAttr);
  
  if ((m_bAutoPost)&&(m_pMgr)) 
    m_pMgr->SendExtra(this, pAttr);
}

//--------------------------------------------------------------------------------
CFVEObjAttr* CFVEObj::GetExtraAttr(LPCTSTR pszName)
{
  CFVEObjAttr *pAttr= NULL;

  // try to find attribute "pszName" in list
  for (int i=0; i<m_ListAttr.GetSize(); i++) {
    if (!_tcscmp(pszName, m_ListAttr[i]->GetName()))
      pAttr= m_ListAttr[i];
  }

  return(pAttr);
}

//--------------------------------------------------------------------------------
bool CFVEObj::RequestValues()
{
  if (m_pMgr) {
    return(m_pMgr->RequestValues(this));
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFVEObj::SendPosition()
{
  if (m_pMgr) {
    return(m_pMgr->SendPosition(this));
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFVEObj::SendOrientation()
{
  if (m_pMgr) {
    return(m_pMgr->SendOrientation(this));
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFVEObj::SendScale()
{
  if (m_pMgr) {
    return(m_pMgr->SendScale(this));
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFVEObj::SendExtra(LPCTSTR pszName)
{
  if (m_pMgr) {
    // try to find specified attribute
    CFVEObjAttr *pAttr= GetExtraAttr(pszName);
    if (!pAttr) return(false);
    return(m_pMgr->SendExtra(this, pAttr));
  }
  return(false);
}

//--------------------------------------------------------------------------------
bool CFVEObj::SendAll()
{
  if (m_pMgr) {
    return(m_pMgr->SendAll(this));
  }
  return(false);
}
