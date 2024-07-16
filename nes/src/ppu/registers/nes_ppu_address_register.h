#include "nes.h"

namespace NES_Emulator {
  class NES_PPU_Address_Register {
  private:
    // Registers
    BYTE address;
    BYTE data;

    // Flags
    bool hi_ptr;
  
  public:
    NES_PPU_Address_Register();

    // Get and set address
    address_t get();
    void set(address_t);

    // Update functions
    void update(BYTE);
    void add(BYTE);

    // Flag functions
    void reset_latch();
  };
}