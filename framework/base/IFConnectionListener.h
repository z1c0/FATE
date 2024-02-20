#pragma once

class CFServer;

//--------------------------------------------------------------------------------
class IFConnectionListener  
{
public:
  virtual bool ClientConnect(CFServer *pServer)= 0;
};

