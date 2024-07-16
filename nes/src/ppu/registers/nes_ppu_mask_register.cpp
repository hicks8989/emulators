#include "nes_ppu_mask_register.h"

namespace NES_Emulator {
  NES_PPU_Mask_Register::NES_PPU_Mask_Register() {
    mask = 0;
  }

  void NES_PPU_Mask_Register::set_flag(flag f) {
    mask |= (BYTE)f;
  }

  bool NES_PPU_Mask_Register::get_flag(flag f) {
    if (mask & (BYTE)f)
      return 1;
    else
      return 0;
  }

  void NES_PPU_Mask_Register::set(BYTE v) {
    mask = v;
  }

  BYTE NES_PPU_Mask_Register::get() {
    return mask;
  }
}
