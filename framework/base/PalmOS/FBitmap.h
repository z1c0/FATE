#ifndef __FBITMAPPALMOS__H__
#define __FBITMAPPALMOS__H__

#include <PalmOS.h>
#include "FateTypeDefs.h"

//TODO: alot

//------------------------------------------------------------------------------
class CFBitmap  
{
public:
	
  CFBitmap() FATE_SECTION;
	CFBitmap(CFBitmap *pBmp) FATE_SECTION;
	~CFBitmap() FATE_SECTION;
  
  void Init() FATE_SECTION;
  
  void CleanUp() FATE_SECTION;
  
  bool Create(int iWidth, int iHeight) FATE_SECTION;

  void SetDestBitmap(CFBitmap *pBmp);
  /*
  HDC GetSourceDC() { return(m_hSourceDC); };
  HDC GetDestDC() { return(m_hDestDC); };
  */
  
  bool SolidFill(const COLORREF colFill) FATE_SECTION;
	
  bool Load(TCHAR* pszFileName);
  bool Load(unsigned long ulResourceID);
	bool Load(char *pszData, unsigned long ulSize);
  
  int GetX() { return(m_iPosX); };
  void SetX(int iPosX) { m_iPosX= iPosX; };
  
  int GetY() { return(m_iPosY); };
  void SetY(int iPosY) { m_iPosY= iPosY; };
  
  int GetWidth() { return(m_iWidth); };
  int GetHeight() { return(m_iHeight); };
  int GetRight() { return(m_iPosX + m_iWidth); };
  int GetBottom() { return(m_iPosY + m_iHeight); };
  
  bool Blit() FATE_SECTION;
  bool Blit(int iWidth, int iHeight);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int iWidth, int iHeight);
  bool TransBlit(COLORREF colTrans);

  /*
  bool CreateSaveDC(HDC hdc= NULL);
  bool SaveUnder(HDC hdc);
  bool RestoreUnder(HDC hdc);
  */

  bool SaveUnder() { return(false); };
  bool RestoreUnder() { return(false); };

  bool SetBits(unsigned char *pBits, int iSize) FATE_SECTION;
  char* GetBits() { return(m_pBits); };
  
  bool PointInside(int iPosX, int iPosY)
  { 
    return(((iPosX >= m_iPosX)&&(iPosX <= m_iPosX + m_iWidth)&&
           (iPosY >= m_iPosY)&&(iPosY <= m_iPosY + m_iHeight)));
  };
  CFBitmap &operator=(const CFBitmap &bmp);
  CFBitmap *operator=(CFBitmap *pBmp);

  bool SaveToFile(const TCHAR *pszFileName);


  //------------------------------------------------------------------------------
  /// Drawing operations on Bitmap
  static bool CalcRectForText(TCHAR *pszText, RECT *pRect);

  COLORREF SetBackgroundColor(COLORREF col) FATE_SECTION;
  COLORREF SetColor(COLORREF col) FATE_SECTION;
  COLORREF SetTextColor(COLORREF col) FATE_SECTION;

  bool DrawText(TCHAR *pszText, RECT *pRect) FATE_SECTION;
  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight) FATE_SECTION;
  bool DrawFilledRect(RECT *pRect) FATE_SECTION;

  bool DrawFrameRect(int iLeft, int iTop, int iWidth, int iHeight) FATE_SECTION;
  bool DrawFrameRect(RECT *pRect) FATE_SECTION;

private:
  
  WinHandle m_hWinSrc;
  WinHandle m_hWinDst;

  BitmapType *m_pBmp;
  
  char *m_pBits;
	
  int m_iBitCount;
	int m_iPlanes;
	
  unsigned long m_ulDataSize;
  
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;
  
  COLORREF m_colBackground;
  COLORREF m_colForeground;
  COLORREF m_colText;

  int m_iBorderWith;
};

#endif //  __FBITMAPPALMOS__H__
