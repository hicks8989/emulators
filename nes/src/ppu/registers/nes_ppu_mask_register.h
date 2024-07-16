#include "nes.h"

namespace NES_Emulator {
  class NES_PPU_Mask_Register {
  private:
    // Register
    BYTE mask;
  
  public:
    // Flags enum
    enum class flag: BYTE {
      EB = 0b10000000, // Emphasize Blue
      EG = 0b01000000, // Emphasize Green
      ER = 0b00100000, // Emphasize Red
      SS = 0b00010000, // Show Sprite
      SB = 0b00001000, // Show Background
      LS = 0b00000100, // Leftmost Sprite
      LB = 0b00000010, // Leftmost Background
      GS = 0b00000001, // Grayscale
    };

    // Colors
    enum class color {
      RED,
      GREEN,
      BLUE
    };

    NES_PPU_Mask_Register();

    // Flags
    void set_flag(flag);
    bool get_flag(flag);
    
    // Register
    void set(BYTE v);
    BYTE get();
  };
}