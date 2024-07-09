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

  // Operation Cycles
  static const std::unordered_map<opcode_t, cycle_t> BASE_OPERATION_CYCLES = {
    {0x69, 2}, {0x65, 3}, {0x75, 4}, {0x6D, 4}, {0x7D, 4}, {0x79, 4}, {0x61, 6}, {0x71, 5}, // ADC
    {0x29, 2}, {0x25, 3}, {0x35, 4}, {0x2D, 4}, {0x3D, 4}, {0x39, 4}, {0x21, 6}, {0x31, 5}, // AND
    {0x0A, 2}, {0x06, 5}, {0x16, 6}, {0x0E, 6}, {0x1E, 7},                                  // ASL
    {0x90, 2},                                                                              // BCC
    {0xB0, 2},                                                                              // BCS
    {0xF0, 2},                                                                              // BEQ
    {0x24, 3}, {0x2C, 4},                                                                   // BIT
    {0x31, 2},                                                                              // BMI
    {0xD0, 2},                                                                              // BNE
    {0x10, 2},                                                                              // BPL
    {0x00, 7},                                                                              // BRK
    {0x50, 2},                                                                              // BVC
    {0x70, 2},                                                                              // BVS
    {0x18, 2},                                                                              // CLC
    {0xD8, 2},                                                                              // CLD
    {0x58, 2},                                                                              // CLI
    {0xB8, 2},                                                                              // CLV
    {0xC9, 2}, {0xC5, 3}, {0xD5, 4}, {0xCD, 4}, {0xDD, 4}, {0xD9, 4}, {0xC1, 6}, {0xD1, 5}, // CMP
    {0xE0, 2}, {0xE4, 3}, {0xEC, 5},                                                        // CPX
    {0xC0, 2}, {0xC4, 3}, {0xCC, 5},                                                        // CPY
    {0xC6, 5}, {0xD6, 6}, {0xCE, 6}, {0xDE, 7},                                             // DEC
    {0xCA, 2},                                                                              // DEX
    {0x88, 2},                                                                              // DEY
    {0x49, 2}, {0x45, 3}, {0x55, 4}, {0x4D, 4}, {0x5D, 4}, {0x59, 4}, {0x41, 6}, {0x51, 5}, // EOR
    {0xE6, 5}, {0xF6, 6}, {0xEE, 6}, {0xFE, 7},                                             // INC
    {0xE8, 2},                                                                              // INX
    {0xC8, 2},                                                                              // INY
    {0x4C, 3}, {0x6C, 5},                                                                   // JMP
    {0x20, 6},                                                                              // JSR
    {0xA9, 2}, {0xA5, 3}, {0xB5, 4}, {0xAD, 4}, {0xBD, 4}, {0xB9, 4}, {0xA1, 6}, {0xB1, 5}, // LDA
    {0xA2, 2}, {0xA6, 3}, {0xB6, 4}, {0xAE, 4}, {0xBE, 4},                                  // LDX
    {0xA0, 2}, {0xA4, 3}, {0xB4, 4}, {0xAC, 4}, {0xBC, 4},                                  // LDY
    {0x4A, 2}, {0x46, 5}, {0x56, 6}, {0x4E, 6}, {0x5E, 7},                                  // LSR
    {0xEA, 2},                                                                              // NOP
    {0x09, 2}, {0x05, 3}, {0x15, 4}, {0x0D, 4}, {0x1D, 4}, {0x19, 4}, {0x01, 6}, {0x11, 5}, // ORA
    {0x48, 3},                                                                              // PHA
    {0x08, 3},                                                                              // PHP
    {0x68, 4},                                                                              // PLA
    {0x28, 4},                                                                              // PLP
    {0x2A, 2}, {0x26, 5}, {0x36, 6}, {0x2E, 6}, {0x3E, 7},                                  // ROL
    {0x6A, 2}, {0x66, 5}, {0x76, 6}, {0x6E, 6}, {0x7E, 7},                                  // ROR
    {0x40, 6},                                                                              // RTI
    {0x60, 6},                                                                              // RTS
    {0xE9, 2}, {0xE5, 3}, {0xF5, 4}, {0xED, 4}, {0xFD, 4}, {0xF9, 4}, {0xE1, 6}, {0xF1, 5}, // SBC
    {0x38, 2},                                                                              // SED
    {0x78, 2},                                                                              // SEI
    {0x85, 3}, {0x95, 4}, {0x8D, 4}, {0x9D, 5}, {0x99, 5}, {0x81, 6}, {0x91, 6},            // STA
    {0x86, 3}, {0x96, 4}, {0x8E, 4},                                                        // STX
    {0x84, 3}, {0x94, 4}, {0x8C, 4},                                                        // STY
    {0xAA, 2},                                                                              // TAX
    {0xA8, 2},                                                                              // TAY
    {0xBA, 2},                                                                              // TSX
    {0x8A, 2},                                                                              // TXA
    {0x9A, 2},                                                                              // TXS
    {0x98, 2},                                                                              // TYA
  };

  static const std::unordered_map<opcode_t, nes_addr_mode> OPERATION_ADDRESS_MODES = {
    {0x69, nes_addr_mode::nes_addr_mode_imm}, {0x65, nes_addr_mode::nes_addr_mode_zp}, 
    {0x75, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x6D, nes_addr_mode::nes_addr_mode_abs}, 
    {0x7D, nes_addr_mode::nes_addr_mode_abs_x}, {0x79, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0x61, nes_addr_mode::nes_addr_mode_ind_x}, {0x71, nes_addr_mode::nes_addr_mode_ind_y}, // ADC
    {0x29, nes_addr_mode::nes_addr_mode_imm}, {0x25, nes_addr_mode::nes_addr_mode_zp}, 
    {0x35, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x2D, nes_addr_mode::nes_addr_mode_abs}, 
    {0x3D, nes_addr_mode::nes_addr_mode_abs_x}, {0x39, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0x21, nes_addr_mode::nes_addr_mode_ind_x}, {0x31, nes_addr_mode::nes_addr_mode_ind_y}, // AND
    {0x0A, nes_addr_mode::nes_addr_mode_acc}, {0x06, nes_addr_mode::nes_addr_mode_zp}, 
    {0x16, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x0E, nes_addr_mode::nes_addr_mode_abs}, 
    {0x1E, nes_addr_mode::nes_addr_mode_abs_x},                                             // ASL
    {0x90, nes_addr_mode::nes_addr_mode_rel},                                               // BCC
    {0xB0, nes_addr_mode::nes_addr_mode_rel},                                               // BCS
    {0xF0, nes_addr_mode::nes_addr_mode_rel},                                               // BEQ
    {0x24, nes_addr_mode::nes_addr_mode_zp}, {0x2C, nes_addr_mode::nes_addr_mode_abs},      // BIT
    {0x31, nes_addr_mode::nes_addr_mode_rel},                                               // BMI
    {0xD0, nes_addr_mode::nes_addr_mode_rel},                                               // BNE
    {0x10, nes_addr_mode::nes_addr_mode_rel},                                               // BPL
    {0x00, nes_addr_mode::nes_addr_mode_imp},                                               // BRK
    {0x50, nes_addr_mode::nes_addr_mode_rel},                                               // BVC
    {0x70, nes_addr_mode::nes_addr_mode_rel},                                               // BVS
    {0x18, nes_addr_mode::nes_addr_mode_imp},                                               // CLC
    {0xD8, nes_addr_mode::nes_addr_mode_imp},                                               // CLD
    {0x58, nes_addr_mode::nes_addr_mode_imp},                                               // CLI
    {0xB8, nes_addr_mode::nes_addr_mode_imp},                                               // CLV
    {0xC9, nes_addr_mode::nes_addr_mode_imm}, {0xC5, nes_addr_mode::nes_addr_mode_zp}, 
    {0xD5, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0xCD, nes_addr_mode::nes_addr_mode_abs}, 
    {0xDD, nes_addr_mode::nes_addr_mode_abs_x}, {0xD9, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0xC1, nes_addr_mode::nes_addr_mode_ind_x}, {0xD1, nes_addr_mode::nes_addr_mode_ind_y}, // CMP
    {0xE0, nes_addr_mode::nes_addr_mode_imm}, {0xE4, nes_addr_mode::nes_addr_mode_zp}, 
    {0xEC, nes_addr_mode::nes_addr_mode_abs},                                               // CPX
    {0xC0, nes_addr_mode::nes_addr_mode_imm}, {0xC4, nes_addr_mode::nes_addr_mode_zp}, 
    {0xCC, nes_addr_mode::nes_addr_mode_abs},                                               // CPY
    {0xC6, nes_addr_mode::nes_addr_mode_zp}, {0xD6, nes_addr_mode::nes_addr_mode_zp_ind_x}, 
    {0xCE, nes_addr_mode::nes_addr_mode_abs}, {0xDE, nes_addr_mode::nes_addr_mode_abs_x},   // DEC
    {0xCA, nes_addr_mode::nes_addr_mode_imp},                                               // DEX
    {0x88, nes_addr_mode::nes_addr_mode_imp},                                               // DEY
    {0x49, nes_addr_mode::nes_addr_mode_imm}, {0x45, nes_addr_mode::nes_addr_mode_zp}, 
    {0x55, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x4D, nes_addr_mode::nes_addr_mode_abs}, 
    {0x5D, nes_addr_mode::nes_addr_mode_abs_x}, {0x59, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0x41, nes_addr_mode::nes_addr_mode_ind_x}, {0x51, nes_addr_mode::nes_addr_mode_ind_y}, // EOR
    {0xE6, nes_addr_mode::nes_addr_mode_zp}, {0xF6, nes_addr_mode::nes_addr_mode_zp_ind_x}, 
    {0xEE, nes_addr_mode::nes_addr_mode_abs}, {0xFE, nes_addr_mode::nes_addr_mode_abs_x},   // INC
    {0xE8, nes_addr_mode::nes_addr_mode_imp},                                               // INX
    {0xC8, nes_addr_mode::nes_addr_mode_imp},                                               // INY
    {0x4C, nes_addr_mode::nes_addr_mode_abs}, {0x6C, nes_addr_mode::nes_addr_mode_ind_jmp}, // JMP
    {0x20, nes_addr_mode::nes_addr_mode_abs},                                               // JSR
    {0xA9, nes_addr_mode::nes_addr_mode_imm}, {0xA5, nes_addr_mode::nes_addr_mode_zp}, 
    {0xB5, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0xAD, nes_addr_mode::nes_addr_mode_abs}, 
    {0xBD, nes_addr_mode::nes_addr_mode_abs_x}, {0xB9, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0xA1, nes_addr_mode::nes_addr_mode_ind_x}, {0xB1, nes_addr_mode::nes_addr_mode_ind_y}, // LDA
    {0xA2, nes_addr_mode::nes_addr_mode_imm}, {0xA6, nes_addr_mode::nes_addr_mode_zp}, 
    {0xB6, nes_addr_mode::nes_addr_mode_zp_ind_y}, {0xAE, nes_addr_mode::nes_addr_mode_abs}, 
    {0xBE, nes_addr_mode::nes_addr_mode_abs_y},                                             // LDX
    {0xA0, nes_addr_mode::nes_addr_mode_imm}, {0xA4, nes_addr_mode::nes_addr_mode_zp}, 
    {0xB4, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0xAC, nes_addr_mode::nes_addr_mode_abs}, 
    {0xBC, nes_addr_mode::nes_addr_mode_abs_x},                                             // LDY
    {0x4A, nes_addr_mode::nes_addr_mode_acc}, {0x46, nes_addr_mode::nes_addr_mode_zp}, 
    {0x56, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x4E, nes_addr_mode::nes_addr_mode_abs}, 
    {0x5E, nes_addr_mode::nes_addr_mode_abs_x},                                             // LSR
    {0xEA, nes_addr_mode::nes_addr_mode_imp},                                               // NOP
    {0x09, nes_addr_mode::nes_addr_mode_imm}, {0x05, nes_addr_mode::nes_addr_mode_zp}, 
    {0x15, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x0D, nes_addr_mode::nes_addr_mode_abs}, 
    {0x1D, nes_addr_mode::nes_addr_mode_abs_x}, {0x19, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0x01, nes_addr_mode::nes_addr_mode_ind_x}, {0x11, nes_addr_mode::nes_addr_mode_ind_y}, // ORA
    {0x48, nes_addr_mode::nes_addr_mode_imp},                                               // PHA
    {0x08, nes_addr_mode::nes_addr_mode_imp},                                               // PHP
    {0x68, nes_addr_mode::nes_addr_mode_imp},                                               // PLA
    {0x28, nes_addr_mode::nes_addr_mode_imp},                                               // PLP
    {0x2A, nes_addr_mode::nes_addr_mode_acc}, {0x26, nes_addr_mode::nes_addr_mode_zp}, 
    {0x36, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x2E, nes_addr_mode::nes_addr_mode_abs}, 
    {0x3E, nes_addr_mode::nes_addr_mode_abs_x},                                             // ROL
    {0x6A, nes_addr_mode::nes_addr_mode_acc}, {0x66, nes_addr_mode::nes_addr_mode_zp}, 
    {0x76, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0x6E, nes_addr_mode::nes_addr_mode_abs}, 
    {0x7E, nes_addr_mode::nes_addr_mode_abs_x},                                             // ROR
    {0x40, nes_addr_mode::nes_addr_mode_imp},                                               // RTI
    {0x60, nes_addr_mode::nes_addr_mode_imp},                                               // RTS
    {0xE9, nes_addr_mode::nes_addr_mode_imm}, {0xE5, nes_addr_mode::nes_addr_mode_zp}, 
    {0xF5, nes_addr_mode::nes_addr_mode_zp_ind_x}, {0xED, nes_addr_mode::nes_addr_mode_abs}, 
    {0xFD, nes_addr_mode::nes_addr_mode_abs_x}, {0xF9, nes_addr_mode::nes_addr_mode_abs_y}, 
    {0xE1, nes_addr_mode::nes_addr_mode_ind_x}, {0xF1, nes_addr_mode::nes_addr_mode_ind_y}, // SBC
    {0x38, nes_addr_mode::nes_addr_mode_imp},                                               // SED
    {0x78, nes_addr_mode::nes_addr_mode_imp},                                               // SEI
    {0x85, nes_addr_mode::nes_addr_mode_zp}, {0x95, nes_addr_mode::nes_addr_mode_zp_ind_x}, 
    {0x8D, nes_addr_mode::nes_addr_mode_abs}, {0x9D, nes_addr_mode::nes_addr_mode_abs_x}, 
    {0x99, nes_addr_mode::nes_addr_mode_abs_y}, {0x81, nes_addr_mode::nes_addr_mode_ind_x}, 
    {0x91, nes_addr_mode::nes_addr_mode_ind_y},                                             // STA
    {0x86, nes_addr_mode::nes_addr_mode_zp}, {0x96, nes_addr_mode::nes_addr_mode_zp_ind_y}, 
    {0x8E, nes_addr_mode::nes_addr_mode_abs},                                               // STX
    {0x84, nes_addr_mode::nes_addr_mode_zp}, {0x94, nes_addr_mode::nes_addr_mode_zp_ind_y}, 
    {0x8C, nes_addr_mode::nes_addr_mode_abs},                                               // STY
    {0xAA, nes_addr_mode::nes_addr_mode_imp},                                               // TAX
    {0xA8, nes_addr_mode::nes_addr_mode_imp},                                               // TAY
    {0xBA, nes_addr_mode::nes_addr_mode_imp},                                               // TSX
    {0x8A, nes_addr_mode::nes_addr_mode_imp},                                               // TXA
    {0x9A, nes_addr_mode::nes_addr_mode_imp},                                               // TXS
    {0x98, nes_addr_mode::nes_addr_mode_imp},                                               // TYA
  };

  enum nes_addr_mode {
      nes_addr_mode_imp, 
      nes_addr_mode_acc,        
      nes_addr_mode_imm,        
      nes_addr_mode_ind_jmp,    
      nes_addr_mode_rel,        
      nes_addr_mode_abs,                     
      nes_addr_mode_abs_jmp,                   
      nes_addr_mode_zp,
      nes_addr_mode_zp_x,
      nes_addr_mode_zp_y,         
      nes_addr_mode_zp_ind_x,   
      nes_addr_mode_zp_ind_y,   
      nes_addr_mode_abs_x,      
      nes_addr_mode_abs_y,      
      nes_addr_mode_ind_x,      
      nes_addr_mode_ind_y,      
  };

  class NES_System {
  private:
    // Processors
    NES_CPU* _cpu;
    NES_APU* _apu;
    NES_PPU* _ppu;

    // Memory
    NES_Memory* _mem;

  public:
    NES_System() {
      _mem = new NES_Memory(); 
      _cpu = new NES_CPU(_mem); 
      _apu = new NES_APU(_mem); 
      _ppu = new NES_PPU(_mem); 
    };
    void step(cycle_t);
  };

  class NES_Memory {
  private:
    // Memory
    BYTE memory[0xFFFF];

  public:
    BYTE read_mem(address_t);
    void write_mem(address_t, BYTE);
  };

  class NES_CPU {
  private:
    // Instruction typedef
    typedef cycle_t (NES_CPU::*instruction)(opcode_t);
    typedef cycle_t (NES_CPU::*address_mode_function)();

    // Operation instructions
    const std::unordered_map<opcode_t, instruction> OPERATION_INSTRUCTIONS = {
      {0x69, &ADC}, {0x65, &ADC}, {0x75, &ADC}, {0x6D, &ADC}, 
      {0x7D, &ADC}, {0x79, &ADC}, {0x61, &ADC}, {0x71, &ADC}, // ADC
      {0x29, &AND}, {0x25, &AND}, {0x35, &AND}, {0x2D, &AND}, 
      {0x3D, &AND}, {0x39, &AND}, {0x21, &AND}, {0x31, &AND}, // AND
      {0x0A, &ASL}, {0x06, &ASL}, {0x16, &ASL}, {0x0E, &ASL}, 
      {0x1E, &ASL},                                           // ASL
      {0x90, &BCC},                                           // BCC
      {0xB0, &BCS},                                           // BCS
      {0xF0, &BEQ},                                           // BEQ
      {0x24, &BIT}, {0x2C, &BIT},                             // BIT
      {0x31, &BMI},                                           // BMI
      {0xD0, &BNE},                                           // BNE
      {0x10, &BPL},                                           // BPL
      {0x00, &BRK},                                           // BRK
      {0x50, &BVC},                                           // BVC
      {0x70, &BVS},                                           // BVS
      {0x18, &CLC},                                           // CLC
      {0xD8, &CLD},                                           // CLD
      {0x58, &CLI},                                           // CLI
      {0xB8, &CLV},                                           // CLV
      {0xC9, &CMP}, {0xC5, &CMP}, {0xD5, &CMP}, {0xCD, &CMP}, 
      {0xDD, &CMP}, {0xD9, &CMP}, {0xC1, &CMP}, {0xD1, &CMP}, // CMP
      {0xE0, &CPX}, {0xE4, &CPX}, {0xEC, &CPX},               // CPX
      {0xC0, &CPY}, {0xC4, &CPY}, {0xCC, &CPY},               // CPY
      {0xC6, &DEC}, {0xD6, &DEC}, {0xCE, &DEC}, {0xDE, &DEC}, // DEC
      {0xCA, &DEX},                                           // DEX
      {0x88, &DEY},                                           // DEY
      {0x49, &EOR}, {0x45, &EOR}, {0x55, &EOR}, {0x4D, &EOR}, 
      {0x5D, &EOR}, {0x59, &EOR}, {0x41, &EOR}, {0x51, &EOR}, // EOR
      {0xE6, &INC}, {0xF6, &INC}, {0xEE, &INC}, {0xFE, &INC}, // INC
      {0xE8, &INX},                                           // INX
      {0xC8, &INY},                                           // INY
      {0x4C, &JMP}, {0x6C, &JMP},                             // JMP
      {0x20, &JSR},                                           // JSR
      {0xA9, &LDA}, {0xA5, &LDA}, {0xB5, &LDA}, {0xAD, &LDA}, 
      {0xBD, &LDA}, {0xB9, &LDA}, {0xA1, &LDA}, {0xB1, &LDA}, // LDA
      {0xA2, &LDX}, {0xA6, &LDX}, {0xB6, &LDX}, {0xAE, &LDX}, 
      {0xBE, &LDX},                                           // LDX
      {0xA0, &LDY}, {0xA4, &LDY}, {0xB4, &LDY}, {0xAC, &LDY}, 
      {0xBC, &LDY},                                           // LDY
      {0x4A, &LSR}, {0x46, &LSR}, {0x56, &LSR}, {0x4E, &LSR}, 
      {0x5E, &LSR},                                           // LSR
      {0xEA, &NOP},                                           // NOP
      {0x09, &ORA}, {0x05, &ORA}, {0x15, &ORA}, {0x0D, &ORA}, 
      {0x1D, &ORA}, {0x19, &ORA}, {0x01, &ORA}, {0x11, &ORA}, // ORA
      {0x48, &PHA},                                           // PHA
      {0x08, &PHP},                                           // PHP
      {0x68, &PLA},                                           // PLA
      {0x28, &PLP},                                           // PLP
      {0x2A, &ROL}, {0x26, &ROL}, {0x36, &ROL}, {0x2E, &ROL}, 
      {0x3E, &ROL},                                           // ROL
      {0x6A, &ROR}, {0x66, &ROR}, {0x76, &ROR}, {0x6E, &ROR}, 
      {0x7E, &ROR},                                           // ROR
      {0x40, &RTI},                                           // RTI
      {0x60, &RTS},                                           // RTS
      {0xE9, &SBC}, {0xE5, &SBC}, {0xF5, &SBC}, {0xED, &SBC}, 
      {0xFD, &SBC}, {0xF9, &SBC}, {0xE1, &SBC}, {0xF1, &SBC}, // SBC
      {0x38, &SED},                                           // SED
      {0x78, &SEI},                                           // SEI
      {0x85, &STA}, {0x95, &STA}, {0x8D, &STA}, {0x9D, &STA}, 
      {0x99, &STA}, {0x81, &STA}, {0x91, &STA},               // STA
      {0x86, &STX}, {0x96, &STX}, {0x8E, &STX},               // STX
      {0x84, &STY}, {0x94, &STY}, {0x8C, &STY},               // STY
      {0xAA, &TAX},                                           // TAX
      {0xA8, &TAY},                                           // TAY
      {0xBA, &TSX},                                           // TSX
      {0x8A, &TXA},                                           // TXA
      {0x9A, &TXS},                                           // TXS
      {0x98, &TYA},                                           // TYA
    };

    // Address mode to function
    const std::unordered_map<nes_addr_mode, address_mode_function> ADDRESS_MODE_FUNCTIONS = {
      {nes_addr_mode::nes_addr_mode_imp,      &IMP}, {nes_addr_mode::nes_addr_mode_imm,      &IMM},
      {nes_addr_mode::nes_addr_mode_zp,       &ZP0}, {nes_addr_mode::nes_addr_mode_zp_ind_x, &ZPX},
      {nes_addr_mode::nes_addr_mode_zp_ind_y, &ZPY}, {nes_addr_mode::nes_addr_mode_abs,      &ABS},
      {nes_addr_mode::nes_addr_mode_abs_jmp,  &ABS}, {nes_addr_mode::nes_addr_mode_abs_x,    &ABX},
      {nes_addr_mode::nes_addr_mode_abs_y,    &ABY}, {nes_addr_mode::nes_addr_mode_ind_jmp,  &IND},
      {nes_addr_mode::nes_addr_mode_ind_x,    &ZPX}, {}
    };

    // Memory Addressing modes
    const std::unordered_set<nes_addr_mode> MEMORY_ADDRESSING_MODES = {
      nes_addr_mode::nes_addr_mode_abs, nes_addr_mode::nes_addr_mode_abs_x,
      nes_addr_mode::nes_addr_mode_abs_y, nes_addr_mode::nes_addr_mode_ind_x,
      nes_addr_mode::nes_addr_mode_ind_y, nes_addr_mode::nes_addr_mode_zp,
      nes_addr_mode::nes_addr_mode_zp_ind_x, nes_addr_mode::nes_addr_mode_zp_ind_y,
      nes_addr_mode::nes_addr_mode_zp_x, nes_addr_mode::nes_addr_mode_zp_y
    };

    // Flags
    static const BYTE N = 0b10000000; // Negative
    static const BYTE V = 0b01000000; // Overflow
    static const BYTE _ = 0b00100000; // Ignored
    static const BYTE B = 0b00010000; // Break
    static const BYTE D = 0b00001000; // Decimal
    static const BYTE I = 0b00000100; // Interrupt
    static const BYTE Z = 0b00000010; // Zero
    static const BYTE C = 0b00000001; // Carry

    // Memory
    NES_Memory* memory;

    // Registers
    BYTE m_accumulator;
    BYTE m_x;
    BYTE m_y;
    BYTE m_stackPointer;
    BYTE m_status;
    address_t m_programCounter;

    // Addressing Values
    operand_t imm;
    operand_t addr_abs;
    operand_t addr_rel;

    // Flag Helpers
    void set_flag(BYTE, bool);
    void calc_alu_flags(BYTE);

    // Cycle Helpers
    cycle_t get_cpu_cycles(opcode_t);

    // Instruction Helpers
    instruction get_instruction(opcode_t);
    cycle_t run_instruction(opcode_t);
    cycle_t branch();
    void jump();

    // Page Helpers
    page_t get_page(address_t);

    // Address Mode Helpers
    nes_addr_mode get_address_mode(opcode_t);
    cycle_t set_value_for_address_mode(nes_addr_mode);
    BYTE get_value_for_address_mode(nes_addr_mode);

    // Addressing Helpers
    cycle_t IMP(); cycle_t IMM(); cycle_t ZP0();
    cycle_t ZPX(); cycle_t ZPY(); cycle_t ABS();
    cycle_t ABX(); cycle_t ABY(); cycle_t IND();
    cycle_t IZX(); cycle_t IZY(); cycle_t REL();

  public:
    // Constructor
    NES_CPU(NES_Memory* memory) { this->memory = memory; };

    // Getters
    BYTE &A() { return m_accumulator; }; BYTE &X() { return m_x; }; BYTE &Y() { return m_y; };
    BYTE &P() { return m_status; }; BYTE &SP() { return m_stackPointer; }; address_t &PC() { return m_programCounter; };

    // Instructions
    cycle_t ADC(opcode_t); cycle_t AND(opcode_t); cycle_t ASL(opcode_t); cycle_t BCC(opcode_t);
    cycle_t BCS(opcode_t); cycle_t BEQ(opcode_t); cycle_t BIT(opcode_t); cycle_t BMI(opcode_t);
    cycle_t BNE(opcode_t); cycle_t BPL(opcode_t); cycle_t BRK(opcode_t); cycle_t BVC(opcode_t);
    cycle_t BVS(opcode_t); cycle_t CLC(opcode_t); cycle_t CLD(opcode_t); cycle_t CLI(opcode_t);
    cycle_t CLV(opcode_t); cycle_t CMP(opcode_t); cycle_t CPX(opcode_t); cycle_t CPY(opcode_t);
    cycle_t DEC(opcode_t); cycle_t DEX(opcode_t); cycle_t DEY(opcode_t); cycle_t EOR(opcode_t);
    cycle_t INC(opcode_t); cycle_t INX(opcode_t); cycle_t INY(opcode_t); cycle_t JMP(opcode_t);
    cycle_t JSR(opcode_t); cycle_t LDA(opcode_t); cycle_t LDX(opcode_t); cycle_t LDY(opcode_t);
    cycle_t LSR(opcode_t); cycle_t NOP(opcode_t); cycle_t ORA(opcode_t); cycle_t PHA(opcode_t);
    cycle_t PHP(opcode_t); cycle_t PLA(opcode_t); cycle_t PLP(opcode_t); cycle_t ROL(opcode_t);
    cycle_t ROR(opcode_t); cycle_t RTI(opcode_t); cycle_t RTS(opcode_t); cycle_t SBC(opcode_t);
    cycle_t SEC(opcode_t); cycle_t SED(opcode_t); cycle_t SEI(opcode_t); cycle_t STA(opcode_t);
    cycle_t STX(opcode_t); cycle_t STY(opcode_t); cycle_t TAX(opcode_t); cycle_t TAY(opcode_t);
    cycle_t TSX(opcode_t); cycle_t TXA(opcode_t); cycle_t TXS(opcode_t); cycle_t TYA(opcode_t);
  };

  class NES_APU {
  private:
    // Memory
    NES_Memory* memory;

  public:
    NES_APU(NES_Memory* memory) { this->memory = memory; };
  };

  class NES_PPU {
  private:
    // Memory
    NES_Memory* memory;

  public:
    NES_PPU(NES_Memory* memory) { this->memory = memory; };
  };
}
