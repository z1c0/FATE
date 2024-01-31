#ifndef __FMSGPANEL__H__
#define __FMSGPANEL__H__

#include "FPanel.h"
#include "FLabel.h"


//------------------------------------------------------------------------------
class CFMsgPanel : public CFPanel  
{
public:
  /// Singleton creation!
  static CFMsgPanel* Create() 
  { 
    if (!m_bCreated) {
      m_bCreated= true;
      return (new CFMsgPanel());
    } else {
      return(NULL);
    }
  };  
  virtual ~CFMsgPanel() { delete(m_labelMsg); };
  
  void SetMsg(LPCTSTR pszMsg) 
  { 
    m_bVisible= true;
    m_labelMsg->SetText(pszMsg); 
  };
  
  virtual void SetSystem(CFSystem *pSystem) 
  {
    m_pSystem= pSystem;

    int iWidth = m_pSystem->GetWidth() - 50;

    m_labelMsg = new CFLabel(iWidth, 100);
    m_labelMsg->SetVisible(true);
    Add(*m_labelMsg);
    m_labelMsg->SetX(10);
    m_labelMsg->SetY(10);
  };

  virtual bool StylusDown(int xPos, int yPos)
  {
    m_app->ReleaseCaptureComp();
    SetVisible(false);
    return true;
  };

private:

  CFMsgPanel() : CFPanel() 
  {
    SetVisible(false);
  };
  CFLabel *m_labelMsg;
  
  static bool m_bCreated;
};

#endif  // __FMSGPANEL__H__
