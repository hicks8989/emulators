#include <unordered_map>

namespace NES_Emulator {
  typedef unsigned char BYTE;
  typedef unsigned short int WORD;
  typedef unsigned char page_t;
  typedef unsigned char cycle_t;
  typedef unsigned char opcode_t;
  typedef unsigned short int operand_t;
  typedef unsigned short int address_t;

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

    // Flag Helpers
    void set_flag(BYTE, bool);
    void calc_alu_flags(BYTE);

    // Cycle Helpers
    cycle_t get_cpu_cycles(opcode_t);

    // Instruction Helpers
    cycle_t branch(BYTE);

    // Page Helpers
    page_t get_page(address_t);

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
