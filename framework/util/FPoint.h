#ifndef __FPOINT__H__
#define __FPOINT__H__

//------------------------------------------------------------------------------
class CFPoint
{

public:

  CFPoint() { m_x = 0; m_y = 0; };
  CFPoint(int x, int y) { m_x = x; m_y = y; };

  int GetX() const { return m_x; };
  int GetY() const { return m_y; };
  
  void SetX(int x) { m_x = x; };
  void SetY(int y) { m_y = y; };
  void Set(int x, int y) { m_x = x; m_y = y; };

  void Add(int x, int y) { m_x += x; m_y += y; };
  void AddX(int x) { m_x += x; };
  void AddY(int y) { m_y += y; };

  CFPoint& operator=(const CFPoint &point)
  {
    // avoid self-assinging
    if (this != &point) {
      m_x = point.m_x;
      m_y = point.m_y;
    }
    return *this;
  }

  CFPoint& operator+=(const CFPoint &point)
  {
    m_x += point.m_x;
    m_y += point.m_y;
    return *this;
  }

private:

  int m_x;
  int m_y;
};

#endif  // __FPOINT__H__