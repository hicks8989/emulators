#include "nes_mapper.h"

namespace NES_Emulator {
  NES_Mapper::NES_Mapper(uint8_t number_prg_banks, uint8_t number_chr_banks) {
    this->number_prg_banks = number_prg_banks;
    this->number_chr_banks = number_chr_banks;
  }
}