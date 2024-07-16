#include "nes_ppu_scroll_register.h"

namespace NES_Emulator {
  NES_PPU_Scroll_Register::NES_PPU_Scroll_Register() {
    scroll_x = 0;
    scroll_y = 0;
    latch = false;
  }

  void NES_PPU_Scroll_Register::write(BYTE val) {
    if (!latch)
      scroll_x = val;
    else
      scroll_y = val;
    
    latch = !latch;
  }

  void NES_PPU_Scroll_Register::reset_latch() {
    latch = false;
  }
}
