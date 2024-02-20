#pragma once

#include "../../base/FateTypeDefs.h"

#include <SDL2/SDL.h>

class CFBitmapImpl
{
  friend class CFSystemImpl;
public:
  CFBitmapImpl();
  ~CFBitmapImpl();

	bool Create(int width, int height);
  bool IsValid() const;
  void SetDestBitmap(const CFBitmapImpl& bmp);
	bool Load(const TCHAR* pszFileName);
	bool Load(int resourceId, const TCHAR* fallbackFilePath);
	bool Load(const char *data, unsigned long size);
  bool SaveToFile(const TCHAR *pszFileName) const;
  char* GetBits();
  int GetX() const { return m_posX; }
  void SetX(int posX) { m_posX = posX; }
  int GetY() const { return m_posY; }
  void SetY(int posY) { m_posY = posY; }
  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }
  bool Blit();
  bool Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty);
  bool ClipBlit(int iWidth, int iHeight);
  bool StretchBlit(int width, int height);
  bool TransBlit(COLORREF colTrans);
  bool SaveUnder(const CFBitmapImpl& bmp);
  bool RestoreUnder(const CFBitmapImpl& bmp);
  COLORREF SetTextColor(COLORREF col);
  COLORREF SetColor(COLORREF col);
  COLORREF SetBackgroundColor(COLORREF col);
  bool SolidFill(const COLORREF colFill);
  bool DrawFilledRect(int iLeft, int iTop, int iWidth, int iHeight);
  bool DrawPolygon(POINT *points, int count);
  bool DrawText(const TCHAR *pszText, RECT& rect);

  static bool CalcRectForText(const TCHAR *pszText, RECT& rect);

private:
  void Trace();
  
  SDL_Renderer* m_pRenderer;
  SDL_Texture* m_pTexture;
  SDL_Texture* m_pTargetTexture;
  SDL_Texture* m_pSaveTexture;
  int m_posX;
  int m_posY;
  int m_width;
  int m_height;
  COLORREF m_col;
  COLORREF m_colBack;
  COLORREF m_colText;
};