#ifndef __IFMENULISTENER__H__
#define __IFMENULISTENER__H__

//------------------------------------------------------------------------------
class IFMenuListener 
{
public:
  virtual bool MenuSelected(unsigned long ulMenuID, int iSelMain, int iSelSub)= 0;
};

#endif  // __IFMENULISTENER__H__
