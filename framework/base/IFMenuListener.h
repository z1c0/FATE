#pragma once

class IFMenuListener 
{
public:
  virtual bool MenuItemSelected(unsigned long ulMenuID, unsigned int id)= 0;
};


