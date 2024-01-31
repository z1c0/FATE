#include "../../framework/include/fate.h"
#include "Snake.h"
#include "Missile.h"

#ifndef __LEVELCONT__H__
#define __LEVELCONT__H__

//------------------------------------------------------------------------------

#define CELL_DIM 4

#define CELL_EMPTY     1
#define CELL_WALL      2
#define CELL_SNAKE     3
#define CELL_FOOD      4
#define CELL_MISSILE   5

//------------------------------------------------------------------------------
class CLevelContainer : public IFateComponent  
{

public:
  
  CLevelContainer();
  virtual ~CLevelContainer();

  virtual bool Create();

  virtual void Draw();

  virtual bool StylusDown(int xPos, int yPos);
  virtual bool StylusMove(int xPos, int yPos);
  virtual bool StylusUp(int xPos, int yPos);
  
  virtual bool KeyDown(PdaKey key);

  void SetCellType(const CFPoint& point, int celltype) { SetCellType(point.GetY(), point.GetX(), celltype); };
  void SetCellType(int row, int col, int celltype);
  int GetCellType(const CFPoint& point) const { return GetCellType(point.GetY(), point.GetX()); };
  int GetCellType(int row, int col) const;

  const int GetRows() { return m_rows; };
  const int GetCols() { return m_cols; };

  const int GetEditMode() { return m_editmode; };
  void SetEditMode(bool editmode) { m_editmode = editmode; };

  void Animate();
  void KillSnake();
  void PlaceFood();

private:

  void ClearCells();
  bool Load(TCHAR *pszFileName);
  bool Save(TCHAR *pszFileName);

  CFBitmap m_bmpback;
  CSnake m_snake;
  CMissile* m_missiles[MAX_MISSILES];

  bool m_editmode;
  int m_rows;
  int m_cols;
  int *m_cells;
};

#endif  // __LEVELCONT__H__
