#include "FBitmapImpl.h"

#include <SDL2/SDL.h>

//------------------------------------------------------------------------------
CFBitmapImpl::CFBitmapImpl() :
	m_pRenderer(NULL),
	m_pTexture(NULL),
	m_pTargetTexture(NULL),
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
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Create(int width, int height)
{
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
void CFBitmapImpl::SetDestBitmap(const CFBitmapImpl& destination)
{
	m_pRenderer = destination.m_pRenderer;
	m_pTargetTexture = destination.m_pTexture;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(const TCHAR* pszFileName)
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(int wResourceID)
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::Load(char *pszData, unsigned long dwSize)
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
	assert(false);
	return false;
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
		if (::SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, &destRect) != 0)
		{
			printf("%s - %d\n", SDL_GetError(), m_width);
		}
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
	assert(false);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::SaveUnder(const CFBitmapImpl& bmp)
{
	assert(false);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::RestoreUnder(const CFBitmapImpl& bmp)
{
	assert(false);
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
	::SDL_SetRenderDrawColor(m_pRenderer, m_colBack.r, m_colBack.g, m_colBack.b, 255);
	SDL_Rect rect = { left, top, width, height };
	::SDL_RenderFillRect(m_pRenderer, &rect);
	::SDL_SetRenderTarget(m_pRenderer, NULL);
	return true;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawPolygon(POINT *points, int count)
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
bool CFBitmapImpl::DrawText(const TCHAR *pszText, RECT& rect)
{
	printf("FIX ME: DrawText\n");
	DrawFilledRect(10, 10, 50, 50);
	return true;
}

//------------------------------------------------------------------------------
/* static */ bool CFBitmapImpl::CalcRectForText(const TCHAR *pszText, RECT& rect)
{
	printf("FIX ME: CalcRectForText\n");
	rect.right = 20;
	rect.bottom = 20;
	return true;
}