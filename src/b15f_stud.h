#include <cstdint>

struct B15F {
  B15F();
  ~B15F();

  static B15F& getInstance();

  uint8_t readDipSwitch();
  void setDipSwitch(uint8_t value);

  uint8_t digitalRead0();
  uint8_t digitalRead1();
  void studSetIn0(uint8_t value);
  void studSetIn1(uint8_t value);

  void digitalWrite0(uint8_t value);
  void digitalWrite1(uint8_t value);

  static void delay_ms(uint16_t ms);

 private:
  static B15F* instance;

  uint8_t dipSwitch = 0;
  uint8_t in0 = 0;
  uint8_t in1 = 0;
  uint8_t out0 = 0;
  uint8_t out1 = 0;
};
