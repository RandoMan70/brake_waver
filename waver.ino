#include <microLED.h>

const int STRIP_PIN = 3;
const int BRAKE_PIN = 5;

const int NUMLEDS = 144;
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;

uint8_t internal[NUMLEDS];
void setup() {
  pinMode(BRAKE_PIN, INPUT);
  Serial.begin(115200);
  strip.setBrightness(255);
  for (int idx = 0; idx < NUMLEDS; idx++) {
    internal[idx] = 0;
  }
}

class FX {
public:
  virtual void tick();
  virtual bool done();
  virtual ~FX(){};  
};

class CWaver: public FX {
  public:
    CWaver(
      uint8_t *screen, 
      int size, 
      int window_start, 
      int window_end, 
      int position, 
      int direction, 
      int shiftdelay
    ): 
      m_screen(screen), 
      m_size(size), 
      m_window_start(window_start),
      m_window_end(window_end),
      m_position(position), 
      m_direction(direction), 
      m_shiftdelay(shiftdelay)
    {}
    virtual void tick() {
      if (--m_shift_downcount <= 0) {
        m_shift_downcount = m_shiftdelay;
        m_position += m_direction;
        if (m_position > m_window_end) {
          m_position = m_window_start;
        }
        if (m_position < m_window_start) {
          m_position = m_window_end;
        }
      }
      for (int idx = 0; idx < m_size; idx++) {
        if (m_screen[idx] > 32) {
          m_screen[idx] -= 1;
        } 
        if (m_screen[idx] < 32) {
          m_screen[idx] += 1;
        }
      }

      for (int idx = 0; idx < m_size; idx++) {
        int distance = abs(m_position - idx);
        if (distance > 5) {
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
    virtual bool done(){
      return false;
    }
  private:
    uint8_t *m_screen;
    int m_size;
    int m_window_start;
    int m_window_end;
    int m_position;
    int m_direction;
    int m_shiftdelay;
    int m_shift_downcount;
};

class CWider: public FX {
public:
  CWider(
      uint8_t *screen, 
      int size,
      int brightness,
      int shiftdelay
    ): m_screen(screen),
      m_size(size),
      m_brightness(brightness) ,
      m_pos(0),
      m_middle(size/2),
      m_shiftdelay(shiftdelay),
      m_shift_downcount(0)
    {}
  virtual void tick() {
    if (--m_shift_downcount < 0) {
      m_pos ++;
      m_shift_downcount = m_shiftdelay - 1;
    }

    bump(m_middle + m_pos);
    bump(m_middle - m_pos);
  }
  void bump(int idx) {
    if (m_shift_downcount == 0) {
      set(idx, m_brightness);
      return;
    }
    int current = get(idx);
    int delta = m_brightness - current;
    int step = delta / (m_shift_downcount + 1);
    set(idx, current + step);
  }
  void reset() {
    m_pos = 0;
  }
  virtual bool done() {
    return m_pos > (m_size / 2);
  }
private:
  void set(int idx, int val) {
    if (idx < 0 || idx >= m_size) {
      return;
    }
    if (m_screen[idx] < val) {
      m_screen[idx] = val;
    }
  }
  int get(int idx) {
    if (idx < 0 || idx >= m_size) {
      return 0;
    }
    return m_screen[idx];
  }
  uint8_t *m_screen;
  int m_size;
  int m_brightness;
  int m_pos;
  int m_middle;
  int m_shiftdelay;
  int m_shift_downcount;
  int m_brightness_at_start;
};

CWaver waver1(internal, NUMLEDS, -16, 144+16, -16, 1, 10);
CWaver waver2(internal, NUMLEDS, -16, 144+16, 144+16, -1, 10);
CWider wider1(internal, NUMLEDS, 64, 10);
CWider wider2(internal, NUMLEDS, 192, 10);

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

  for (int idx = 0; idx < NUMLEDS; idx++ ) {
    if (brake) {
      strip.leds[idx] = mRGB(255, 0, 0);
    } else {
      strip.leds[idx] = mRGB(internal[idx], 0, 0);
    }
  }
  strip.show();
}
