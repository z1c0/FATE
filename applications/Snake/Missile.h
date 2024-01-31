#ifndef __MISSILE_H__
#define __MISSILE_H__

#include "../../framework/include/fate.h"

//------------------------------------------------------------------------------

#define MAX_MISSILES      3
#define MISSILE_SPEED    20

//------------------------------------------------------------------------------

class CLevelContainer;

//------------------------------------------------------------------------------
class CMissile  
{

public:
  
  CMissile(CLevelContainer& levelcont);
  virtual ~CMissile();

  void Launch(const CFPoint& pos, const CFPoint& vec);
  void Move();
  bool IsActive() const { return m_active; };

private:

  void CheckForImpact(const CFPoint& pos);

  CFTicker m_ticker;
  CFPoint m_pos;
  CFPoint m_movevector;

  CLevelContainer& m_levelcont;

  bool m_active;
};

#endif  // __MISSILE_H__