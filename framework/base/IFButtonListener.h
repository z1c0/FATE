#ifndef __IFBUTTONLISTENER__H__
#define __IFBUTTONLISTENER__H__

//------------------------------------------------------------------------------
class IFButtonListener
{
public:
  virtual bool ButtonPressed(unsigned long ulBtnID)= 0;
  virtual bool ButtonReleased(unsigned long ulBtnID)= 0;
};

#endif // __IFBUTTONLISTENER__H__
