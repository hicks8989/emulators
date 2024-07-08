#include "nes.h"

namespace NES_Emulator {
  // Helpers
  void NES_CPU::set_flag(BYTE F, bool v) {
    if (v)
      P() |= F;
    else
      P() &= ~F;
  }

  void NES_CPU::calc_alu_flags(BYTE val) {
    // N
    set_flag(N, val < 0);

    // Z
    set_flag(Z, val == 0);
  }

  cycle_t NES_CPU::get_cpu_cycles(opcode_t op) {
    // Check for NOP
    if (!BASE_OPERATION_CYCLES.count(op))
      return 0;
    
    return BASE_OPERATION_CYCLES.at(op);
  }

  cycle_t NES_CPU::branch(BYTE value) {
    cycle_t additional_cycles = 1;
    WORD new_pc = PC() + value;

    if (get_page(new_pc) != get_page(PC()))
      additional_cycles++;

    PC() = new_pc;
    return additional_cycles;
  }

  page_t NES_CPU::get_page(address_t address) {
    return address >> 8;
  }

  nes_addr_mode NES_CPU::get_address_mode(opcode_t op) {
    if (!OPERATION_ADDRESS_MODES.count(op))
      return nes_addr_mode::nes_addr_mode_imp;
    
    return OPERATION_ADDRESS_MODES.at(op);
  }

  // Instructions
  cycle_t NES_CPU::ADC(opcode_t op) {
    /**
     * Add with carry.
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    A() &= val;

    // Calculate flags
    calc_alu_flags(A());

    // Cycle count
    return get_cpu_cycles(op);
  }

  cycle_t NES_CPU::AND(opcode_t op) {
    /**
     * And (with accumulator).
     */
    BYTE val = 0;

    A() &= val;

    // Calculate flags
    calc_alu_flags(A());

    // Cycle count
    return get_cpu_cycles(op);
  }

  cycle_t NES_CPU::ASL(opcode_t op) {
    /**
     * Arithmetic shift left.
     */
  }

  cycle_t NES_CPU::BCC(opcode_t op) {
    /**
     * Branch on carry clear.
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (~P() & C) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BCS(opcode_t op) {
    /**
     * Branch on carry set.
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (P() & C) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BEQ(opcode_t op) {
    /**
     * Branch on equal (zero set).
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (P() & Z) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BIT(opcode_t op) {
    /**
     * Bit test.
     */
  }

  cycle_t NES_CPU::BMI(opcode_t op) {
    /**
     * Branch on minus (negative set).
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (P() & N) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BNE(opcode_t op) {
    /**
     * Branch on not equal (zero clear).
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (~P() & Z) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BPL(opcode_t op) {
    /**
     * Branch on plus (negative clear).
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (~P() & N) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BRK(opcode_t op) {
    /**
     * Break / interrupt.
     */
    BYTE val = 0;

    

    // Cycle count
    return get_cpu_cycles(op);
  }

  cycle_t NES_CPU::BVC(opcode_t op) {
    /**
     * Branch on overflow clear.
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (~P() & V) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::BVS(opcode_t op) {
    /**
     * Branch on overflow set.
     */
    BYTE val = 0;

    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);

    if (P() & V) {
      cycles += branch(val);
    }

    return cycles;
  }

  cycle_t NES_CPU::CLC(opcode_t op) {
    /**
     * Clear Carry Flag.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(C, 0);

    return cycles;
  }

  cycle_t NES_CPU::CLD(opcode_t op) {
    /**
     * Clear Decimal Flag.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(D, 0);

    return cycles;
  }

  cycle_t NES_CPU::CLI(opcode_t op) {
    /**
     * Clear Interrupt disable bit.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(I, 0);

    return cycles;
  }

  cycle_t NES_CPU::CLV(opcode_t op) {
    /**
     * Clear Overflow flag.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(V, 0);

    return cycles;
  }

  cycle_t NES_CPU::CMP(opcode_t op) {
    /**
     * Compare Memory with Accumulator.
     */
    BYTE val = 0;


  }

  cycle_t NES_CPU::DEC(opcode_t op) {
    /**
     * Decrement Memory by one.
     */

  }

  cycle_t NES_CPU::DEX(opcode_t op) {
    /**
     * Decrement X by one.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    X() -= 1;

    calc_alu_flags(X());

    return cycles;
  }

  cycle_t NES_CPU::DEY(opcode_t op) {
    /**
     * Decrement Y by one.
     */
    // Cycle count
    cycle_t cycles = get_cpu_cycles(op);
    Y() -= 1;
    
    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::EOR(opcode_t op) {
    /**
     * Exclusive-OR with memory and accumulator.
     */
    cycle_t cycles = get_cpu_cycles(op);
    BYTE value = 0;

    A() ^= value;

    calc_alu_flags(A());
    
    return cycles;
  }

  cycle_t NES_CPU::INC(opcode_t op) {

  }

  cycle_t NES_CPU::INX(opcode_t op) {
    /**
     * Increment X by one.
     */
    cycle_t cycles = get_cpu_cycles(op);
    X() += 1;

    calc_alu_flags(X());

    return cycles;
  }

  cycle_t NES_CPU::INY(opcode_t op) {
    /**
     * Increment Y by one.
     */
    cycle_t cycles = get_cpu_cycles(op);
    Y() += 1;

    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::JMP(opcode_t op) {
    /**
     * Jump to new location.
     */
    cycle_t cycles = get_cpu_cycles(op);
  }

  cycle_t NES_CPU::JSR(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);

  }

  cycle_t NES_CPU::LDA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    BYTE value = 0;

    A() = value;

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::LDX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    BYTE value = 0;

    X() = value;

    calc_alu_flags(X());
    
    return cycles;
  }

  cycle_t NES_CPU::LDY(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    BYTE value = 0;

    Y() = value;

    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::LSR(opcode_t op) {

  }
}