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

enum class Mode {
  Invert,
  KnightRider,
};

Mode mode_from_bool(bool val) {
  if (val) {
    return Mode::KnightRider;
  } else {
    return Mode::Invert;
  }
}

void mode_invert(B15F& drv, std::mutex& drv_mutex,
                 std::atomic<bool>& thread_allowed, std::atomic<Mode>& mode) {
  while (true) {
    if (mode != Mode::Invert || !thread_allowed) continue;
    std::cout << "Test\n";
    thread_allowed = false;
  }
}

void knight_rider(B15F& drv, std::mutex& drv_mutex,
                  std::atomic<bool>& thread_allowed, std::atomic<Mode>& mode) {
  uint8_t start = 0b00000001;
  size_t i = 0;

  while (true) {
    if (mode != Mode::KnightRider || !thread_allowed) continue;

    double shift = tri_wave(i, 8);
    uint8_t leds = start << (size_t)shift;
    // std::cout << std::bitset<8>(leds) << "\n";

    drv.digitalWrite0(leds);
    drv.digitalWrite1(~leds);

    i += 1;
    thread_allowed = false;
  }
}

int main() {
  B15F& drv = B15F::getInstance();
  std::mutex drv_mutex;
  std::cout << std::thread::hardware_concurrency() << "\n";

  std::atomic<Mode> mode = {Mode::KnightRider};
  // Not using a mutex here because its just too slow
  std::atomic<bool> thread_accessing = {false};
  std::thread thread_a([&drv, &drv_mutex, &thread_accessing, &mode]() {
    knight_rider(drv, drv_mutex, thread_accessing, mode);
  });
  std::thread thread_b([&drv, &drv_mutex, &thread_accessing, &mode]() {
    mode_invert(drv, drv_mutex, thread_accessing, mode);
  });
  thread_a.detach();
  thread_b.detach();

  while (true) {
    // emulate speed of b15 board on other machines
    B15F::delay_ms(50);

    if (thread_accessing) continue;
    const auto dip_switch = std::bitset<8>(drv.readDipSwitch());
    const auto new_mode = mode_from_bool(dip_switch[0]);
    const auto changed = new_mode != mode;
    mode = new_mode;
    thread_accessing = true;
  }
}
