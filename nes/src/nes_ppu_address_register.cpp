#include "nes_ppu_address_register.h"

namespace NES_Emulator {
  NES_PPU_Address_Register::NES_PPU_Address_Register() {
    address = 0; 
    data = 0;
    hi_ptr = true;
  }

  address_t NES_PPU_Address_Register::get() {
    return ((address_t)data << 8) | address;
  }

  void NES_PPU_Address_Register::set(address_t value) {
    data = (value >> 8) & 0xFF;
    address = value & 0xFF;
  }

  void NES_PPU_Address_Register::update(BYTE value) {
    if (hi_ptr)
      data = value;
    else
      address = value;

    address_t addr = this->get();
    if (addr > 0x3FFF)
      this->set(addr & 0x3FFF);
    
    hi_ptr = !hi_ptr;
  }

  void NES_PPU_Address_Register::add(BYTE value) {
    BYTE lo = address;
    address = ((int)address + value) & 0xFF;

    if (lo > address)
      data = ((int)data + 1) & 0xFF;
    
    address_t addr = this->get();
    if (addr > 0x3FFF)
      this->set(addr & 0x3FFF);
  }

  void NES_PPU_Address_Register::reset_latch() {
    hi_ptr = true;
  }
}
