#include "FSystemImpl.h"
#include "FBitmapImpl.h"
#include "FFileImpl.h"

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
	SDL_Event evt;
  evt.type = SDL_QUIT;
	::SDL_PushEvent(&evt);
	return true;
}

//------------------------------------------------------------------------------
void CFSystemImpl::ShowError(const TCHAR* msg)
{
	::SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg, NULL);
}

//------------------------------------------------------------------------------
void CFSystemImpl::RenderDoubleBuffer(CFBitmapImpl& doubleBuffer)
{
	doubleBuffer.Blit();
}

//------------------------------------------------------------------------------
void CFSystemImpl::ForceRedraw()
{
	QueueEvent(FATE_EVENT_ID_REDRAW, 0, NULL);
}

//------------------------------------------------------------------------------
void CFSystemImpl::DrawFileIcon(CFBitmapImpl& bmp, const TCHAR* filePath, int x, int y, bool normal)
{
	if (CFFileImpl::IsDirectory(filePath))
	{
		bmp.SetBackgroundColor(RGB(255, 205, 0));
	}
	else
	{
		bmp.SetBackgroundColor(RGB(70, 130, 180));
	}
	bmp.DrawFilledRect(x, y, 16, 16);
}

//------------------------------------------------------------------------------
void CFSystemImpl::QueueEvent(int eventId, int componentId, void *pCustomData)
{
	SDL_Event evt;
  evt.type = EVENT_TYPE_FATE;
	FateEventData* data = new FateEventData();
	data->eventId = eventId;
	data->componentId = componentId;
	data->pCustomData = pCustomData;
	evt.user.data1 = data;
	::SDL_PushEvent(&evt);
}

struct TimerData
{
	CFSystemImpl* pSystem;
	unsigned long id;
};
Uint32 TimerCallBack(Uint32 interval, void* param)
{
	TimerData* timerData = reinterpret_cast<TimerData*>(param);
	timerData->pSystem->QueueEvent(FATE_EVENT_ID_TIMER, timerData->id, NULL);
	return interval;
}

//------------------------------------------------------------------------------
void CFSystemImpl::AddTimer(unsigned long id, int interval)
{
	TimerData *data = new TimerData();
	data->id = id;
	::SDL_AddTimer(interval, TimerCallBack, data);
}

//------------------------------------------------------------------------------
void CFSystemImpl::Sleep(int milliSeconds)
{
  ::usleep(milliSeconds * 1000);
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
		strcat(pszAppPath, "/");
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
