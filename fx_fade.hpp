#ifndef FX_FADE_HPP
#define FX_FADE_HPP

#include "fx.hpp"
#include "canvas.hpp"

class CFade: public FX {
public:
  CFade(Canvas * canvas, uint8_t target, uint8_t step):
    m_canvas(canvas),
    m_target(target),
    m_step(step),
    m_window_lowaddr(0),
    m_window_highaddr(canvas->Size() - 1)
  {}
  virtual void tick() {
    m_had_changes = false;
    for (int idx = m_window_lowaddr; idx <= m_window_highaddr; idx++) {
      int value = m_canvas->Get(idx);
      if (value == m_target) {
        continue;
      }

      m_had_changes = true;
      if (value < m_target) {
        value += m_step;
        if (value > m_target) {
          value = m_target;
        }
      } else {
        value -= m_step;
        if (value < m_target) {
          value = m_target;
        }
      }

      m_canvas->Set(idx, value);
    }
  }
  virtual bool done() {
    return !m_had_changes;
  }
  virtual void reset() {
    // Actually do nothing, we're fading while anything changes
  }
private:
  Canvas * m_canvas;
  int m_target;
  uint8_t m_step;
  int m_window_lowaddr;
  int m_window_highaddr;
  bool m_had_changes;
};

#endif  // FX_FADE_HPP
