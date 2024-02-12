#include "../../framework/include/fate.h"

class CSimple : public CFateApp
{
public:  
  CSimple();
  virtual ~CSimple();

  virtual bool InitFateApp();
  virtual void Draw();
  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);  
  virtual bool KeyDown(PdaKey key);
  virtual bool ButtonReleased(unsigned long ulBtnID);

private:
  CFButton *m_pBtnQuit;
  CFButton *m_pBtnAction;
};
