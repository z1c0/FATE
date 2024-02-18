#include "FSystemImpl.h"
#include "FBitmapImpl.h"


//------------------------------------------------------------------------------
CFSystemImpl::CFSystemImpl(SDL_Window* pWindow, int width, int height) : 
	m_pWindow(pWindow), m_width(width), m_height(height)
{
}

//------------------------------------------------------------------------------
CFBitmapImpl* CFSystemImpl::CreateDoubleBuffer()
{
	SDL_Renderer* pRenderer = ::SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	assert(pRenderer);
	CFBitmapImpl* doubleBuffer = new CFBitmapImpl();
	doubleBuffer->m_pRenderer = pRenderer;
	doubleBuffer->Create(GetWidth(), GetHeight());
	doubleBuffer->SolidFill(COLOR_CORNFLOWER_BLUE);
	return doubleBuffer;
}

//------------------------------------------------------------------------------
bool CFSystemImpl::ShutDownSystem()
{
	assert(false);
	return false;
}

//------------------------------------------------------------------------------
void CFSystemImpl::ShowError(const TCHAR* msg)
{
	assert(false);
}

//------------------------------------------------------------------------------
void CFSystemImpl::RenderDoubleBuffer(CFBitmapImpl& doubleBuffer)
{
	doubleBuffer.Blit();
}

//------------------------------------------------------------------------------
void CFSystemImpl::ForceRedraw()
{
	assert(false);
}

//------------------------------------------------------------------------------
void CFSystemImpl::DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal)
{
	assert(false);
}

//------------------------------------------------------------------------------
void CFSystemImpl::QueueEvent(int iEventID, int iComponentID, void *pCustomData)
{
	assert(false);
}

//------------------------------------------------------------------------------
void CFSystemImpl::AddTimer(unsigned long id, int interval)
{
	assert(false);
}


//------------------------------------------------------------------------------
/* static */ void CFSystemImpl::GetPathToApplication(TCHAR *pszAppPath)
{
	char buffer[MAX_PATH];
	ssize_t length = readlink("/proc/self/exe", buffer, MAX_PATH);
	if (length != 1)
	{
		buffer[length] = 0;
		strcpy(pszAppPath, dirname(buffer));
	}
	else
	{
		pszAppPath[0] = 0;
		assert(false);
	}
}

//------------------------------------------------------------------------------
/* static */ unsigned int CFSystemImpl::GetTicks()
{
	assert(false);
	return 0;
}

//------------------------------------------------------------------------------
/* static */ int CFSystemImpl::GetRandomNumber(int max)
{
	assert(false);
	return 0;
}
