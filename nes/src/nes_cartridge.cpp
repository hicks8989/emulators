#include "nes_cartridge.h"

namespace NES_Emulator {
  NES_Cartridge::NES_Cartridge(const std::string &file_name) {
    // iNES Format Header
    struct sHeader {
      char name[4];
      uint8_t prg_rom_chunks;
      uint8_t chr_rom_chunks;
      uint8_t mapper1;
      uint8_t mapper2;
      uint8_t prg_ram_size;
      uint8_t tv_system1;
      uint8_t tv_system2;
      char unused[5];
    } header;

    // Read file in ifstream
    std::ifstream ifs;
	  ifs.open(file_name, std::ifstream::binary);

    // Make sure file opened properly
    if (!ifs.is_open())
      return;
    
    // Read file header
		ifs.read((char*)&header, sizeof(sHeader));

		// If a "trainer" exists we just need to read past
		// it before we get to the good stuff
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// Determine Mapper ID
		mapper_id = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

		// "Discover" File Format
		uint8_t file_type = 1;

		if (file_type == 0) {

		}

		if (file_type == 1) {
			number_prg_banks = header.prg_rom_chunks;
			prg_memory.resize(number_prg_banks * 16384);
			ifs.read((char*)prg_memory.data(), prg_memory.size());

			number_chr_banks = header.chr_rom_chunks;
			chr_memory.resize(number_chr_banks * 8192);
			ifs.read((char*)chr_memory.data(), chr_memory.size());
		}

		if (file_type == 2) {

		}

		// Load appropriate mapper
		switch (mapper_id) {
		case 0: 
      mapper = new NES_Mapper(number_prg_banks, number_chr_banks); 
      break;
		}

		ifs.close();
  }
}
