#pragma once

#include "../../base/FateTypeDefs.h"

class CFBitmapImpl
{ 
  friend class CFSystemImpl;
public:
	CFBitmapImpl();
	~CFBitmapImpl();

  CFBitmapImpl& operator=(const CFBitmapImpl &bmp);
  
  bool IsValid() const;  
  void CleanUp();  
  bool Create(int iWidth, int iHeight);  
  void SetDestBitmap(const CFBitmapImpl& bmp);  
  
  bool SolidFill(const COLORREF colFill);
	
  bool Load(const TCHAR* pszFileName);
  bool Load(int resourceId, const TCHAR* fallback);
	bool Load(const char *pData, unsigned long dwSize);
  
  int GetX() { return(m_iPosX); };
  void SetX(int iPosX) { m_iPosX= iPosX; };
  
  int GetY() { return(m_iPosY); };
  void SetY(int iPosY) { m_iPosY= iPosY; };
  
  int GetWidth() { return m_iWidth; }
  int GetHeight() { return m_iHeight; }
  int GetRight() { return(m_iPosX + m_iWidth); };
  int GetBottom() { return(m_iPosY + m_iHeight); };
  
  bool Blit();
  bool Blit(int iWidth, int iHeight);
  bool Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int iWidth, int iHeight);
  bool TransBlit(COLORREF colTrans);
  
  bool CreateSaveBitmap(const CFBitmapImpl& pBmp);
  bool SaveUnder(const CFBitmapImpl& pBmp);
  bool SaveUnder() { return SaveUnder(*this); };

  bool RestoreUnder(const CFBitmapImpl& pBmp);
  bool RestoreUnder() { return RestoreUnder(*this); };

  bool SetBits(unsigned char *pBits, int iSize);
  char* GetBits() { return(m_pBits); };
  
  bool SaveToFile(const TCHAR *pszFileName);

  COLORREF SetTextColor(COLORREF col);
  COLORREF SetColor(COLORREF col);
  COLORREF SetBackgroundColor(COLORREF col);

  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFilledRect(const RECT& rect);

  bool DrawPolygon(POINT *points, int count);

  bool DrawText(const TCHAR *pszText, RECT& rect);

  //------------------------------------------------------------------------------
  /// Drawing operations on Bitmap
  static bool CalcRectForText(const TCHAR *pszText, RECT& rect);

private:
  void Init();

  HBITMAP m_hOldBmp;
  HBITMAP m_hOldSaveBmp;
	
  char *m_pBits;
	
  int m_iBitCount;
	int m_iPlanes;
	
  ULONG m_ulPaletteSize;
  ULONG m_ulDataSize;
  
  HDC m_hSourceDC;
  HDC m_hDestDC;
  HDC m_hSaveDC;
  
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;

  COLORREF m_colBackground;
  COLORREF m_colForeground;

  HPEN m_hPen;
  HBRUSH m_hBrush;
  HPEN m_hPenOld;
  HBRUSH m_hBrushOld;
};
