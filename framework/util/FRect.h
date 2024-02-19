#pragma once

//------------------------------------------------------------------------------
class CFRect
{
public:
  CFRect() { m_left = 0; m_top = 0; m_width = 0; m_height = 0; };
  CFRect(int left, int top, int width, int height) 
  { 
    m_left = left;
    m_top = top;
    m_width = width;
    m_height = height;
  };

  int GetLeft() const { return m_left; };
  int GetTop() const { return m_top; };
  int GetWidth() const { return m_width; };
  int GetHeight() const { return m_height; };  

  void SetLeft(int left) { m_left = left; };
  void SetTop(int top) { m_top = top; };
  void SetWidth(int width) { m_width = width; };
  void SetHeight(int height) { m_height = height; };  

  CFRect& operator=(const CFRect& rect)
  {
    // Avoid self-assinging.
    if (this != &rect) {
      m_left = rect.m_left;
      m_top = rect.m_top;
      m_width = rect.m_width;
      m_height = rect.m_height;
    }
    return *this;
  }

  bool Intersects(const CFRect& rect) 
  {
    return Intersects(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
  }

  bool Intersects(const RECT& rect) 
  {
    return Intersects(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
  }

  bool Intersects(int left, int top, int widht, int height)
  {
    // TODO
    return false;
  }

private:
  int m_left;
  int m_top;
  int m_width;
  int m_height;
};

