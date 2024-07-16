#include "nes_ppu_status_register.h"

namespace NES_Emulator {
  NES_PPU_Status_Register::NES_PPU_Status_Register() {
    status = 0;
  }

  void NES_PPU_Status_Register::set_flag(flag f) {
    status |= (BYTE)f;
  }

  bool NES_PPU_Status_Register::get_flag(flag f) {
    if (status & (BYTE)f)
      return 1;
    else
      return 0;
  }

  void NES_PPU_Status_Register::set(BYTE v) {
    status = v;
  }

  BYTE NES_PPU_Status_Register::get() {
    return status;
  }
}
