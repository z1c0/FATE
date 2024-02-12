#pragma once

#include "../base/FateTypeDefs.h"

//TODO: alot

class CFBitmap  
{
public:
	
  CFBitmap();
  CFBitmap(CFBitmap *pBmp);
  ~CFBitmap();
  
  void Init();
  
  void CleanUp();
  
  bool Create(int iWidth, int iHeight);

  void SetDestBitmap(const CFBitmap& bmp);
  
  bool SolidFill(const COLORREF colFill);
	
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
  
  bool Blit();
  bool Blit(int iWidth, int iHeight);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int iWidth, int iHeight);
  bool TransBlit(COLORREF colTrans);

  bool SaveUnder() { return(false); };
  bool RestoreUnder() { return(false); };

  bool SetBits(unsigned char *pBits, int iSize);
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
  static bool CalcRectForText(const TCHAR *pszText, RECT& rect);

  COLORREF SetBackgroundColor(COLORREF col);
  COLORREF SetColor(COLORREF col);
  COLORREF SetTextColor(COLORREF col);

  bool DrawText(const TCHAR *pszText, RECT& rect);
  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFilledRect(RECT *pRect);

  bool DrawFrameRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFrameRect(RECT *pRect);

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

