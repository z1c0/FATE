#pragma once

#include "FSystem.h"

//------------------------------------------------------------------------------
class CFateApp;
class CFBitmap;
class CFSystem;
class IFateContainer;

//--------------------------------------------------------------------------------
/// Enumeration for the Pocket PCs hardware keys.
enum PdaKey
{
  KeyWest,
  KeyEast,
  KeyNorth,
  KeySouth,
  KeyOne,
  KeyTwo,
  KeyThree,
  KeyFour,
  KeyRec,
  KeyEnter,
  KeyVolUp,
  KeyVolDown,
  KeyNone
};

//--------------------------------------------------------------------------------
class IFateComponent  
{
public:
  IFateComponent() 
  { 
    m_pszClassName= TEXT("IFateComponent");
    m_ulID        = 0xFFFFFFFF;
    m_bVisible    = true; 
    m_bEnabled    = true;
    m_app         = NULL; 
    m_pSystem     = NULL;
    m_pBmp        = NULL;
    m_iPosX       = 0;
    m_iPosY       = 0;
    m_iWidth      = 0;
    m_iHeight     = 0;
    // help for detecting memory leaks
    m_iInstanceCounter++;
  }; 
  virtual ~IFateComponent() 
  {
    // help for detecting memory leaks
    m_iInstanceCounter--;
  }; 

  virtual bool StylusDown(int xPos, int yPos) { return(false); }; 
	virtual bool StylusMove(int xPos, int yPos) { return(false); }; 
	virtual bool StylusUp(int xPos, int yPos) { return(false); }; 
  
  virtual bool KeyDown(PdaKey) { return(false); };
  virtual bool KeyUp(PdaKey) { return(false); };
  
  virtual void Draw() {};
  
  virtual void SetVisible(bool bVisible) 
  { 
    m_bEnabled= bVisible;
    m_bVisible= bVisible; 
    if (m_pSystem) {
      m_pSystem->ForceRedraw();
    }
  };
  bool GetVisible() { return(m_bVisible); };
  bool IsVisible() { return(m_bVisible); };
  
  virtual void SetEnabled(bool bEnabled) { m_bEnabled= bEnabled; };
  bool GetEnabled() { return(m_bEnabled); };
  bool IsEnabled() { return(m_bEnabled); };
  
  void SetId (unsigned long ulID) { m_ulID= ulID; };
  unsigned long GetId() { return(m_ulID); };
  
  virtual void SetAppObject(CFateApp *app) { m_app = app; };
  
  virtual void SetDestBitmap(CFBitmap *pBmp) { m_pBmp = pBmp; };  
  
  virtual void SetSystem(CFSystem *pSystem) { m_pSystem = pSystem; };
  
  virtual void SetX(int iPosX) { m_iPosX= iPosX; };
  int GetX() { return(m_iPosX); };
  
  virtual void SetY(int iPosY) { m_iPosY= iPosY; };
  int GetY() { return(m_iPosY); };
  
  virtual void SetWidth(int iWidth) { m_iWidth= iWidth; };
  int GetWidth() { return(m_iWidth); };
  
  virtual void SetHeight(int iHeight) { m_iHeight= iHeight; };
  int GetHeight() { return(m_iHeight); };
  
  int GetRight() { return(m_iPosX + m_iWidth); };
  int GetBottom() { return(m_iPosY + m_iHeight); };
  
  virtual const TCHAR* GetClassName() { return(m_pszClassName); };
  
  //--------------------------------------------------------------------------------
  /// Sets the pointer to parent container. Method should not be called manually. It's
  /// only triggered by framework mechanisms. 
  virtual void SetParent(IFateContainer *pCont) { m_pParent= pCont; }; 
  //--------------------------------------------------------------------------------
  /// Returns a pointer to parent container.
  IFateContainer *GetParent() { return(m_pParent); }; 

  static int GetNumberOfInstances() { return(m_iInstanceCounter); };

protected:

  CFateApp *m_app;       /// pointer to main application component
  CFSystem *m_pSystem;

  IFateContainer *m_pParent;  // pointer to parent container
  bool m_bVisible;
  bool m_bEnabled;  
  
  CFBitmap *m_pBmp;
  unsigned long m_ulID;          /// unique identifier of component
  
  int m_iPosX;
  int m_iPosY;
  int m_iWidth;
  int m_iHeight;
  
  const TCHAR* m_pszClassName;

  static int m_iInstanceCounter;
};


