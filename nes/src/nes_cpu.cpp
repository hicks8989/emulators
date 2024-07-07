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
}