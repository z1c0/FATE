#pragma once

#include "FateTypeDefs.h"

class CFBitmapImpl;

class CFBitmap
{  
  friend class CFSystem;
public:
	CFBitmap();
	CFBitmap(CFBitmapImpl* pImpl);
	CFBitmap(const CFBitmap& bmp);
	~CFBitmap();
  
  bool IsValid() const;
  
  void CleanUp();
  
  bool Create(int width, int height);
  
  void SetDestBitmap(const CFBitmap& bmp);
  
  bool SolidFill(const COLORREF colFill);
	
  bool Load(const TCHAR* pszFileName);
  bool Load(int wesourceId, const TCHAR* fallbackFilePath = NULL);
	bool Load(const char *data, unsigned long size);
  
  int GetX() const;
  void SetX(int posX);
  int GetY() const;
  void SetY(int posY);
  int GetWidth() const;
  int GetHeight() const;
  int GetRight() const { return GetX() + GetWidth(); }
  int GetBottom() const { return GetY() + GetHeight(); }
  
  bool Blit();
  bool Blit(int iWidth, int iHeight);
  bool Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int width, int height);
  bool TransBlit(COLORREF colTrans);
  
  bool CreateSaveBitmap(const CFBitmap& pBmp);
  bool SaveUnder(const CFBitmap& pBmp);
  bool SaveUnder() { return SaveUnder(*this); };

  bool RestoreUnder(const CFBitmap& pBmp);
  bool RestoreUnder() { return RestoreUnder(*this); };

  bool SetBits(unsigned char *pBits, int iSize);
  char* GetBits();
  
  bool PointInside(int iPosX, int iPosY) const
  { 
    return iPosX >= GetX() &&
      iPosX <= GetX() + GetWidth() && 
      iPosY >= GetY() &&
      iPosY <= GetY() + GetHeight();
  };
  CFBitmap &operator=(const CFBitmap &bmp);
  
  bool SaveToFile(const TCHAR *pszFileName) const;

  COLORREF SetTextColor(COLORREF col);
  COLORREF SetColor(COLORREF col);
  COLORREF SetBackgroundColor(COLORREF col);

  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawFilledRect(const RECT& rect);
  bool DrawPolygon(POINT *points, int count);
  bool DrawText(const TCHAR *pszText, RECT& rect);

  static bool CalcRectForText(const TCHAR *pszText, RECT& rect);
private:
  CFBitmapImpl* m_pImpl;
};
