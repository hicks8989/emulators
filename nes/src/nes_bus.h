#include "nes.h"
#include <unordered_map>
#include <unordered_set>

namespace NES_Emulator {
  class NES_Bus {
  private:
    // Memory
    BYTE memory[0xFFFF];

  public:
    BYTE read(address_t);
    void write(address_t, BYTE);
  };
}
