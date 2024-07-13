#include "nes.h"

namespace NES_Emulator {
  class NES_Mapper {
  protected:
    uint8_t number_prg_banks;
    uint8_t number_chr_banks;

  public:
    NES_Mapper(uint8_t, uint8_t);
  };
}
