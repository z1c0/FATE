#pragma once

#include "../base/IFateContainer.h"

class CFPanel : public IFateContainer  
{
public:
  CFPanel() : IFateContainer() {};
  virtual ~CFPanel() {};
  
  // methods implemented emtpy; derive from CFPanel if further functionality is needed
  virtual bool StylusDown(int xPos, int yPos) { return(false); };
  virtual bool StylusMove(int xPos, int yPos) { return(false); };
	virtual bool StylusUp(int xPos, int yPos) { return(false); };
  virtual bool KeyDown(PdaKey key) { return(false); };
  virtual bool KeyUp(PdaKey key) { return(false); };
  virtual void Draw() {};
  
  // event handling methods are implemented empty
  virtual bool ButtonPressed(unsigned long ulBtnID) { return(false); };
  virtual bool ButtonReleased(unsigned long ulBtnID) { return(false); };
  
  virtual bool MenuItemSelected(unsigned long ulMenuID, unsigned int id) { return false; };
  
  virtual bool ItemListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry) { return(false); };
  
  virtual bool DropListExpanded(unsigned long ulListID) { return(false); };
  virtual bool DropListSelected(unsigned long ulListID, ITEMLISTENTRY *pEntry) { return(false); };
  virtual bool DropListCollapsed(unsigned long ulListID) { return(false); };
  
  virtual bool SliderChanged(unsigned long ulSliderID, int iVal) { return(false); };
  
  virtual bool Char(TCHAR chChar) { return(false); };
  
  virtual bool ClientConnect(class CFServer* pServer) { return(false); };
};


