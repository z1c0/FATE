#ifndef __IFSLIDERLISTENER__H__
#define __IFSLIDERLISTENER__H__


//------------------------------------------------------------------------------
class IFSliderListener  
{
public:
  virtual bool SliderChanged(unsigned long ulSliderID, int iValue)= 0;
};

#endif  // __IFSLIDERLISTENER__H__
