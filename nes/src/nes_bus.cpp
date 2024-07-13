#include "nes_bus.h"

namespace NES_Emulator {
  BYTE NES_Bus::cpu_read(address_t address) {
    // CPU RAM addressing (with mirroring).
    if (address >= 0x0000 && address <= 0x1FFF)
      return cpu_ram[address & 0x07FF];
    // PPU Register addressing (with mirroring).
    else if (address >= 0x2000 && address <= 0x3FFF)
      return cpu_ram[0x2000 + (address & 0x0008)];
    return 0x00;
  }

  void NES_Bus::cpu_write(address_t address, BYTE val) {
    // CPU RAM addressing (with mirroring).
    if (address >= 0x0000 && address <= 0x1FFF)
      cpu_ram[address & 0x07FF] = val;
    // PPU Register addressing (with mirroring).
    else if (address >= 0x2000 && address <= 0x3FFF)
      cpu_ram[0x2000 + (address & 0x0008)] = val;
  }

  void NES_Bus::insert_cartridge(const NES_Cartridge* cartridge) {
    this->cartridge = cartridge;
  }
}
