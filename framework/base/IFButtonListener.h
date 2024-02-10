#pragma once 

class IFButtonListener
{
public:
  virtual bool ButtonPressed(unsigned long ulBtnID)= 0;
  virtual bool ButtonReleased(unsigned long ulBtnID)= 0;
};


