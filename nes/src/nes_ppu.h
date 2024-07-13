#include "nes.h"
#include "nes_cpu.h"
#include "nes_bus.h"

namespace NES_Emulator {
  class NES_PPU {
    private:
      NES_Bus* bus;

    public:
      NES_PPU(NES_Bus*);
  };
}
