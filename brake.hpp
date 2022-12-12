#ifndef BRAKE_HPP
#define BRAKE_HPP

#include <Arduino.h>
const int BRAKE_PIN = 5;

class Brake {
public:
  Brake(CStrip *strip, int brake_pin): m_strip(strip), m_brake_pin(brake_pin), m_instance(NULL) {
    pinMode(m_brake_pin, INPUT);
  };

  bool IsBraking() {
    return digitalRead(m_brake_pin);
  }

  void Render() {
    for(int idx = 0; idx < m_strip->Size(); idx++) {
      m_strip->Set(idx, 255);
    }
    m_strip->Show();
  }
private:
  CStrip *m_strip;
  int m_brake_pin;
  Brake *m_instance;
};

#endif  // BRAKE_HPP
