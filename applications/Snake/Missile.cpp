#include "Missile.h"
#include "LevelCont.h"


//------------------------------------------------------------------------------
CMissile::CMissile(CLevelContainer& levelcont) :
  m_levelcont(levelcont),
  m_ticker(MISSILE_SPEED),
  m_active(false)
{
}

//------------------------------------------------------------------------------
CMissile::~CMissile()
{
}

//------------------------------------------------------------------------------
void CMissile::Launch(const CFPoint& pos, const CFPoint& vec)
{
  m_active = true;
  m_pos = pos;
  m_movevector = vec;
  m_pos += m_movevector;
}

//------------------------------------------------------------------------------
void CMissile::Move()
{
  if (m_active) {
    if (m_ticker.HasTimeElapsed()) {
      m_levelcont.SetCellType(m_pos, CELL_EMPTY);
      m_pos += m_movevector;
      
      CheckForImpact(m_pos);
    }
  }
}

//------------------------------------------------------------------------------
void CMissile::CheckForImpact(const CFPoint& pos)
{
  int celltype = m_levelcont.GetCellType(m_pos);
  switch (celltype) {
  case CELL_WALL:
    m_active = false;
    break;

  case CELL_SNAKE:
    m_active = false;
    m_levelcont.KillSnake();
    break;

  case CELL_FOOD:
    m_active = false;
    m_levelcont.SetCellType(m_pos, CELL_EMPTY);
    m_levelcont.PlaceFood();
    break;

  default:
    m_levelcont.SetCellType(m_pos, CELL_MISSILE);
    break;
  }
}

