#include "nes.h"

namespace NES_Emulator {
  class NES_PPU_Scroll_Register {
  public:
    // Scroll registers
    BYTE scroll_x;
    BYTE scroll_y;

    // Latch
    bool latch;

    NES_PPU_Scroll_Register();

    // Write
    void write(BYTE);

    // Reset Latch
    void reset_latch();

  };
}