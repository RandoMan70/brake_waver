#include <microLED.h>

const int STRIP_PIN = 3;
const int BRAKE_PIN = 5;

const int NUMLEDS = 144;
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> STRIP;
class RedStripInterface{
public:
  Set(uint8_t value) {
    STRIP.leds[idx] = mRGB(value, 0, 0);
  }
  int Size() {
    return NUMLEDS;
  }
}
RedStripInterface strip;

class Canvas {
public:
  Canvas(uint8_t *storage, int size, RedStripInterface* strip) :
    m_storage(storage),
    m_size(size),
    m_strip(strip)
  {}

  void Fill(int value) {
    value = correct_value(value);
    for (int idx = 0; idx < m_size) {
      m_storage[idx] = value;
    }
  }

  void Set(int index, int value) {
    if (index < 0 || index >= m_size) {
      return;
    }

    m_canvas[idx] = correct_value(value);
  }

  int Get(int index) {
    if (index < 0 || index >= m_size) {
      return 0;
    }
    return m_storage[idx];
  }

  int Size() {
    return m_size;
  }

  Render() {
    for(idx = 0; idx < m_strip.Size(); idx++) {
      m_strip.Set(m_storage[idx]);
    }
    m_strip.show();
  }
private:
  uint8_t correct_value(int value) {
    if (value < 0) {
      value = 0;
    }
    if (value > 255) {
      value = 255;
    }

    return value;
  }
  uint8_t *m_storage;
  int m_size;
  RedStripInterface *m_strip;
}

class FX {
public:
  FX() m_fx_next(NULL){};
  virtual void tick();
  virtual bool done();
  virtual ~FX(){};

  void fx_set_next(FX *next) {
    m_fx_next = next;
  }

  bool fx_chain_done() {
    return self.done() || (m_fx_next != NULL ? m_fx_next->done() : false);
  }
protected:
  FX *fx_next() {
    return m_fx_next;
  }
private:
  FX *m_fx_next;
};

class CFade: public FX {
public:
  CFade(Canvas * canvas, uint8_t target, uint8_t step):
    m_canvas(canvas),
    m_target(target),
    m_step(step),
    m_window_lowaddr(0),
    m_window_highaddr(canvas.Size() - 1)
  {}
  virtual void tick() {
    m_had_changes = false;
    for (int idx = m_window_lowaddr, idx <= m_window_highaddr, idx++) {
      int value = m_canvas.Get(idx);
      if (value == m_target) {
        continue
      }

      m_had_changes = true;
      if (value < target) {
        value += m_step;
        if (value > target) {
          value = target;
        }
      } else {
        value -= m_step;
        if (value < target) {
          value = target;
        }
      }

      m_canvas.Set(idx, value);
    }
  }
  virtual bool done() {
    return !m_had_changes;
  }
private:
  Canvas * m_canvas;
  int m_target;
  uint8_t m_step;
  int m_window_lowaddr;
  int m_window_highaddr;
  bool m_had_changes;
}

class CFront: public FX {
public:
  CFront(
      Canvas *canvas,
      int start_position,
      int end_position,
      int step_ticks,
      bool cyclic,
      int front_width,
      int front_fade_scaling_factor
    ):
    m_canvas(canvas),
    m_start_position(start_position),
    m_end_position(end_position),
    m_step_ticks(step_ticks),
    m_cyclic(cyclic),
    m_front_width(front_width),
    m_front_fade_scaling_factor(front_fade_scaling_factor),
    m_direction(m_end_position > m_start_position ? 1 : m_start_position == m_end_position ? 0 : -1)
  {}

  virtual void tick() {
    if (--m_shift_downcount < 0) {
      m_position ++;
      m_step_tick_left = step_ticks - 1;
    }

    for (int idx = 0; idx < canvas.Size(); idx++) {
      int distance = abs(m_position - idx);
      if (distance > ) {
        continue;
      }
      int shift = (5-distance) * 10;
      int newval = m_screen[idx];
      newval += shift;
      if (newval > 255) {
        newval = 255;
      }
      m_screen[idx] = newval;
    }
  }

  void tick_up() {
    for (int idx = 0; idx < m_front_width; idx++) {
      int shift = (m_front_width - idx) * 10;
      int value = canvas.Get(m_position + idx);
      value += shift;
      tick_set(m_position + idx, value);
    }
  }

  void tick_down() {
    for (int idx = 0; idx < m_front_width; idx++) {
      int shift = idx * 10;
      int value = canvas.Get(m_position - idx);
      value += shift;
      tick_set(m_position - idx, value);
    }
  }

  void tick_set(int idx, int value) {
    if (value > 255) {
      value = 255;
    }
    canvas.Set(idx, value);
  }

  void reset() {
    m_position = m_start_position;
  }

  virtual bool done() {
    if (cyclic) {
      return false;
    } else {
      return m_position == m_end_position;
    }
  }
private:
// Configurational variables
  Canvas *m_canvas;
  int m_start_position;
  int m_end_position;
  int m_step_ticks;
  bool m_cyclic;
  int m_front_width;
  int m_front_fade_scaling_factor;

// Automatic configuration variables
  int m_direction;

// Runtime variables
  int m_step_tick_left;
  int m_position;
};

uint8_t storage[NUMLEDS];
Canvas  canvas(&storage, NUMLEDS, &strip);
void setup() {
  pinMode(BRAKE_PIN, INPUT);
  Serial.begin(115200);
  STRIP.setBrightness(255);
  canvas.Fill(0);
}
// CWaver waver1(internal, NUMLEDS, -16, 144+16, -16, 1, 10);
// CWaver waver2(internal, NUMLEDS, -16, 144+16, 144+16, -1, 10);
// CWider wider1(internal, NUMLEDS, 64, 10);
// CWider wider2(internal, NUMLEDS, 192, 10);

class BrakePattern {
public:
  BrakePattern(RedStripInterface *strip): m_strip(strip) {};
  Render() {
    for(idx = 0; idx < m_strip.Size(); idx++) {
      m_strip.Set(255);
    }
    m_strip.show();
  }
private:
  RedStripInterface *m_strip;
}

BrakePattern brakes(&strip);
void loop() {
  if (!wider1.done()) {
    wider1.tick();
  } else if (!wider2.done()){
    wider2.tick();
  } else {
    waver1.tick();
    waver2.tick();
  }

  bool brake = digitalRead(BRAKE_PIN);
  if (brake) {
    brakes.Render();
  } else {
    canvas.Render();
  }
}
