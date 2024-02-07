#ifndef __IFREMOTEWIDGET__H__
#define __IFREMOTEWIDGET__H__

//--------------------------------------------------------------------------------
class IFRemoteWidget  
{
public:
	IFRemoteWidget()
  {
    m_ulRemoteID= 0xFFFFFFFF;
  }
  virtual ~IFRemoteWidget() {};
  virtual bool Action()= 0;
  void SetWidgetID(unsigned long ulID) { m_ulRemoteID= ulID; };
  unsigned long GetWidgetID() { return(m_ulRemoteID); };

protected:
  
  unsigned long m_ulRemoteID;
};

#endif  // __IFREMOTEWIDGET__H__