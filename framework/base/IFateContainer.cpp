//#include "FateTypeDefs.h"
#include "IFateContainer.h"
#include "FateApp.h"
#include "../gui/FTextField.h"


//--------------------------------------------------------------------------------
IFateContainer::IFateContainer() : IFateComponent()
{
  m_pCompList    = new CFLinkedList<IFateComponent*>;
  m_pContList    = new CFLinkedList<IFateContainer*>;
  m_pBtnList     = new CFLinkedList<IFButtonListener*>;
  m_pMenuList    = new CFLinkedList<IFMenuListener*>;
  m_pItemListList= new CFLinkedList<IFItemListListener*>;
  m_pDropListList= new CFLinkedList<IFDropListListener*>;
  m_pSliderList  = new CFLinkedList<IFSliderListener*>;
  m_pCharList    = new CFLinkedList<IFCharListener*>;
  m_pConnList    = new CFLinkedList<IFConnectionListener*>;
}

//--------------------------------------------------------------------------------
IFateContainer::~IFateContainer()
{
  // clear component-datastructures
  ClearComponents();
  
  delete(m_pCompList);
  delete(m_pContList);
  delete(m_pBtnList);
  delete(m_pMenuList);
  delete(m_pItemListList);
  delete(m_pDropListList);
  delete(m_pSliderList);
  delete(m_pCharList);
  delete(m_pConnList);
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events.
bool IFateContainer::Add(IFateComponent& comp)
{ 
  // set main initialization values
  if (comp.GetId() == 0xFFFFFFFF) {
    comp.SetId(m_app->GetNextID());
  }
  comp.SetParent(this);
  comp.SetAppObject(m_app);
  comp.SetSystem(m_pSystem);
  comp.SetDestBitmap(m_pBmp);

  if (!m_pCompList->Append(&comp)) {
    return false;
  }
  
  //autmatically register textfields as Char-Listeners
  if (!_tcscmp(comp.GetClassName(), TEXT("CFTextField"))) {
    AddCharListener((CFTextField&)comp);
  }  
  
  return true;
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events.
bool IFateContainer::Add(IFateContainer& cont)
{
  // set main initialization values
  cont.SetParent(this);
  cont.SetAppObject(m_app);
  cont.SetSystem(m_pSystem);
  cont.SetDestBitmap(m_pBmp);

  bool ret = m_pContList->Append(&cont);

  return ret;
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events triggered by FATE-buttons.
bool IFateContainer::AddButtonListener(IFButtonListener *pBtnLst)
{ 
  return(m_pBtnList->Append(pBtnLst));
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events triggered by FATE-menus.
bool IFateContainer::AddMenuListener(IFMenuListener *pMenuLst)
{ 
  return(m_pMenuList->Append(pMenuLst));
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events triggered by FATE-itemlists.
bool IFateContainer::AddItemListListener(IFItemListListener *pItemListLst)
{ 
  return(m_pItemListList->Append(pItemListLst));
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events triggered by FATE-DropListes.
bool IFateContainer::AddDropListListener(IFDropListListener *pDropListLst)
{ 
  return(m_pDropListList->Append(pDropListLst));
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about events triggered by FATE-Sliders.
bool IFateContainer::AddSliderListener(IFSliderListener *pSliderLst)
{ 
  return(m_pSliderList->Append(pSliderLst));
}

//--------------------------------------------------------------------------------
/// Adds a new Fate-Component to this container. Component is automatically
/// informed about keyboard events.
bool IFateContainer::AddCharListener(IFCharListener& charlistener)
{ 
  return m_pCharList->Append(&charlistener);
}

//--------------------------------------------------------------------------------
/// Adds a new listener for incoming socket connections to this container. 
bool IFateContainer::AddConnectionListener(IFConnectionListener *pConnLst)
{ 
  return(m_pConnList->Append(pConnLst));
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_LBUTTONDOWN message was posted.
/// All registered listeners are iterated through and their StylusDown-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// StylusDownNotify-function is called.
/// If a "Capture Component" is registered the event only gets passed to this 
/// component no matter if handled or not.
bool IFateContainer::StylusDownNotify(int xPos, int yPos)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;
  
   // system capture component registered?
  if (m_app->m_pSysCapt) {
    return(m_app->m_pSysCapt->StylusDown(xPos, yPos));
  }
  
  // capture component registered?
  if (m_app->GetCaptureComp()) {
    return(m_app->GetCaptureComp()->StylusDown(xPos, yPos));
  }

  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsEnabled()) {
      if (pCont->StylusDownNotify(xPos, yPos)) return(true);
      if (pCont->StylusDown(xPos, yPos)) return(true);
    }
    
    pContIt->Next();
  }

  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsEnabled()) {
      if (pComp->StylusDown(xPos, yPos)) return(true);
    }
    
    pCompIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_MOUSEMOVE message was posted.
/// All registered listeners are iterated through and their StylusMove-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// StylusMoveNotify-function is called.
/// If a "Capture Component" is registered the event only gets passed to this 
/// component no matter if handled or not.
bool IFateContainer::StylusMoveNotify(int xPos, int yPos)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;
  
   // system capture component registered?
  if (m_app->m_pSysCapt) {
    return(m_app->m_pSysCapt->StylusMove(xPos, yPos));
  }
  
  // capture component registered?
  if (m_app->GetCaptureComp()) {
    return(m_app->GetCaptureComp()->StylusMove(xPos, yPos));
  }

  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsEnabled()) {
      if (pCont->StylusMoveNotify(xPos, yPos)) return(true);
      if (pCont->StylusMove(xPos, yPos)) return(true);
    }
    
    pContIt->Next();
  }

  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsEnabled()) {
      if (pComp->StylusMove(xPos, yPos)) return(true);
    }
    
    pCompIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_LBUTTONUP message was posted.
/// All registered listeners are iterated through and their StylusUp-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// StylusUpNotify-function is called.
/// If a "Capture Component" is registered the event only gets passed to this 
/// component no matter if handled or not.
bool IFateContainer::StylusUpNotify(int xPos, int yPos)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;
  
   // system capture component registered?
  if (m_app->m_pSysCapt) {
    return(m_app->m_pSysCapt->StylusUp(xPos, yPos));
  }
  
  // capture component registered?
  if (m_app->GetCaptureComp()) {
    return(m_app->GetCaptureComp()->StylusUp(xPos, yPos));
  }

  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsEnabled()) {
      if (pCont->StylusUpNotify(xPos, yPos)) return(true);
      if (pCont->StylusUp(xPos, yPos)) return(true);
    }
    
    pContIt->Next();
  }

  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsEnabled()) {
      if (pComp->StylusUp(xPos, yPos)) return(true);
    }
    
    pCompIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_KEYDOWN message was posted.
