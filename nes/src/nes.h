#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace NES_Emulator {
  typedef unsigned char BYTE;
  typedef unsigned short int WORD;
  typedef unsigned char page_t;
  typedef unsigned char cycle_t;
  typedef unsigned char opcode_t;
  typedef unsigned short int operand_t;
  typedef unsigned short int address_t;
  typedef cycle_t (*instruction(opcode_t));
}
