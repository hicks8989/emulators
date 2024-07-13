#include "nes.h"
#include "nes_cartridge.h"

namespace NES_Emulator {
  class NES_Bus {
  private:
    // Memory
    BYTE cpu_ram[0xFFFF];

    // Cartridge
    const NES_Cartridge* cartridge;

  public:
    // Read and Write
    BYTE cpu_read(address_t);
    void cpu_write(address_t, BYTE);

    BYTE ppu_read(address_t);
    void ppu_write(address_t, BYTE);

    // System interface
    void insert_cartridge(const NES_Cartridge*);
  };
}
