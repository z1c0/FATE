#ifndef __IFCONNECTONLISTENER__H__
#define __IFCONNECTONLISTENER__H__


class CFServer;

//--------------------------------------------------------------------------------
class IFConnectionListener  
{
public:
  virtual bool ClientConnect(CFServer *pServer)= 0;
};

#endif  // __IFCONNECTONLISTENER__H__
