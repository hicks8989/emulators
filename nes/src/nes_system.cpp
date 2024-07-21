#include "nes_system.h"

namespace NES_Emulator {
  NES_System::NES_System() {
    _bus = new NES_Bus();
    _cpu = new NES_CPU(_bus);
    _ppu = new NES_PPU();
  }

  void NES_System::clock() {
    if (ppu_cycles >= 341) {
      ppu_cycles -= 341;
      scanline += 1;

      if (scanline >= 262) {
        scanline = 0;
      }
    }

    if (cpu_cycles == 0) {
      opcode_t opcode = _bus->cpu_read(_cpu->PC()++);
      cpu_cycles = _cpu->run_instruction(opcode);
    }

    if (ppu_cycles % 3 == 0)
      cpu_cycles--;
    
    ppu_cycles++;
  }
}