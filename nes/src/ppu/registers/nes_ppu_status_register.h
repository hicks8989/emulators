#include "nes.h"

namespace NES_Emulator {
  class NES_PPU_Status_Register {
  private:
    // Register
    BYTE status;
  
  public:
    // Flags enum
    enum class flag: BYTE {
      VBS = 0b10000000, // VBlank Started
      SZH = 0b01000000, // Sprite Zero Hit
      SOF = 0b00100000, // Sprite Overflow
      NU1 = 0b00010000, // Unused
      NU2 = 0b00001000, // Unused
      NU3 = 0b00000100, // Unused
      NU4 = 0b00000010, // Unused
      NU5 = 0b00000001, // Unused
    };

    NES_PPU_Status_Register();

    // Flags
    void set_flag(flag);
    bool get_flag(flag);
    
    // Register
    void set(BYTE v);
    BYTE get();
  };
}