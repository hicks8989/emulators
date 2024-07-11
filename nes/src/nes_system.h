#include "nes.h"
#include "nes_cpu.h"
#include "nes_bus.h"

namespace NES_Emulator {
  class NES_System {
  private:
    // Cycles
    cycle_t cycles;

    // Processors
    NES_CPU* _cpu;

    // Memory
    NES_Bus* _bus;

  public:
    NES_System() {
      _bus = new NES_Bus(); 
      _cpu = new NES_CPU(_bus); 
    };
    void clock();
  };
}
