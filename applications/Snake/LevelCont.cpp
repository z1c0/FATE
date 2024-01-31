#include "LevelCont.h"

//------------------------------------------------------------------------------
#define EMTPY_COLOR  RGB(240, 240, 240)
#define SNAKE_COLOR  RGB(0, 180, 0)
#define FOOD_COLOR  RGB(255, 0, 255)
#define MISSILE_COLOR  RGB(200, 0, 0)


//------------------------------------------------------------------------------
CLevelContainer::CLevelContainer() :
  m_snake(*this)
{
  m_cells = NULL;
  for (int i = 0; i < MAX_MISSILES; i++) {
    m_missiles[i] = new CMissile(*this);
  }
}

//------------------------------------------------------------------------------
CLevelContainer::~CLevelContainer()
{
  if (m_cells) {
    delete m_cells;
  }
  for (int i = 0; i < MAX_MISSILES; i++) {
    delete m_missiles[i];
  }
}

//------------------------------------------------------------------------------
bool CLevelContainer::Create()
{
  m_bmpback.SetDestBitmap(*m_pBmp);
  m_bmpback.Create(GetWidth(), GetHeight());

  m_cols = GetWidth() / CELL_DIM;
  m_rows = GetHeight() / CELL_DIM;
  
  // Create and init cell matrix
  m_cells = new int[m_rows * m_cols];
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      if ((i == 0)||(j == 0)||(i == m_rows - 1)||(j == m_cols - 1)) {
        m_cells[i * m_cols + j] = CELL_WALL;
      }
      else {
        m_cells[i * m_cols + j] = CELL_EMPTY;
      }
    }
  }

  m_snake.Init(5, 5, SNAKE_DIR_DOWN);
  PlaceFood();

  return true;
}

//------------------------------------------------------------------------------
void CLevelContainer::Draw()
{
  m_bmpback.SolidFill(EMTPY_COLOR);

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      int y = i * CELL_DIM;
      int x = j * CELL_DIM;
      switch (m_cells[i * m_cols + j]) {
      case CELL_WALL:
        m_bmpback.SetBackgroundColor(RGB(0, 0, 200));
        m_bmpback.SetColor(RGB(0, 0, 200));
        m_bmpback.DrawFilledRect(x, y, CELL_DIM, CELL_DIM);
        break;

      case CELL_SNAKE:
        m_bmpback.SetBackgroundColor(SNAKE_COLOR);
        m_bmpback.SetColor(SNAKE_COLOR);
        m_bmpback.DrawFilledRect(x, y, CELL_DIM, CELL_DIM);
        break;

      case CELL_FOOD:
        m_bmpback.SetBackgroundColor(FOOD_COLOR);
        m_bmpback.SetColor(FOOD_COLOR);
        m_bmpback.DrawFilledRect(x, y, CELL_DIM, CELL_DIM);
        break;

      case CELL_MISSILE:
        m_bmpback.SetBackgroundColor(MISSILE_COLOR);
        m_bmpback.SetColor(MISSILE_COLOR);
        m_bmpback.DrawFilledRect(x + 1, y + 1, CELL_DIM - 1, CELL_DIM - 1);
        break;


      case CELL_EMPTY:
        break;
      }
    }
  }

  m_bmpback.Blit();
}

//------------------------------------------------------------------------------
bool CLevelContainer::StylusDown(int xPos, int yPos)
{
  bool ret = false;
  if (m_editmode) {
    if (m_bmpback.PointInside(xPos, yPos)) {
      ret = true;

      int i = yPos / CELL_DIM;
      int j = xPos / CELL_DIM;
      SetCellType(i, j, CELL_WALL);
    }
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CLevelContainer::StylusMove(int xPos, int yPos)
{
  bool ret = false;
  if (m_editmode) {
    if (m_bmpback.PointInside(xPos, yPos)) {
      ret = true;

      int i = yPos / CELL_DIM;
      int j = xPos / CELL_DIM;
      SetCellType(i, j, CELL_WALL);
    }
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CLevelContainer::StylusUp(int xPos, int yPos)
{
  return false;
}

//------------------------------------------------------------------------------
bool CLevelContainer::KeyDown(PdaKey key)
{
  switch (key) {
  case KeyNorth:
    m_snake.SetDirection(SNAKE_DIR_UP);
    break;

  case KeySouth:
    m_snake.SetDirection(SNAKE_DIR_DOWN);
    break;

  case KeyWest:
    m_snake.SetDirection(SNAKE_DIR_LEFT);
    break;

  case KeyEast:
    m_snake.SetDirection(SNAKE_DIR_RIGHT);
    break;

  case KeyEnter:
    int i;
    for (i = 0; i < MAX_MISSILES; i++) {
      if (!m_missiles[i]->IsActive()) {
        m_missiles[i]->Launch(m_snake.GetHeadPosition(), m_snake.GetMoveVector());
        break;
      }
    }
    break;

  default:
    break;
  }

  return true;
}

//------------------------------------------------------------------------------
void CLevelContainer::ClearCells()
{
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      m_cells[i * m_cols + j] = CELL_EMPTY;
    }
  }
  Draw();
}

//------------------------------------------------------------------------------
void CLevelContainer::Animate()
{
  for (int i = 0; i < MAX_MISSILES; i++) {
    m_missiles[i]->Move();
  }
  m_snake.Move();
}

//------------------------------------------------------------------------------
void CLevelContainer::KillSnake()
{
  m_snake.Die();
}

//------------------------------------------------------------------------------
void CLevelContainer::PlaceFood()
{
  bool ok = false;
  while (!ok) {
    int i = CFSystem::GetRandomNumber(m_rows);
    int j = CFSystem::GetRandomNumber(m_cols);
    if (m_cells[i * m_cols + j] == CELL_EMPTY) {
      ok = true;
      m_cells[i * m_cols + j] = CELL_FOOD;
    }
  }
}

//------------------------------------------------------------------------------
int CLevelContainer::GetCellType(int row, int col) const
{
  int celltype = m_cells[row * m_cols + col];
  return celltype;
}

//------------------------------------------------------------------------------
void CLevelContainer::SetCellType(int row, int col, int celltype)
{
  m_cells[row * m_cols + col] = celltype;
  Draw();
}

//------------------------------------------------------------------------------
bool CLevelContainer::Load(TCHAR *pszFileName)
{
  CFFile file(pszFileName);
  bool ret = file.IsValid();
  if (ret) {
    unsigned char* bytes = NULL;
    unsigned int size = file.GetSize();
    bool ret = file.ReadBytes(bytes, size);
    if (ret) {
      unsigned int expected = CELL_DIM * m_cols * m_rows;
      if (size == expected) {
        int *cells = (int*)bytes;
        for (int i = 0; i < m_rows; i++) {
          for (int j = 0; j < m_cols; j++) {
            int celltype = cells[i * m_cols + j];
            m_cells[i * m_cols + j] = celltype;
          }
        }
        Draw();
      }
    }
    if (bytes) {
      delete bytes;
    }
  }
  return ret;
}

//------------------------------------------------------------------------------
bool CLevelContainer::Save(TCHAR *pszFileName)
{
  CFFile file(pszFileName);
  bool ret = file.WriteBytes((unsigned char*)m_cells, m_rows * m_cols * sizeof(int));

  return ret;
}
