#pragma once

#include "../util/FLinkedList.h"
#include "IFateComponent.h"
#include "IFButtonListener.h"
#include "IFMenuListener.h"
#include "IFItemListListener.h"
#include "IFDropListListener.h"
#include "IFSliderListener.h"
#include "IFCharListener.h"
#include "IFConnectionListener.h"

//--------------------------------------------------------------------------------
class IFateContainer : public IFateComponent, public IFButtonListener, 
                       public IFMenuListener, public IFItemListListener, 
                       public IFDropListListener, public IFSliderListener, 
                       public IFCharListener, public IFConnectionListener
{
public:
  IFateContainer();
	virtual ~IFateContainer();
  bool Add(IFateComponent& comp);
  bool Add(IFateContainer& cont);
  bool AddButtonListener(IFButtonListener *pBtnLst);
  bool AddMenuListener(IFMenuListener *pMenuLst);
  bool AddItemListListener(IFItemListListener *pItemListLst);
  bool AddDropListListener(IFDropListListener *pDropListLst);
  bool AddSliderListener(IFSliderListener *pSliderLst);
  bool AddCharListener(IFCharListener& charlistener);
  bool AddConnectionListener(IFConnectionListener *pConnLst);
	
  bool StylusDownNotify(int xPos, int yPos);
	bool StylusMoveNotify(int xPos, int yPos);
	bool StylusUpNotify(int xPos, int yPos);
  
  bool KeyDownNotify(PdaKey key);
  bool KeyUpNotify(PdaKey key);
  
  bool ButtonPressNotify(unsigned long ulBtnID);
  bool ButtonReleaseNotify(unsigned long ulBtnID);
  
  bool MenuItemSelectNotify(unsigned long ulMenuID, unsigned int id);
  
  bool ItemListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry);  
  
  bool DropListExpandNotify(unsigned long ulListID);
  bool DropListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry); 
  bool DropListCollapseNotify(unsigned long ulListID);
  
  bool SliderChangeNotify(unsigned long ulSliderID, int iVal);
  
  bool CharNotify(TCHAR chChar);
  
  bool ClientConnectNotify(CFServer *pServer);
  
  void ClearComponents();  
  bool Remove(IFateComponent *pComp);
  
  void DrawNotify();
  
  IFateComponent* FindCompByID(unsigned long ulID);

protected:
  CFLinkedList<IFateComponent*>       *m_pCompList;
  CFLinkedList<IFateContainer*>       *m_pContList;
  CFLinkedList<IFButtonListener*>     *m_pBtnList;
  CFLinkedList<IFMenuListener*>       *m_pMenuList;
  CFLinkedList<IFItemListListener*>   *m_pItemListList;
  CFLinkedList<IFDropListListener*>   *m_pDropListList;
  CFLinkedList<IFSliderListener*>     *m_pSliderList;
  CFLinkedList<IFCharListener*>       *m_pCharList;
  CFLinkedList<IFConnectionListener*> *m_pConnList;
};

