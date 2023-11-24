#include "b15f_stud.h"

#include <bitset>
#include <cstdint>
#include <iostream>
#include <thread>

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

uint8_t B15F::digitalRead0() { return in0; }
uint8_t B15F::digitalRead1() { return in1; }
void B15F::studSetIn0(uint8_t value) {
  std::cout << "in0: " << std::bitset<8>(value) << "\n";
  in0 = value;
}
void B15F::studSetIn1(uint8_t value) {
  std::cout << "in1: " << std::bitset<8>(value) << "\n";
  in0 = value;
}

void B15F::digitalWrite0(uint8_t value) {
  std::cout << "out0: " << std::bitset<8>(value) << "\n";
  out0 = value;
}
void B15F::digitalWrite1(uint8_t value) {
  std::cout << "out1: " << std::bitset<8>(value) << "\n";
  out1 = value;
}

void B15F::delay_ms(uint16_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
