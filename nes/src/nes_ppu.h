#include "nes.h"
#include "nes_cpu.h"
#include "nes_cartridge.h"
#include "nes_ppu_address_register.h"

namespace NES_Emulator {
  class NES_PPU {
    private:
      // flags
      static const BYTE NMI = 0b10000000; // Generate NMI
      static const BYTE MSS = 0b01000000; // Master Slave Select
      static const BYTE SSZ = 0b00100000; // Sprite Size
      static const BYTE BPA = 0b00010000; // Background Pattern Addr
      static const BYTE SPA = 0b00001000; // Sprite Pattern Addr
      static const BYTE VAI = 0b00000100; // VRAM Add Increment
      static const BYTE NT2 = 0b00000010; // Nametable2
      static const BYTE NT1 = 0b00000001; // Nametable1

      // Memory
      BYTE palette_table[32];
      BYTE vram[0xFFFF];
      BYTE oam_data[256];

      // Registers
      BYTE control;
      BYTE mask;
      BYTE status;
      BYTE oam_address;
      BYTE oam_data;
      BYTE scroll;
      NES_PPU_Address_Register* addr;

      // Internal Buffer
      BYTE internal_buf;

      // Cartridge
      NES_Cartridge* cartridge;

      // Flag helpers
      bool get_flag(BYTE);
      void set_flag(BYTE, bool);

      // VRAM Helpers
      address_t mirror_vram_addr(address_t);

    public:
      NES_PPU();

      // Read and Write
      BYTE read();
      void write(BYTE);

      // VRAM
      void increment_vram_addr();

      // Control register
      void write_to_control(BYTE);

      // Address and data registers
      void write_to_ppu_addr(BYTE);
      void write_to_ppu_data(BYTE);

      // Cartridge
      void insert_cartridge(NES_Cartridge*);
  };
}
