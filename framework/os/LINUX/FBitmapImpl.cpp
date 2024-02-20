#include "FBitmapImpl.h"
#include "FFileImpl.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//------------------------------------------------------------------------------
CFBitmapImpl::CFBitmapImpl() :
	m_pRenderer(NULL),
	m_pTexture(NULL),
	m_pTargetTexture(NULL),
	m_pSaveTexture(NULL),
	m_posX(0),
	m_posY(0),
	m_width(0),
	m_height(0)
{
}

//------------------------------------------------------------------------------
CFBitmapImpl::~CFBitmapImpl()
{
	::SDL_DestroyTexture(m_pTexture);
	if (m_pSaveTexture)
	{
		::SDL_DestroyTexture(m_pSaveTexture);
	}
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Create(int width, int height)
{
	assert(width);
	assert(height);
	if (!m_pRenderer)
	{
		assert(false);
		return false;
	}
	m_width = width;
	m_height = height;
	m_pTexture = ::SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
	assert(m_pTexture);
	return true;
}

//------------------------------------------------------------------------------
void CFBitmapImpl::Trace()
{
	printf("CFBitmapImpl { %d, %d, %d, %d }\n", m_posX, m_posY, m_width, m_height);
}

//------------------------------------------------------------------------------
void CFBitmapImpl::SetDestBitmap(const CFBitmapImpl& destination)
{
	m_pRenderer = destination.m_pRenderer;
	m_pTargetTexture = destination.m_pTexture;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(const TCHAR* fileName)
{
	assert(m_pRenderer);
	assert(CFFileImpl::Exists(fileName));
	SDL_Surface* pSurface = ::SDL_LoadBMP(fileName);
	assert(pSurface);
	m_pTexture = ::SDL_CreateTextureFromSurface(m_pRenderer, pSurface);
	assert(m_pTexture);
	::SDL_FreeSurface(pSurface);
	::SDL_QueryTexture(m_pTexture, NULL, NULL, &m_width, &m_height);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(int resourceId, const TCHAR* fallbackFilePath)
{
	if (!fallbackFilePath)
	{
		printf("load bmp %d - no fallback path\n", resourceId);
		return false;
	}
	return Load(fallbackFilePath);
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(const char *data, unsigned long size)
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::SaveToFile(const TCHAR *pszFileName) const
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::IsValid() const
{
	return m_pTexture != NULL && m_width > 0 && m_height > 0;
}

//------------------------------------------------------------------------------
char* CFBitmapImpl::GetBits()
{
	assert(false);
	return NULL;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Blit()
{
	if (m_pTargetTexture)
	{
		::SDL_SetRenderTarget(m_pRenderer, m_pTargetTexture);
		SDL_Rect destRect = { m_posX, m_posY, m_width, m_height };
		::SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &destRect);
		::SDL_SetRenderTarget(m_pRenderer, NULL);
	}
	else
	{
		::SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, NULL);
		::SDL_RenderPresent(m_pRenderer);
	}
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Blit(int srcx, int srcy, int srcw, int srch, int destx, int desty)
{
	assert(false);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::StretchBlit(int width, int height)
{
	assert(false);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::ClipBlit(int width, int height)
{
	assert(false);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::TransBlit(COLORREF colTrans)
{
	Blit();
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::SaveUnder(const CFBitmapImpl& bmp)
{
	if (!m_pSaveTexture)
	{
		m_pSaveTexture = ::SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
	}
	assert(m_pTargetTexture);
	::SDL_SetRenderTarget(m_pRenderer, m_pSaveTexture);
	SDL_Rect sourceRect = { m_posX, m_posY, m_width, m_height };
	SDL_Rect destRect = { 0, 0, m_width, m_height };
	::SDL_RenderCopy(m_pRenderer, m_pTargetTexture, &sourceRect, &destRect);
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::RestoreUnder(const CFBitmapImpl& bmp)
{
	assert(m_pSaveTexture);
	assert(m_pTargetTexture);
	::SDL_SetRenderTarget(m_pRenderer, m_pTargetTexture);
	SDL_Rect sourceRect = { 0, 0, m_width, m_height };
	SDL_Rect destRect = { m_posX, m_posY, m_width, m_height };
	::SDL_RenderCopy(m_pRenderer, m_pSaveTexture, &sourceRect, &destRect);
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetTextColor(COLORREF col)
{
	COLORREF old = m_colText;
	m_colText = col;
	return old;
}


//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetColor(COLORREF col)
{
	COLORREF old = m_col;
	m_col = col;
	return old;
}


//------------------------------------------------------------------------------
COLORREF CFBitmapImpl::SetBackgroundColor(COLORREF col)
{
	COLORREF old = m_colBack;
	m_colBack = col;
	return old;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::SolidFill(const COLORREF col)
{
	::SDL_SetRenderTarget(m_pRenderer, m_pTexture);
	::SDL_SetRenderDrawColor(m_pRenderer, col.r, col.g, col.b, 255);
	::SDL_RenderClear(m_pRenderer);
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawFilledRect(int left, int top, int width, int height)
{
	::SDL_SetRenderTarget(m_pRenderer, m_pTexture);

	::SDL_SetRenderDrawColor(m_pRenderer, m_col.r, m_col.g, m_col.b, 255);
	SDL_Rect outerRect = { left, top, width, height };
	::SDL_RenderDrawRect(m_pRenderer, &outerRect);

	::SDL_SetRenderDrawColor(m_pRenderer, m_colBack.r, m_colBack.g, m_colBack.b, 255);
	SDL_Rect innerRect = { left + 1, top + 1, width - 2, height -2 };
	::SDL_RenderFillRect(m_pRenderer, &innerRect);

	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawPolygon(POINT *points, int count)
{
	::SDL_SetRenderTarget(m_pRenderer, m_pTexture);
	::SDL_SetRenderDrawColor(m_pRenderer, m_col.r, m_col.g, m_col.b, 255);
	for (int i = 0; i < count - 1; i++)
	{
		::SDL_RenderDrawLine(m_pRenderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
	}
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
static TTF_Font* GetFont()
{
	static TTF_Font* font = ::TTF_OpenFont("./font.ttf", 14);
	assert(font);
	return font;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawText(const TCHAR *text, RECT& rect)
{
	SDL_Color textColor = { m_colText.r, m_colText.g, m_colText.b, 255 };
	SDL_Surface* pSurface = ::TTF_RenderText_Solid(GetFont(), text, textColor);
	SDL_Texture* pTextTexture = ::SDL_CreateTextureFromSurface(m_pRenderer, pSurface);
	::SDL_FreeSurface(pSurface);
	::SDL_SetRenderTarget(m_pRenderer, m_pTexture);
	SDL_Rect sdlRect = { rect.left, rect.top, pSurface->w, pSurface->h };
	::SDL_RenderCopy(m_pRenderer, pTextTexture, NULL, &sdlRect);
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
/* static */ bool CFBitmapImpl::CalcRectForText(const TCHAR* text, RECT& rect)
{
	int width;
	int height;
	::TTF_SizeText(GetFont(), text, &width, &height);
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;
	return true;
}