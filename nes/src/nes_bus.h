#include "nes.h"
#include "nes_cartridge.h"
#include "nes_ppu.h"

namespace NES_Emulator {
  class NES_Bus {
  private:
    // CPU Memory
    BYTE cpu_ram[0xFFFF];

    // PPU
    NES_PPU* ppu;

    // Cartridge
    NES_Cartridge* cartridge;

  public:
    NES_Bus();

    // CPU Read and Write
    BYTE cpu_read(address_t);
    void cpu_write(address_t, BYTE);

    // System interface
    void insert_cartridge(NES_Cartridge*);
  };
}
