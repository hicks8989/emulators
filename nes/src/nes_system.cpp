#include "nes_system.h"

namespace NES_Emulator {
  void NES_System::clock() {
    if (cycles == 0) {
      opcode_t opcode = _bus->read(_cpu->PC()++);
      cycles = _cpu->run_instruction(opcode);
    }

    cycles--;
  }
}