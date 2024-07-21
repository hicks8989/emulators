#include "nes.h"

namespace NES_Emulator {
  class NES_PPU_Control_Register {
  private:
    // Register
    BYTE control;
  
  public:
    // Flags enum
    enum class flag: BYTE {
      NMI = 0b10000000, // Generate NMI
      MSS = 0b01000000, // Master Slave Select
      SSZ = 0b00100000, // Sprite Size
      BPA = 0b00010000, // Background Pattern Addr
      SPA = 0b00001000, // Sprite Pattern Addr
      VAI = 0b00000100, // VRAM Add Increment
      NT2 = 0b00000010, // Nametable2
      NT1 = 0b00000001, // Nametable1
    };

    NES_PPU_Control_Register();

    // Flags
    void set_flag(flag);
    bool get_flag(flag);
    
    // Register
    void set(BYTE v);
    BYTE get();

    // Flag getters
    address_t get_background_pattern_addr();
    address_t get_sprite_pattern_addr();

    // Helpers
    uint8_t get_vram_increment();
  };
}