#include "nes_bus.h"

namespace NES_Emulator {
  BYTE NES_Bus::read(address_t address) {
    return memory[address];
  }

  void NES_Bus::write(address_t address, BYTE val) {
    memory[address] = val;
  }
}
