#include "b15f_stud.h"

#include <cstdint>

B15F::B15F() {}
B15F::~B15F() {}

B15F* B15F::instance = nullptr;
B15F& B15F::getInstance() {
  if (!instance) {
    instance = new B15F();
  }
  return *instance;
}

uint8_t B15F::readDipSwitch() { return 0b00000001; }

uint8_t B15F::digitalRead0() { return 0b00000000; }
uint8_t B15F::digitalRead1() { return 0b00000000; }

void B15F::digitalWrite0(uint8_t value) {
  // TODO
}
void B15F::digitalWrite1(uint8_t value) {
  // TODO
}
