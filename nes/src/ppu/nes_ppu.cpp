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

  std::vector<uint8_t> NES_PPU::background_palette(BYTE tile_col, BYTE tile_row) {
    uint8_t attr_table_idx = tile_row / 4 * 8 +  tile_col / 4;
    BYTE attr_byte = vram[0x3C0 + attr_table_idx];
    BYTE palette_idx;

    switch ((tile_col % 4 / 2) << 1 | (tile_row % 4 / 2)) {
      case 0:
        palette_idx = attr_byte & 0b11;
        break;
      case 1:
        palette_idx = (attr_byte >> 2) & 0b11;
        break;
      case 2:
        palette_idx = (attr_byte >> 4) & 0b11;
        break;
      case 3:
        palette_idx = (attr_byte >> 6) & 0b11;
        break;
    }

    BYTE palette_start = 1 + (palette_idx * 4);
    return {
      palette_table[0], 
      palette_table[palette_start], 
      palette_table[palette_start + 1], 
      palette_table[palette_start + 2]
    };
  }

  std::vector<uint8_t> NES_PPU::sprite_palette(BYTE palette_idx) {
    BYTE start = 0x11 + (palette_idx * 4);
    return {
      0,
      palette_table[start],
      palette_table[start + 1],
      palette_table[start + 2]
    };
  }

  void NES_PPU::render(NES_Frame &frame) {
    // Render background
    address_t bank = control->get_background_pattern_addr();

    for (int i = 0; i < 0x03C0; i++) {
      address_t tile = vram[i];
      BYTE tile_x = tile % 32;
      BYTE tile_y = tile / 32;
      std::vector<uint8_t> palette = background_palette(tile_x, tile_y);
      
      for (int y = 0; y <= 7; y++) {
        BYTE upper = cartridge->read_chr_memory(y + (bank + tile * 16));
        BYTE lower = cartridge->read_chr_memory(y + 8 + (bank + tile * 16));

        for (int x = 7; x >= 0; x--) {
          BYTE val = (1 & upper) << 1 | (1 & lower);
          upper >>= 1;
          lower >>= 1;
          std::vector<uint8_t> rgb = SYSTEM_PALETTE[palette[val]];
          frame.set_pixel(tile_x * 8 + x, tile_y * 8 + y, rgb[0], rgb[1], rgb[2]);
        }
      }
    }

    // Render sprites
    for (int i = 252; i >= 0; i--) {
      address_t tile = oam_data[i + 1];
      BYTE tile_x = oam_data[i + 3];
      BYTE tile_y = oam_data[i];

      bool flip_vertical = (oam_data[i + 2] >> 7) & 1;
      bool flip_horizontal = (oam_data[i + 2] >> 6) & 1;
      BYTE flip = (oam_data[i + 2] >> 6) & 0b11;

      BYTE palette_idx = oam_data[i + 2] & 0b11;
      std::vector<uint8_t> palette = sprite_palette(palette_idx);

      bank = control->get_sprite_pattern_addr();

      for (int y = 0; y <= 7; y++) {
        BYTE upper = cartridge->read_chr_memory(y + (bank + tile * 16));
        BYTE lower = cartridge->read_chr_memory(y + 8 + (bank + tile * 16));

        for (int x = 7; x >= 0; x--) {
          BYTE val = (1 & upper) << 1 | (1 & lower);
          upper >>= 1;
          lower >>= 1;

          if (val == 0)
            continue;
          
          std::vector<uint8_t> rgb = SYSTEM_PALETTE[palette[val]];

          switch (flip) {
            case 0:
              frame.set_pixel(tile_x + x, tile_y + y, rgb[0], rgb[1], rgb[2]);
              break;
            case 1:
              frame.set_pixel(tile_x + 7 - x, tile_y + y, rgb[0], rgb[1], rgb[2]);
              break;
            case 2:
              frame.set_pixel(tile_x + x, tile_y + 7 - y, rgb[0], rgb[1], rgb[2]);
              break;
            case 3:
              frame.set_pixel(tile_x + 7 - x, tile_y + 7 - y, rgb[0], rgb[1], rgb[2]);
              break;
          }
        }
      }
    }
  }
}
