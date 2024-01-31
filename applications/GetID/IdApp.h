#ifndef __IDAPP__H__
#define __IDAPP__H__

#include "../../framework/include/fate.h"

//--------------------------------------------------------------------------------
#define ID_BTN_QUIT          0x00F0
#define ID_BTN_GENERATE      0x00F1
#define ID_BTN_SAVE          0x00F2


//--------------------------------------------------------------------------------
class CIdApp : public CFateApp  
{
public:

	CIdApp();
	virtual ~CIdApp();

  virtual bool InitFateApp();

  virtual void Draw();

  virtual bool ButtonReleased(DWORD dwBtnID);
  
private:
  
  void GetID();
  void SaveID();

  CFButton *m_pBtnQuit;
  CFButton *m_pBtnGenerate;
  CFButton *m_pBtnSave;
  
  CFLabel *m_pLabelID;
  CFLabel *m_pLabelStatus;

  CFBitmap *m_pBmpBack;
  
  TCHAR m_szID[256];
  int m_iLenID;
};

#endif  // __IDAPP__H__