/// All registered listeners are iterated through and their KeyDown-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// KeyDownNotify-function is called.
bool IFateContainer::KeyDownNotify(PdaKey key)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;
  
  // system capture component registered?
  if (m_app->m_pSysCapt) {
    return(m_app->m_pSysCapt->KeyDown(key));
  }

  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsEnabled()) {
      if (pCont->KeyDownNotify(key)) return(true);
      if (pCont->KeyDown(key)) return(true);
    }
    
    pContIt->Next();
  }

  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsEnabled()) {
      if (pComp->KeyDown(key)) return(true);
    }
    
    pCompIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_KEYUP message was posted.
/// All registered listeners are iterated through and their KeyUp-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// KeyUpNotify-function is called.
bool IFateContainer::KeyUpNotify(PdaKey key)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;

  // system capture component registered?
  if (m_app->m_pSysCapt) {
    return(m_app->m_pSysCapt->KeyUp(key));
  }
  
  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsEnabled()) {
      if (pCont->KeyUpNotify(key)) return(true);
      if (pCont->KeyUp(key)) return(true);
    }
    
    pContIt->Next();
  }

  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsEnabled()) {
      if (pComp->KeyUp(key)) return(true);
    }
    
    pCompIt->Next();
  }
 
  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_PAINT message was posted.
