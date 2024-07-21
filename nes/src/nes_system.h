#include "nes.h"
#include "nes_cpu.h"
#include "nes_ppu.h"
#include "nes_bus.h"

namespace NES_Emulator {
  class NES_System {
  private:
    // Cycles
    cycle_t cpu_cycles;
    cycle_t ppu_cycles;

    // Scanline
    BYTE scanline;

    // Processors
    NES_CPU* _cpu;
    NES_PPU* _ppu;

    // Memory
    NES_Bus* _bus;

  public:
    NES_System();
    void clock();
  };
}
