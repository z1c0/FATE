// FInfoPanel.h: interface for the CFInfoPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINFOPANEL_H__E36411F4_A36C_4851_A550_24997CF8209C__INCLUDED_)
#define AFX_FINFOPANEL_H__E36411F4_A36C_4851_A550_24997CF8209C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFInfoPanel : public CFPanel
{
public:
	CFInfoPanel();
	virtual ~CFInfoPanel();
  void Create(CFBitmap *bmpFlabInfo) { m_bmpFlabInfo= bmpFlabInfo; };
  virtual void SetX(int iPosX) { m_bmpFlabInfo->SetX(iPosX); };
  virtual void SetY(int iPosY) { m_bmpFlabInfo->SetY(iPosY); }; 
  virtual bool StylusDown(int xPos, int yPos);
  virtual void Draw() { m_bmpFlabInfo->Blit(); };
private:
  CFBitmap *m_bmpFlabInfo;
};

#endif // !defined(AFX_FINFOPANEL_H__E36411F4_A36C_4851_A550_24997CF8209C__INCLUDED_)
