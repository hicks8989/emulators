#include "nes_bus.h"

namespace NES_Emulator {
  NES_Bus::NES_Bus() {
    ppu = new NES_PPU();
  }

  BYTE NES_Bus::cpu_read(address_t address) {
    // CPU RAM addressing (with mirroring).
    if (address >= 0x0000 && address <= 0x1FFF)
      return cpu_ram[address & 0x07FF];
    // PPU Register addressing.
    else if (address == 0x2007)
      return ppu->read();
    // CPU read with mirrored down address.
    else if (address >= 0x2008 && address <= 0x7FFF)
      return cpu_read(address & 0x2007);
    // CPU read from cartridge ROM.
    else if (address >= 0x8000 && address <= 0xFFFF)
      return cartridge->read_prg_memory(address - 0x8000);
    return 0x00;
  }

  void NES_Bus::cpu_write(address_t address, BYTE val) {
    // CPU RAM addressing (with mirroring).
    if (address >= 0x0000 && address <= 0x1FFF)
      cpu_ram[address & 0x07FF] = val;
    // PPU Control register
    else if (address == 0x2000)
      ppu->write_to_control(val);
    // PPU address register.
    else if (address == 0x2006)
      ppu->write_to_ppu_addr(val);
    // PPU data register.
    else if (address = 0x2007)
      ppu->write_to_ppu_data(val);
  }

  void NES_Bus::insert_cartridge(NES_Cartridge* cartridge) {
    this->cartridge = cartridge;
  }
}
