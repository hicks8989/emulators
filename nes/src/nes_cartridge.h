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

    // Mirroring
    mirror_mode mirror;

  public:
    NES_Cartridge(const std::string&);

    // Read ROM
    BYTE read_prg_memory(address_t);
    BYTE read_chr_memory(address_t);

    // Mirror mode
    mirror_mode get_mirror_mode();
  };
}
