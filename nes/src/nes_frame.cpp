#include "nes_frame.h"

namespace NES_Emulator {
  NES_Frame::NES_Frame() {
    data = std::vector<uint8_t>((int)WIDTH * HEIGHT * 3, 0);
  }

  void NES_Frame::set_pixel(address_t x, address_t y, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t base = y * 3 * WIDTH + x * 3;

    if (base + 2 < data.size()) {
      data[base]     = r;
      data[base + 1] = g;
      data[base + 2] = b;
    }
  }
}