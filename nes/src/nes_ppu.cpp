#include "nes_ppu.h"

namespace NES_Emulator {
  bool NES_PPU::get_flag(BYTE f) {
    return control & f;
  }

  void NES_PPU::set_flag(BYTE f, bool v) {
    if (v)
      control |= f;
    else
      control &= ~f;
  }

  address_t NES_PPU::mirror_vram_addr(address_t address) {
    address_t mirrored_address = address & 0x2FFF;
    address_t vram_index = mirrored_address - 0x2000;
    uint8_t name_table = vram_index / 0x0400;
    mirror_mode mirror = cartridge->get_mirror_mode();

    if (mirror == mirror_mode::VERTICAL && (name_table == 2 || name_table == 3)) {
      return vram_index - 0x0800;
    } else if (mirror == mirror_mode::HORIZONTAL && (name_table == 1 || name_table == 2)) {
      return vram_index - 0x0400;
    } else if (mirror == mirror_mode::HORIZONTAL && name_table == 3) {
      return vram_index - 0x0800;
    }

    return vram_index;
  }

  NES_PPU::NES_PPU() {
    this->addr = new NES_PPU_Address_Register();
  }

  void NES_PPU::increment_vram_addr() {
    addr->add(get_flag(VAI) ? 32 : 1);
  }

  void NES_PPU::write_to_control(BYTE v) {
    control = v;
  }

  void NES_PPU::write_to_ppu_addr(BYTE v) {
    addr->update(v);
  }

  void NES_PPU::write_to_ppu_data(BYTE v) {
    address_t address = addr->get();
    increment_vram_addr();
    
    if (address >= 0x2000 && address <= 0x2FFF)
      vram[mirror_vram_addr(address)] = v;
    else if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F1C)
      palette_table[address - 0x10 - 0x3F00] = v;
    else if (address <= 0x3000 && address >= 0x3FFF)
      palette_table[address - 0x3F00] = v;
  }

  BYTE NES_PPU::read() {
    address_t address = addr->get();
    BYTE result = internal_buf;
    increment_vram_addr();
    
    if (address >= 0x0000 && address <= 0x1FFF)
      internal_buf = cartridge->read_chr_memory(address);
    else if (address >= 0x2000 && address <= 0x2FFF)
      internal_buf = vram[mirror_vram_addr(address)];
    else if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F1C)
      internal_buf = palette_table[address - 0x10 - 0x3F00];
    else if (address <= 0x3000 && address >= 0x3FFF)
      internal_buf = palette_table[address - 0x3F00];

    return result;
  }

  void NES_PPU::insert_cartridge(NES_Cartridge* cartridge) {
    this->cartridge = cartridge;
  }
}
