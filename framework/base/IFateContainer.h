#ifndef __IFATECONTAINER__H__
#define __IFATECONTAINER__H__

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
  IFateContainer() FATE_SECTION;
	virtual ~IFateContainer() FATE_SECTION;
  bool Add(IFateComponent& comp) FATE_SECTION;
  bool Add(IFateContainer& cont) FATE_SECTION;
  bool AddButtonListener(IFButtonListener *pBtnLst) FATE_SECTION;
  bool AddMenuListener(IFMenuListener *pMenuLst) FATE_SECTION;
  bool AddItemListListener(IFItemListListener *pItemListLst) FATE_SECTION;
  bool AddDropListListener(IFDropListListener *pDropListLst) FATE_SECTION;
  bool AddSliderListener(IFSliderListener *pSliderLst) FATE_SECTION;
  bool AddCharListener(IFCharListener& charlistener) FATE_SECTION;
  bool AddConnectionListener(IFConnectionListener *pConnLst) FATE_SECTION;
	
  bool StylusDownNotify(int xPos, int yPos) FATE_SECTION;
	bool StylusMoveNotify(int xPos, int yPos) FATE_SECTION;
	bool StylusUpNotify(int xPos, int yPos) FATE_SECTION;
  
  bool KeyDownNotify(PdaKey key) FATE_SECTION;
  bool KeyUpNotify(PdaKey key) FATE_SECTION;
  
  bool ButtonPressNotify(unsigned long ulBtnID) FATE_SECTION;
  bool ButtonReleaseNotify(unsigned long ulBtnID) FATE_SECTION;
  
  bool MenuSelectNotify(unsigned long ulMenuID, int iSelMain, int iSelSub) FATE_SECTION;
  
  bool ItemListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry) FATE_SECTION;  
  
  bool DropListExpandNotify(unsigned long ulListID) FATE_SECTION;
  bool DropListSelectNotify(unsigned long ulListID, ITEMLISTENTRY *pEntry) FATE_SECTION; 
  bool DropListCollapseNotify(unsigned long ulListID) FATE_SECTION;
  
  bool SliderChangeNotify(unsigned long ulSliderID, int iVal) FATE_SECTION;
  
  bool CharNotify(TCHAR chChar) FATE_SECTION;
  
  bool ClientConnectNotify(CFServer *pServer) FATE_SECTION;
  
  void ClearComponents() FATE_SECTION;  
  bool Remove(IFateComponent *pComp) FATE_SECTION;
  
  void DrawNotify() FATE_SECTION;
  
  IFateComponent* FindCompByID(unsigned long ulID) FATE_SECTION;

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

#endif  // __IFATECONTAINER__H__
