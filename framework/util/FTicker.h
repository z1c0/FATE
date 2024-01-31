#ifndef __TICKER__H__
#define __TICKER__H__

//------------------------------------------------------------------------------
class CFTicker
{

public:

  CFTicker() 
  {
    m_interval = 0;
    m_lasttick = 0;
  };

  CFTicker(int interval) 
  {
    m_interval = interval;
    m_lasttick = 0;
  };

  void SetTickInterval(unsigned int interval)
  {
    m_interval = interval;
  }

  unsigned int GetTickInterval() const
  {
    return m_interval;
  }

  bool HasTimeElapsed()
  {
    bool ret = false;
    unsigned int tick = CFSystem::GetTicks();
    if (tick - m_lasttick > m_interval) {
      ret = true;
      m_lasttick = tick;
    }
    return ret;
  }

private:

  unsigned int m_lasttick;
  unsigned int m_interval;

};

#endif  // __TICKER__H__