#include "strip.hpp"
#include "brake.hpp"
#include "canvas.hpp"
#include "fx.hpp"
#include "fx_chain.hpp"
#include "fx_front.hpp"
#include "fx_fade.hpp"

CStrip strip;
Brake brakes(&strip, BRAKE_PIN);
uint8_t storage[STRIP_NUMLEDS];
Canvas  canvas(storage, STRIP_NUMLEDS, &strip);
FXChain fx_chain;


void setup() {
  Serial.begin(115200);
  canvas.Fill(0);

  int size = canvas.Size();
  int middle = size / 2 - 1;


  fx_chain.append(new CFront(&canvas, 0, size - 1, 7, false, 3, 2, 0, 192));
//  fx_chain.append(new CFront(&canvas, -5, size - 6, 7, false, 3, -2, 0, 0));
  fx_chain.append(new CFront(&canvas, size - 1, 0, 7, false, 3, 2, 0, 192));
//  fx_chain.append(new CFront(&canvas, size + 4, 5, 7, false, 3, -2, 0, 0));
  fx_chain.append(new CFade(&canvas, 0, 1));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, 0, middle, 7, false, 3, 2, 0, 192));
//  fx_chain.append(new CFront(&canvas, -5, middle - 5, 7, false, 3, -2, 0, 0));
  fx_chain.append(new CFront(&canvas, size - 1, middle, 7, false, 3, 2, 0, 192));
//  fx_chain.append(new CFront(&canvas, size + 4, middle +5, 7, false, 3, -2, 0, 0));
  fx_chain.append(new CFade(&canvas, 0, 1));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, middle + 1, size - 1, 7, false, 5, 2, 0, 192));
  fx_chain.append(new CFront(&canvas, middle, 0, 7, false, 5, 2, 0, 192));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, middle + 1 - 2, size + 1, 7, false, 5, -2, 0, 0));
  fx_chain.append(new CFront(&canvas, middle + 2, -2, 7, false, 5, -2, 0, 0));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, middle + 1, size - 1, 7, false, 5, 2, 0, 64));
  fx_chain.append(new CFront(&canvas, middle, 0, 7, false, 5, 2, 0, 64));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, middle + 1, size - 1, 7, false, 5, 2, 0, 192));
  fx_chain.append(new CFront(&canvas, middle, 0, 7, false, 5, 2, 0, 192));
  fx_chain.append_next_chain();
  fx_chain.append(new CFront(&canvas, middle + 1 - 2, size + 1, 7, false, 5, -2, 0, 0));
  fx_chain.append(new CFront(&canvas, middle + 2, -2, 7, false, 5, -2, 0, 0));
//  fx_chain.append(new CFade(&canvas, 0, 1));
}

void loop() {
  fx_chain.tick();

  if (brakes.IsBraking()) {
    brakes.Render();
  } else {
    canvas.Render();
  }
}
