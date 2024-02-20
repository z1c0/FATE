#include "../../base/FateTypeDefs.h"
#include "../../base/FateApp.h"
#include "../../comm/FUDPSocket.h"
#include "FSystemImpl.h"

#include <SDL2/SDL.h>

//--------------------------------------------------------------------------------
/// Global variables
CFateApp    *g_pApp     = NULL;
CFUDPSocket *g_sockDebug= NULL;
CFInetAddr  *g_addrDebug= NULL;

SDL_Window* g_pWindow = NULL;

//--------------------------------------------------------------------------------
// This method has to be implemented by the actual application.
// An instance of a class derived from CFateApp is expected to be returned.
extern CFateApp* CreateFateApp();

//--------------------------------------------------------------------------------
void __InitRemoteConsole(LPCTSTR pszHost, int iPort)
{
	SAFE_DELETE(g_sockDebug);
	SAFE_DELETE(g_addrDebug);
	g_sockDebug= new CFUDPSocket();
	g_sockDebug->Create();
	g_addrDebug= new CFInetAddr(pszHost, iPort);
}

//--------------------------------------------------------------------------------
void __RemoteConsole(LPCTSTR pszFormat, ...)
{
	char szBuffer[256];
	va_list pArgs;
	
	// initialized?
	if (g_sockDebug)
	{
		va_start(pArgs, pszFormat);
		sprintf(szBuffer, pszFormat, pArgs);
		va_end(pArgs);
		strcat(szBuffer, "\r\n");
		g_sockDebug->Send(szBuffer, strlen(szBuffer) + 1, g_addrDebug);
	}
}



//------------------------------------------------------------------------------
static bool StartApplication()
{
	::SDL_Init(SDL_INIT_VIDEO);
	::TTF_Init();

	g_pApp = CreateFateApp();
	g_pWindow = ::SDL_CreateWindow("FateApp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_pApp->GetWidth(), g_pApp->GetHeight(), SDL_WINDOW_SHOWN);
	if (!g_pWindow)
	{
		return false;
	}
	g_pApp->SetSystem(new CFSystem(new CFSystemImpl(g_pWindow, g_pApp->GetWidth(), g_pApp->GetHeight())));
	g_pApp->Init();
	g_pApp->InitFateApp();

	// Force redraw at startup.
	g_pApp->GetSystem()->ForceRedraw();

	return true;
}

//------------------------------------------------------------------------------
static void StopApplication()
{
	::SDL_DestroyWindow(g_pWindow);
	::SDL_Quit();
}

//------------------------------------------------------------------------------
void EventLoop()
{
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		if (::SDL_PollEvent(&e) != 0)
		{
			switch(e.type)
			{				
				case SDL_QUIT:
					quit = true;
					break;

				case EVENT_TYPE_FATE:
				{
					FateEventData* data = static_cast<FateEventData*>(e.user.data1);
					switch (data->eventId)
					{
						case FATE_EVENT_ID_REDRAW:
							g_pApp->Draw();
							// tell registered components to draw themselves
							g_pApp->DrawNotify();
							break;

						case FATE_EVENT_ID_BUTTONPRESS:
							if (!g_pApp->ButtonPressed(data->componentId))
							{
								g_pApp->ButtonPressNotify(data->componentId);
							}
							break;

						case FATE_EVENT_ID_BUTTONRELEASE:
							if (!g_pApp->ButtonReleased(data->componentId))
							{
								g_pApp->ButtonReleaseNotify(data->componentId);
							}
							break;

						case FATE_EVENT_ID_ITEMLISTSELECT:
							if (!g_pApp->ItemListSelected(data->componentId, reinterpret_cast<ITEMLISTENTRY*>(data->pCustomData)))
							{
								g_pApp->ItemListSelectNotify(data->componentId, reinterpret_cast<ITEMLISTENTRY*>(data->pCustomData));
							}
							break;

						case FATE_EVENT_ID_DROPLISTEXPAND:
							if (!g_pApp->DropListExpanded(data->componentId))
							{
								g_pApp->DropListExpandNotify(data->componentId);
							}
							break;

						case FATE_EVENT_ID_DROPLISTSELECT:
							if (!g_pApp->DropListSelected(data->componentId, reinterpret_cast<ITEMLISTENTRY*>(data->pCustomData)))
							{
								g_pApp->DropListSelectNotify(data->componentId, reinterpret_cast<ITEMLISTENTRY*>(data->pCustomData));
							}
							break;

						case FATE_EVENT_ID_DROPLISTCOLLAPSE:
							if (!g_pApp->DropListCollapsed(data->componentId))
							{
								g_pApp->DropListCollapseNotify(data->componentId);
							}
							break;

						case FATE_EVENT_ID_MENUSELECTION:
						{
							long id = reinterpret_cast<long>(data->pCustomData);
							g_pApp->MenuItemSelected(data->componentId, id);
							{
								g_pApp->MenuItemSelectNotify(data->componentId, id);
							}
							break;
						}

						default:
							printf("EVENT: %06X\n", data->eventId);
							assert(false);
							break;
					}
					
					delete data;
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						if (!g_pApp->StylusDown(mouseX, mouseY))
						{
							g_pApp->StylusDownNotify(mouseX, mouseY);
						}
					}
					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						if (!g_pApp->StylusUp(mouseX, mouseY))
						{
							g_pApp->StylusUpNotify(mouseX, mouseY);
						}
					}
					break;
				}
			}
		}
		/*
		// FATE loop
		if (g_pApp->IsListening())
		{
			g_pApp->CheckServers();
		}
		*/
		if (g_pApp->IsFateLoopEnabled())
		{
			g_pApp->Idle();
			g_pApp->DrawDoubleBuffer();
		}

		SDL_Delay(10);
	}
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	// basic initialization 
	if (!StartApplication())
	{
		printf("StartApplication failed\n");
		return false;
	}
	
	// main loop
	EventLoop();

	if (g_pApp)
	{
		int nrOfInstances = IFateComponent::GetNumberOfInstances();
		if (nrOfInstances != 0)
		{
			char szTemp[100];
			sprintf(szTemp, "%d instances of IFateComponent are left allocated!", nrOfInstances);
			g_pApp->Message(szTemp);
		}

		delete(g_pApp);
		SAFE_DELETE(g_sockDebug);
		SAFE_DELETE(g_addrDebug);
	}

	StopApplication();

	return 0;
}