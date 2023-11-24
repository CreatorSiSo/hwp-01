#include <atomic>
#include <bitset>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>

#include "b15f_stud.h"
// #include "b15f/b15f.h"

// Generates a triangle wave, period is also the amplitude
size_t tri_wave(size_t i, uint32_t period) {
  if ((i / period) % 2) {
    return i % period;
  } else {
    return ((i % period) * -1) + period - 1;
  }
}

void mode_invert(std::atomic<bool>& thread_accessing,
                 std::atomic<bool>& thread_terminate) {
  B15F& drv = B15F::getInstance();
  while (!thread_terminate) {
    if (!thread_accessing) continue;
    std::cout << "Test\n";
    thread_accessing = false;
  }
}

void mode_knight_rider(std::atomic<bool>& thread_accessing,
                       std::atomic<bool>& thread_terminate) {
  B15F& drv = B15F::getInstance();
  uint8_t start = 0b00000001;
  size_t i = 0;

  while (!thread_terminate) {
    if (!thread_accessing) continue;

    double shift = tri_wave(i, 8);
    uint8_t leds = start << (size_t)shift;
    // std::cout << std::bitset<8>(leds) << "\n";

    drv.digitalWrite0(leds);
    drv.digitalWrite1(~leds);

    i += 1;
    thread_accessing = false;
  }
}

enum class Mode {
  Invert = 0,
  KnightRider = 1,
};

Mode mode_from_bool(bool val) {
  if (val) {
    return Mode::KnightRider;
  } else {
    return Mode::Invert;
  }
}

int main() {
  B15F& drv = B15F::getInstance();
  std::cout << std::thread::hardware_concurrency() << "\n";

  Mode mode = {Mode::KnightRider};
  // Not using a mutex because its just too slow
  // (on the b15f board)
  std::atomic<bool> thread_accessing = {false};
  std::atomic<bool> thread_terminate = {false};
  std::thread thread;

  drv.setDipSwitch(0b00000001);
  bool first = true;
  while (true) {
    // emulate speed of the board on other machines
    B15F::delay_ms(50);

    if (thread_accessing) continue;
    const auto dip_switch = std::bitset<8>(drv.readDipSwitch());
    const auto new_mode = mode_from_bool(dip_switch[0]);
    const auto changed = new_mode != mode;
    mode = new_mode;
    thread_accessing = true;

    if (first) {
      first = false;
    } else if (changed) {
      thread_terminate = true;
      thread.join();
      thread_terminate = false;
    } else {
      continue;
    }

    switch (mode) {
      case Mode::Invert:
        thread = std::thread([&thread_accessing, &thread_terminate]() {
          mode_invert(thread_accessing, thread_terminate);
        });
        break;
      case Mode::KnightRider:
        thread = std::thread([&thread_accessing, &thread_terminate]() {
          mode_knight_rider(thread_accessing, thread_terminate);
        });
        break;
    }
  }
}
