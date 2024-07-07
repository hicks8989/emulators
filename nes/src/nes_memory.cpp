#include "nes.h"

namespace NES_Emulator {
  BYTE NES_Memory::read_mem(address_t address) {
    return memory[address];
  }

  void NES_Memory::write_mem(address_t address, BYTE val) {
    memory[address] = val;
  }
}
