#include "nes.h"
#include "nes_cartridge.h"
#include "nes_ppu_address_register.h"
#include "nes_ppu_control_register.h"
#include "nes_frame.h"
#include "nes_palette.h"
#include "nes_ppu_mask_register.h"
#include "nes_ppu_scroll_register.h"
#include "nes_ppu_status_register.h"

namespace NES_Emulator {
  class NES_PPU {
    private:
      // Memory
      BYTE palette_table[32];
      BYTE vram[0xFFFF];
      BYTE oam_data[256];

      // Registers
      NES_PPU_Control_Register* control;
      NES_PPU_Mask_Register* mask;
      NES_PPU_Status_Register* status;
      BYTE oam_address;
      NES_PPU_Scroll_Register* scroll;
      NES_PPU_Address_Register* addr;

      // Internal Buffer
      BYTE internal_buf;

      // Cartridge
      NES_Cartridge* cartridge;

      // VRAM Helpers
      address_t mirror_vram_addr(address_t);

    public:
      NES_PPU();

      // Read
      BYTE read();

      // VRAM
      void increment_vram_addr();

      // Control register
      void write_to_control(BYTE);

      // Mask register
      void write_to_mask(BYTE);

      // Status register
      BYTE read_status();

      // OAM
      BYTE read_oam_data();
      void write_to_oam_addr(BYTE);
      void write_to_oam_data(BYTE);

      // Scroll register
      void write_to_scroll(BYTE);

      // Address and data registers
      void write_to_ppu_addr(BYTE);
      void write_to_ppu_data(BYTE);

      // Cartridge
      void insert_cartridge(NES_Cartridge*);

      // Palette
      std::vector<uint8_t> background_palette(BYTE, BYTE);
      std::vector<uint8_t> sprite_palette(BYTE);

      // Render
      void render(NES_Frame&);

  };
}