/// All registered listeners are iterated through and their Draw-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// DrawNotify-function is called.
void IFateContainer::DrawNotify()
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;
  
  // iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();
    if (pComp->IsVisible()) 
      pComp->Draw();

    pCompIt->Next();
  }

  // iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    if (pCont->IsVisible()) {
      pCont->Draw();
      pCont->DrawNotify();
    }
    
    pContIt->Next();
  }
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_BUTTONPRESS message was posted.
/// All registered listeners are iterated through and their ButtonPress-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// ButtonPressNotify-function is called.
bool IFateContainer::ButtonPressNotify(unsigned long ulBtnID)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFButtonListener*> *pBtnIt;
  IFateContainer *pCont;
  IFButtonListener *pBtnLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->ButtonPressNotify(ulBtnID)) return(true);
    if (pCont->ButtonPressed(ulBtnID)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pBtnIt= m_pBtnList->GetIterator();
  while(pBtnIt->HasMore()) {
    pBtnLst= pBtnIt->GetData();
    if (pBtnLst->ButtonPressed(ulBtnID)) return(true);

    pBtnIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_BUTTONRELEASE message was posted.
/// All registered listeners are iterated through and their ButtonRelease-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// ButtonReleaseNotify-function is called.
bool IFateContainer::ButtonReleaseNotify(unsigned long ulBtnID)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFButtonListener*> *pBtnIt;
  IFateContainer *pCont;
  IFButtonListener *pBtnLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->ButtonReleaseNotify(ulBtnID)) return(true);
    if (pCont->ButtonReleased(ulBtnID)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pBtnIt= m_pBtnList->GetIterator();
  while(pBtnIt->HasMore()) {
    pBtnLst= pBtnIt->GetData();
    if (pBtnLst->ButtonReleased(ulBtnID)) return(true);

    pBtnIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_MENUSELECTION message was posted.
/// All registered listeners are iterated through and their MenuSelect-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// MenuSelectNotify-function is called.
bool IFateContainer::MenuItemSelectNotify(unsigned long ulMenuID, unsigned int id)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFMenuListener*> *pMenuIt;
  IFateContainer *pCont;
  IFMenuListener *pMenuLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore())
  {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->MenuItemSelectNotify(ulMenuID, id))
    {
      return true;
    }
    if (pCont->MenuItemSelected(ulMenuID, id))
    {
      return true;
    }    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pMenuIt= m_pMenuList->GetIterator();
  while(pMenuIt->HasMore())
  {
    pMenuLst= pMenuIt->GetData();
    if (pMenuLst->MenuItemSelected(ulMenuID, id)) return(true);

    pMenuIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_ITEMLISTSELECT message was posted.
/// All registered listeners are iterated through and their ItemListSelect-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// ItemListSelectNotify-function is called.
bool IFateContainer::ItemListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFItemListListener*> *pItemIt;
  IFateContainer *pCont;
  IFItemListListener *pItemLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->ItemListSelectNotify(ulListID, pEntry)) return(true);
    if (pCont->ItemListSelected(ulListID, pEntry)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pItemIt= m_pItemListList->GetIterator();
  while(pItemIt->HasMore()) {
    pItemLst= pItemIt->GetData();
    if (pItemLst->ItemListSelected(ulListID, pEntry)) return(true);

    pItemIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_DROPLISTEXPAND message was posted.
/// All registered listeners are iterated through and their DropListExpand-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// DropListExpandNotify-function is called.
bool IFateContainer::DropListExpandNotify(unsigned long ulListID)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFDropListListener*> *pDropIt;
  IFateContainer *pCont;
  IFDropListListener *pDropLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->DropListExpandNotify(ulListID)) return(true);
    if (pCont->DropListExpanded(ulListID)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pDropIt= m_pDropListList->GetIterator();
  while(pDropIt->HasMore()) {
    pDropLst= pDropIt->GetData();
    if (pDropLst->DropListExpanded(ulListID)) return(true);

    pDropIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_DROPLISTSELECT message was posted.
/// All registered listeners are iterated through and their DropListSelect-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// DropListSelectNotify-function is called.
bool IFateContainer::DropListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFDropListListener*> *pDropIt;
  IFateContainer *pCont;
  IFDropListListener *pDropLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->DropListSelectNotify(ulListID, pEntry)) return(true);
    if (pCont->DropListSelected(ulListID, pEntry)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pDropIt= m_pDropListList->GetIterator();
  while(pDropIt->HasMore()) {
    pDropLst= pDropIt->GetData();
    if (pDropLst->DropListSelected(ulListID, pEntry)) return(true);

    pDropIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_DROPLISTCOLLAPSE message was posted.
/// All registered listeners are iterated through and their DropListCollapse-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// DropListCollapseNotify-function is called.
bool IFateContainer::DropListCollapseNotify(unsigned long ulListID)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFDropListListener*> *pDropIt;
  IFateContainer *pCont;
  IFDropListListener *pDropLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->DropListCollapseNotify(ulListID)) return(true);
    if (pCont->DropListCollapsed(ulListID)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pDropIt= m_pDropListList->GetIterator();
  while(pDropIt->HasMore()) {
    pDropLst= pDropIt->GetData();
    if (pDropLst->DropListCollapsed(ulListID)) return(true);

    pDropIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_SLIDERCHANGE message was posted.
/// All registered listeners are iterated through and their SliderChanged-
/// function is called.
/// For registered listeners derived from "IFateContainer" again their
/// SliderChangeNotify-function is called.
bool IFateContainer::SliderChangeNotify(unsigned long ulSliderID, int iVal)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFSliderListener*> *pSliderIt;
  IFateContainer *pCont;
  IFSliderListener *pSliderLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->SliderChangeNotify(ulSliderID, iVal)) return(true);
    if (pCont->SliderChanged(ulSliderID, iVal)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pSliderIt= m_pSliderList->GetIterator();
  while(pSliderIt->HasMore()) {
    pSliderLst= pSliderIt->GetData();
    if (pSliderLst->SliderChanged(ulSliderID, iVal)) return(true);

    pSliderIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_CHAR message was posted.
/// All registered listeners are iterated through and their Char-function is called.
/// For registered listeners derived from "IFateContainer" again their
/// CharNotify-function is called.
bool IFateContainer::CharNotify(TCHAR chChar)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFCharListener*> *pCharIt;
  IFateContainer *pCont;
  IFCharListener *pCharLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->CharNotify(chChar)) return(true);
    if (pCont->Char(chChar)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pCharIt= m_pCharList->GetIterator();
  while(pCharIt->HasMore()) {
    pCharLst= pCharIt->GetData();
    if (pCharLst->Char(chChar)) return(true);

    pCharIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// This function is called when the WM_CLIENTCONNECT message was posted.
/// All registered listeners are iterated through and their ClientConnect-method 
///is called.
/// For registered listeners derived from "IFateContainer" again their
/// ClientConnectNotify-method is called.
bool IFateContainer::ClientConnectNotify(CFServer *pServer)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFConnectionListener*> *pConnIt;
  IFateContainer *pCont;
  IFConnectionListener *pConnLst;  
  
  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to notify their registrees again
    if (pCont->ClientConnectNotify(pServer)) return(true);
    if (pCont->ClientConnect(pServer)) return(true);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pConnIt= m_pConnList->GetIterator();
  while(pConnIt->HasMore()) {
    pConnLst= pConnIt->GetData();
    if (pConnLst->ClientConnect(pServer)) return(true);

    pConnIt->Next();
  }

  return(false);  // event not handled
}

//--------------------------------------------------------------------------------
/// Iterates recursively through all registered "IFateComponents" and returns
/// the component matching the specified ID.
/// If no component with the specified ID is found, NULL is returned.
IFateComponent* IFateContainer::FindCompByID(unsigned long ulID)
{
  CFListIterator<IFateContainer*> *pContIt;
  CFListIterator<IFateComponent*> *pCompIt;
  IFateContainer *pCont;
  IFateComponent *pComp;

  // first iterate through registered containers
  pContIt= m_pContList->GetIterator();
  while(pContIt->HasMore()) {
    pCont= pContIt->GetData();
    // containers have to ask their registrees again
    pComp= pCont->FindCompByID(ulID);
    // recursion successful?
    if (pComp) return(pComp);
    
    pContIt->Next();
  }
  
  // now iterate through listeners
  pCompIt= m_pCompList->GetIterator();
  while(pCompIt->HasMore()) {
    pComp= pCompIt->GetData();

    if (pComp->GetId() == ulID) {
      return pComp;
    }
    pCompIt->Next();
  }  

  return(NULL);
}

//--------------------------------------------------------------------------------
/// Removes all registered components from the containers datastructure
void IFateContainer::ClearComponents()
{
  m_pCompList->Clear();
  m_pContList->Clear();
}

//--------------------------------------------------------------------------------
bool IFateContainer::Remove(IFateComponent *pComp)
{
  return(m_pCompList->Delete(pComp));
}

