#include "../../framework/include/fate.h"


//#define APP_SECTION __attribute__ ((section ("sect_APP"))) 
#define APP_SECTION

//------------------------------------------------------------------------------
class CSimple : public CFateApp
{

public:
  
  CSimple() APP_SECTION;
  virtual ~CSimple() APP_SECTION;

  virtual bool InitFateApp() APP_SECTION;;

  virtual void Draw() APP_SECTION;

  virtual bool StylusDown(int xPos, int yPos) APP_SECTION;
  virtual bool StylusMove(int xPos, int yPos) APP_SECTION;
  
  virtual bool KeyDown(PdaKey key) APP_SECTION;

  virtual bool ButtonReleased(unsigned long ulBtnID) APP_SECTION;

private:

  CFButton *m_pBtnQuit;

  CFButton *m_pBtnAction;
};
