#include "Snake.h"
#include "LevelCont.h"


//------------------------------------------------------------------------------
CSnake::CSnake(CLevelContainer& levelcont) :
  m_levelcont(levelcont),
  m_ticker(INITIAL_SNAKE_SPEED)
{
  m_alive = true;
  m_growby = 0;
  m_readyforinput = true;
}

//------------------------------------------------------------------------------
CSnake::~CSnake()
{
  CFListIterator<CFPoint*>* iterator = m_parts.GetIterator();
  while (iterator->HasMore()) {
    delete iterator->GetData();
    iterator->Next();
  }
}

//------------------------------------------------------------------------------
void CSnake::Init(int tailx, int taily, int direction)
{
  SetDirection(direction);

  for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
    CFPoint* point = NULL;
    switch (direction) {        
    case SNAKE_DIR_UP:
      point = new CFPoint(tailx, taily - i);
      break;

    case SNAKE_DIR_DOWN:
      point = new CFPoint(tailx, taily + i);
      break;

    case SNAKE_DIR_LEFT:
      point = new CFPoint(tailx - i, taily);
      break;

    case SNAKE_DIR_RIGHT:
    default:
      point = new CFPoint(tailx + i, taily);
      break;
    }
    m_parts.Append(point);
  }

  CFListIterator<CFPoint*>* iterator = m_parts.GetIterator();
  while (iterator->HasMore()) {
    CFPoint *point = iterator->GetData();
    m_levelcont.SetCellType(point->GetY(), point->GetX(), CELL_SNAKE);
    iterator->Next();
  }
}

//------------------------------------------------------------------------------
void CSnake::SetDirection(int direction)
{
  if (m_readyforinput) {
    m_readyforinput = false;

    switch (direction) {
      case SNAKE_DIR_DOWN:
        m_movevector.Set(0, 1);
        break;

      case SNAKE_DIR_UP:
        m_movevector.Set(0, -1);
        break;

      case SNAKE_DIR_LEFT:
        m_movevector.Set(-1, 0);
        break;

      case SNAKE_DIR_RIGHT:
        m_movevector.Set(1, 0);
        break;
    }
  }
}

//------------------------------------------------------------------------------
void CSnake::Move()
{
  if (m_alive) {
    if (m_ticker.HasTimeElapsed()) {
      m_readyforinput = true;

      CFPoint *tmp = NULL;
      //
      // Grow or move?
      //
      if (m_growby > 0) {
        m_growby --;
        tmp = new CFPoint(*m_parts.GetTail());
        m_parts.Append(tmp);
      }
      else {
        // Clear the tail.
        tmp = m_parts.GetHead();
        m_levelcont.SetCellType(tmp->GetY(), tmp->GetX(), CELL_EMPTY);

        // "Move" the snake
        CFPoint *prev = NULL;
        CFListIterator<CFPoint*>* iterator = m_parts.GetIterator();
        while (iterator->HasMore()) {
          tmp = iterator->GetData();
          if (prev != NULL) {
            *prev = *tmp;
          }
          prev = tmp;
          iterator->Next();
        }
      }
      // Update the head
      tmp = m_parts.GetTail();
      *tmp += m_movevector;

      // Did we crash, eat, ...?
      int celltype = m_levelcont.GetCellType(tmp->GetY(), tmp->GetX());
      switch (celltype) {
      case CELL_FOOD:
        Eat();
        break;

      case CELL_WALL:
      case CELL_SNAKE:
        Die();
        break;
      }
      m_levelcont.SetCellType(tmp->GetY(), tmp->GetX(), CELL_SNAKE);
    }
  }
}

//------------------------------------------------------------------------------
void CSnake::Die()
{
  m_alive = false;
}

//------------------------------------------------------------------------------
void CSnake::Eat()
{
  // Grow.
  unsigned int speed = m_ticker.GetTickInterval();
  m_ticker.SetTickInterval(max(speed - SNAKE_SPEED_INCREASE_FACTOR, MAX_SNAKE_SPEED));
  m_growby = SNAKE_GROW_BY;
  m_levelcont.PlaceFood();
}

//------------------------------------------------------------------------------
const CFPoint& CSnake::GetHeadPosition() const
{
  return *m_parts.GetTail();
}

//------------------------------------------------------------------------------
const CFPoint& CSnake::GetMoveVector() const
{
  return m_movevector;
}
