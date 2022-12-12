#ifndef FX_FRONT_HPP
#define FX_FRONT_HPP

#include "fx.hpp"
#include "canvas.hpp"

class CFront: public FX {
public:
  CFront(
      Canvas *canvas,
      int start_position,
      int end_position,
      int step_ticks,
      bool cyclic,
      int front_width,
      int front_fade_scaling_factor,
      int min_brightness,
      int max_brightness
    ):
    m_canvas(canvas),
    m_start_position(start_position),
    m_end_position(end_position),
    m_step_ticks(step_ticks),
    m_cyclic(cyclic),
    m_front_width(front_width),
    m_front_fade_scaling_factor(front_fade_scaling_factor),
    m_direction(m_end_position > m_start_position ? 1 : m_start_position == m_end_position ? 0 : -1),
    m_min_brightness(min_brightness),
    m_max_brightness(max_brightness)
  {
    reset();
  }

  virtual void tick() {
    tick_updown();

    if (--m_step_tick_left < 0) {
      if (m_position == m_end_position) {
        if (m_cyclic) {
          m_position = m_start_position;
        }
      } else {
        m_position += m_direction;
      }
      m_step_tick_left = m_step_ticks - 1;
    }
  }

  virtual void reset() {
    m_position = m_start_position;
    m_step_tick_left = m_step_ticks;
  }

  void set_max_brightness(int brightness) {
    m_max_brightness = brightness;
  }

  void set_min_brightness(int brightness) {
    m_min_brightness = brightness;
  }

  virtual bool done() {
    if (m_cyclic) {
      return false;
    } else {
      return m_position == m_end_position;
    }
  }

private:
  void tick_updown() {
    int offset = m_front_fade_scaling_factor > 0 ? 0 : m_front_width;
    for (int idx = 0; idx < m_front_width; idx++) {
      int index = m_direction * (idx - offset);
      int shift = (m_front_width - idx) * m_front_fade_scaling_factor;
      int value = relative_get(index);
      value += shift;
      relative_set(index, value);
    }
  }

  int relative_get(int offset) {
    return m_canvas->Get(m_position + offset);
  }

  void relative_set(int offset, int value) {
      if (m_front_fade_scaling_factor > 0 && value > m_max_brightness) {
        value = m_max_brightness;
      }
      if (m_front_fade_scaling_factor < 0 && value < m_min_brightness) {
        value = m_min_brightness;
      }

    m_canvas->Set(m_position + offset, value);
  }

// Configurational variables
  Canvas *m_canvas;
  int m_start_position;
  int m_end_position;
  int m_step_ticks;
  bool m_cyclic;
  int m_front_width;
  int m_front_fade_scaling_factor;
  int m_min_brightness;
  int m_max_brightness;

// Automatic configuration variables
  int m_direction;

// Runtime variables
  int m_step_tick_left;
  int m_position;
};
#endif  // FX_FRONT_HPP
