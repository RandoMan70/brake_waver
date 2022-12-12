#include "strip.hpp"
#include "brake.hpp"
#include "canvas.hpp"
#include "fx.hpp"
#include "fx_chain.hpp"
#include "fx_front.hpp"

CStrip strip;
Brake brakes(&strip, BRAKE_PIN);
uint8_t storage[STRIP_NUMLEDS];
Canvas  canvas(storage, STRIP_NUMLEDS, &strip);
CFront *front1, *front2, *front3, *front4;

void setup() {
  Serial.begin(115200);
  canvas.Fill(0);

  int middle = canvas.Size() / 2 - 1;
  front1 = new CFront(&canvas, middle + 1, canvas.Size() - 1, 7, false, 5, 2, 0, 96);
  front2 = new CFront(&canvas, middle, 0, 7, false, 5, 2, 0, 96);
  front1->fx_append(front2);
}

void loop() {
  if (front1->fx_chain_done()) {
    front1->set_max_brightness(192);
    front2->set_max_brightness(192);
    front1->fx_chain_reset();
    //canvas.Fill(0);
  }

  front1->tick();
  front2->tick();
  
  bool brake = digitalRead(BRAKE_PIN);
  if (brake) {
    brakes.Render();
  } else {
    canvas.Render();
  }
}
