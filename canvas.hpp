#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "strip.hpp"

class Canvas {
public:
  Canvas(uint8_t *storage, int size, CStrip* strip) :
    m_storage(storage),
    m_size(size),
    m_strip(strip)
  {}

  void Fill(int value) {
    value = correct_value(value);
    for (int idx = 0; idx < m_size; idx++) {
      m_storage[idx] = value;
    }
  }

  void Set(int index, int value) {
    if (index < 0 || index >= m_size) {
      return;
    }

    m_storage[index] = correct_value(value);
  }

  int Get(int index) {
    if (index < 0 || index >= m_size) {
      return 0;
    }
    return m_storage[index];
  }

  int Size() {
    return m_size;
  }

  void Render() {
    for(int idx = 0; idx < m_strip->Size(); idx++) {
      m_strip->Set(idx, m_storage[idx]);
    }
    m_strip->Show();
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
  CStrip *m_strip;
};

#endif  // CANVAS_HPP
