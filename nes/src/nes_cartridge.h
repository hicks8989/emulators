#include "nes.h"
#include "nes_mapper.h"

namespace NES_Emulator {
  class NES_Cartridge {
  private:
    // Memory
    std::vector<BYTE> prg_memory;
    std::vector<BYTE> chr_memory;

    // Mapper metadata
    uint8_t mapper_id        = 0;
    uint8_t number_prg_banks = 0;
    uint8_t number_chr_banks = 0;

    // Mapper
    NES_Mapper* mapper;


  public:
    NES_Cartridge(const std::string&);
  };
}
