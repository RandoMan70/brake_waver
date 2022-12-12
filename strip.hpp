#ifndef STRIP_HPP
#define STRIP_HPP

const int STRIP_PIN = 3;
const int STRIP_NUMLEDS = 144;
const int STRIP_BRIGHTNESS = 255;

class CStrip {
public:
  CStrip();
  void Set(int index, unsigned char value);
  int Size();
  void Show();
};

#endif  // STRIP_HPP
