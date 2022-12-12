#ifndef FX_PAUSE_HPP
#define FX_PAUSE_HPP

#include "fx.hpp"

class CPause: public FX {
public:
  CPause(int ticks): m_ticks(ticks){
    reset();
  };
  virtual void tick() {
    if (m_counter > 0) {
      m_counter --;
    }
  }
  virtual void reset() {
    m_counter = m_ticks;
  }
  virtual bool done() {
    return m_counter == 0;
  }

private:
  int m_ticks;
  int m_counter;
};

#endif  // FX_PAUSE_HPP
