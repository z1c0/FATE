#ifndef __SNAKE__H__
#define __SNAKE__H__

#include "../../framework/include/fate.h"

//------------------------------------------------------------------------------

#define INITIAL_SNAKE_LENGTH         10
#define INITIAL_SNAKE_SPEED          90
#define MAX_SNAKE_SPEED               5
#define SNAKE_GROW_BY                10
#define SNAKE_SPEED_INCREASE_FACTOR   5

#define SNAKE_DIR_UP     0
#define SNAKE_DIR_DOWN   1
#define SNAKE_DIR_LEFT   2
#define SNAKE_DIR_RIGHT  3

//------------------------------------------------------------------------------

class CLevelContainer;

//------------------------------------------------------------------------------
class CSnake  
{

public:
  
  CSnake(CLevelContainer& levelcont);
  virtual ~CSnake();

  void Init(int tailx, int taily, int direction);
  void SetDirection(int direction);
  void Move();
  void Die();
  void Eat();

  bool IsAlive() const { return m_alive; };
  const CFPoint& GetHeadPosition() const;
  const CFPoint& GetMoveVector() const;

private:

  CFLinkedList<CFPoint*> m_parts;
  CFPoint m_movevector;
  CFTicker m_ticker;

  CLevelContainer& m_levelcont;

  bool m_alive;
  unsigned int m_growby;
  bool m_readyforinput;
};

#endif  // __SNAKE__H__
