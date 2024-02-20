#pragma once

#include "../../base/FateTypeDefs.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class CFBitmapImpl;

#define EVENT_TYPE_FATE SDL_USEREVENT + 1

struct FateEventData
{
	int eventId;
	int componentId;
	void *pCustomData;
};


class CFSystemImpl
{
public:
	CFSystemImpl(SDL_Window* pWindow, int width, int height);

	CFBitmapImpl* CreateDoubleBuffer();
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	bool ShutDownSystem();
	void ShowError(const TCHAR* msg);
	void RenderDoubleBuffer(CFBitmapImpl& doubleBuffer);
	void ForceRedraw();
	void DrawFileIcon(CFBitmapImpl& bmp, const TCHAR *pszFilePath, int x, int y, bool normal);
	void QueueEvent(int iEventID, int iComponentID, void *pCustomData);
	void AddTimer(unsigned long id, int interval); 
	void Sleep(int milliSeconds);
	bool EnableSuspend(bool suspend) { return true; }

	static int GetDefaultWidth() { return 240; }
	static int GetDefaultHeight() { return 320; }
	static const TCHAR* GetDirectorySeparator() { return "/"; }
	static void GetPathToApplication(TCHAR *pszAppPath);
	static unsigned int GetTicks();
	static int GetRandomNumber(int max);

private:
	SDL_Window* m_pWindow;
	int m_width;
	int m_height;
};