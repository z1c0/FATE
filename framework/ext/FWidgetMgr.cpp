#include "FWidgetMgr.h"
#include "IFRemoteWidget.h"
#include "../comm/FServer.h"
#include "FRemoteButton.h"
#include "../base/FateApp.h"
#include "../ext/gui_protocol.h"


//--------------------------------------------------------------------------------
CFWidgetMgr::CFWidgetMgr()
{
  m_iPort= -1;
}

//--------------------------------------------------------------------------------
CFWidgetMgr::~CFWidgetMgr()
{
  ClearWidgets();
}

//--------------------------------------------------------------------------------
bool CFWidgetMgr::StartListening(int iPort)
{
  CFateApp *pApp= CFateApp::GetApp();
  if (pApp) {
    m_iPort= iPort;
    pApp->AddServer(m_iPort);
    return(true);
  }
  return(false);
}

//--------------------------------------------------------------------------------
void CFWidgetMgr::AddWidget(IFRemoteWidget *pWidget)
{
  m_WidgetList.Append(pWidget);
}

//--------------------------------------------------------------------------------
bool CFWidgetMgr::RemoveWidget(DWORD dwID)
{
  for (int i=0; i<m_WidgetList.GetSize(); i++) {
    if (m_WidgetList[i]->GetWidgetID() == dwID) {
      delete(m_WidgetList[i]);
      m_WidgetList.Delete(i);
      return(true);
    }
  }
  return(false);
}

//--------------------------------------------------------------------------------
void CFWidgetMgr::ClearWidgets()
{
  for (int i=0; i<m_WidgetList.GetSize(); i++) delete(m_WidgetList[i]);
  m_WidgetList.Clear();
}

//--------------------------------------------------------------------------------
/// Receive necessary information.
bool CFWidgetMgr::ClientConnect(CFServer *pServer)
{
  if (pServer->GetListenPort() == m_iPort) {
    CFSocket sock;
    IFRemoteWidget *pWidget;
    
    if (pServer->Accept(sock)) {    
      GUI_PACKET pack;
      char *pData;
      
      // receive command
      sock.Receive((char*)&pack, sizeof(GUI_PACKET));

      if (pack.uiCmd == WIDGET_ADD) {
        // what kind of widget has to be created
        switch(pack.uiKind) {
          case WIDGET_BUTTON:
            pData= (char*)malloc(pack.uiLen);
            // receive caption of button
            sock.Receive(pData, pack.uiLen);

#ifdef _WIN32_WCE
            wchar_t *pCaption;

            pCaption= (wchar_t*)malloc(pack.uiLen);
            MultiByteToWideChar(CP_ACP, 0, pData, -1, pCaption, pack.uiLen);
            pWidget= new CFRemoteButton(pCaption);
            delete(pCaption);
#else
            pWidget= new CFRemoteButton(pData);
#endif
            ((CFRemoteButton*)pWidget)->SetVisible(true);
            delete(pData);
            break;

          case WIDGET_SWITCH:       
          case WIDGET_SLIDER:       
          case WIDGET_MENU:       
          case WIDGET_ITEMLIST:       
          default:
            pWidget= NULL;
        }

        // create
        if (pWidget) {
          pWidget->SetWidgetID(pack.uiID);
          AddWidget(pWidget);
        }
      
      } else if (pack.uiCmd == WIDGET_REMOVE) {
        RemoveWidget(pack.uiID);
      } 
    }
    return(true);
  }
  return(false);
}

