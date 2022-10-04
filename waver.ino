#include <microLED.h>

const int STRIP_PIN = 2;

const int NUMLEDS = 144;
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;

uint8_t internal[NUMLEDS];
void setup() {
  Serial.begin(115200);
  strip.setBrightness(255);
  for (int idx = 0; idx < NUMLEDS; idx++) {
    internal[idx] = 0;
  }
}


class CWaver {
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
    void tick() {
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

CWaver waver1(internal, NUMLEDS, -16, 80, -16, 1, 10);
CWaver waver2(internal, NUMLEDS, -16, 80, 80, -1, 10);

void loop() {
  waver1.tick();
  waver2.tick();

  bool fake_brake = false;
  if (millis() % 7000 < 2000) {
    fake_brake = true;
  }
  for (int idx = 0; idx < NUMLEDS; idx++ ) {
    if (fake_brake) {
      strip.leds[idx] = mRGB(255, 0, 0);
    } else {
      strip.leds[idx] = mRGB(internal[idx], 0, 0);
    }
  }
  strip.show();

}
