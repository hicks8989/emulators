#include "nes_system.h"

namespace NES_Emulator {
  NES_System::NES_System() {
    _bus = new NES_Bus();
    _cpu = new NES_CPU(_bus);
    _ppu = new NES_PPU(_bus);
  }

  void NES_System::clock() {
    if (cycles == 0) {
      opcode_t opcode = _bus->cpu_read(_cpu->PC()++);
      cycles = _cpu->run_instruction(opcode);
    }

    cycles--;
  }
}