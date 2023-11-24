#include <cstdint>

struct B15F {
  B15F();
  ~B15F();

  static B15F& getInstance();

  uint8_t readDipSwitch();

  uint8_t digitalRead0();
  uint8_t digitalRead1();

  void digitalWrite0(uint8_t value);
  void digitalWrite1(uint8_t value);

 private:
  static B15F* instance;

  uint8_t in0 = 0;
  uint8_t in1 = 0;
  uint8_t out0 = 0;
  uint8_t out1 = 0;
};
