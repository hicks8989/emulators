#include "nes_ppu_control_register.h"

namespace NES_Emulator {
  NES_PPU_Control_Register::NES_PPU_Control_Register() {
    control = 0;
  }

  void NES_PPU_Control_Register::set_flag(flag f) {
    control |= (BYTE)f;
  }

  bool NES_PPU_Control_Register::get_flag(flag f) {
    if (control & (BYTE)f)
      return 1;
    else
      return 0;
  }

  void NES_PPU_Control_Register::set(BYTE v) {
    control = v;
  }

  BYTE NES_PPU_Control_Register::get() {
    return control;
  }

  uint8_t NES_PPU_Control_Register::get_vram_increment() {
    return get_flag(flag::VAI) ? 32 : 1;
  }
}
