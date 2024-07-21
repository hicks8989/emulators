#include "nes.h"

namespace NES_Emulator {
  class NES_Frame {
  private:
    std::vector<uint8_t> data;

    static const address_t WIDTH  = 256;
    static const address_t HEIGHT = 240;

  public:
    NES_Frame();

    // Pixel function
    void set_pixel(address_t, address_t, uint8_t, uint8_t, uint8_t);
  };
}