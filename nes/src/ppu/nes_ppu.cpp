#include "nes_ppu.h"

namespace NES_Emulator {
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
    control = new NES_PPU_Control_Register();
    mask = new NES_PPU_Mask_Register();
    status = new NES_PPU_Status_Register();
    scroll = new NES_PPU_Scroll_Register();
    addr = new NES_PPU_Address_Register();
  }

  void NES_PPU::increment_vram_addr() {
    addr->add(control->get_vram_increment());
  }

  void NES_PPU::write_to_control(BYTE v) {
    control->set(v);
  }

  void NES_PPU::write_to_mask(BYTE v) {
    mask->set(v);
  }

  BYTE NES_PPU::read_status() {
    status->get();
  }

  BYTE NES_PPU::read_oam_data() {
    return oam_data[oam_address];
  }

  void NES_PPU::write_to_oam_addr(BYTE val) {
    oam_address = val;
  }

  void NES_PPU::write_to_oam_data(BYTE val) {
    oam_data[oam_address] = val;
    oam_address = (int)oam_address + 1;
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
