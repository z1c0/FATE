#pragma once

#include "../base/FateTypeDefs.h"

class CFBitmapImpl
{
public:	
  CFBitmapImpl();
  ~CFBitmapImpl();

  void CleanUp();
  
  bool Create(int iWidth, int iHeight);
  bool IsValid() const;

  void SetDestBitmap(const CFBitmapImpl& bmp);
  
  bool SolidFill(const COLORREF colFill);
	
  bool Load(const TCHAR* pszFileName);
  bool Load(unsigned long resourceId, const char* fallbackPath);
  bool Load(const char *pszData, unsigned long ulSize);
  
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
  bool Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int iWidth, int iHeight);
  bool TransBlit(COLORREF colTrans);

  bool SaveUnder(const CFBitmapImpl& bmp);
  bool RestoreUnder(const CFBitmapImpl& bmp);

  bool SetBits(unsigned char *pBits, int iSize);
  char* GetBits() { return(m_pBits); };
  
  bool SaveToFile(const TCHAR *pszFileName);

  COLORREF SetBackgroundColor(COLORREF col);
  COLORREF SetColor(COLORREF col);
  COLORREF SetTextColor(COLORREF col);

  bool DrawText(const TCHAR *pszText, RECT& rect);
  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFilledRect(const RECT& rect);
  bool DrawPolygon(POINT *points, int count);
  bool DrawFrameRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFrameRect(RECT *pRect);

  static bool CalcRectForText(const TCHAR *pszText, RECT& rect);

private:
  void Init();
  
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

