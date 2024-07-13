#include "nes_cpu.h"

namespace NES_Emulator {
  // Helpers
  cycle_t NES_CPU::reset() {
    A() = 0;
    X() = 0;
    Y() = 0;
    SP() = 0xFD;
    P() = 0x00;

    addr_abs = 0xFFFC;
    BYTE lo = bus->cpu_read(addr_abs);
    BYTE hi = bus->cpu_read(addr_abs + 1);

    PC() = (hi << 8) | lo;

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    imm = 0x00;

    return 8;
  }

  cycle_t NES_CPU::IRQ() {
    if (P() & I)
      return;
    
    bus->cpu_write(0x0100 + SP()--, (PC() >> 8) & 0x00FF);
    bus->cpu_write(0x0100 + SP()--, PC() & 0x00FF);

    set_flag(B, 0);
		set_flag(_, 1);
		set_flag(I, 1);
    bus->cpu_write(0x0100 + SP()--, P());

    addr_abs = 0xFFEE;
    BYTE lo = bus->cpu_read(addr_abs + 0);
		BYTE hi = bus->cpu_read(addr_abs + 1);
    PC() = (hi << 8) & lo;

    return 7;
  }

  cycle_t NES_CPU::NMI() {
    bus->cpu_write(0x0100 + SP()--, (PC() >> 8) & 0x00FF);
    bus->cpu_write(0x0100 + SP()--, PC() & 0x00FF);

    set_flag(B, 0);
		set_flag(_, 1);
		set_flag(I, 1);
    bus->cpu_write(0x0100 + SP()--, P());

    addr_abs = 0xFFEE;
    BYTE lo = bus->cpu_read(addr_abs + 0);
		BYTE hi = bus->cpu_read(addr_abs + 1);
    PC() = (hi << 8) & lo;

    return 8;
  }

  void NES_CPU::set_flag(BYTE F, bool v) {
    if (v)
      P() |= F;
    else
      P() &= ~F;
  }

  void NES_CPU::calc_alu_flags(BYTE val) {
    // N
    set_flag(N, val & N);

    // Z
    set_flag(Z, val == 0);
  }

  cycle_t NES_CPU::get_cpu_cycles(opcode_t op) {
    // Check for NOP
    if (!BASE_OPERATION_CYCLES.count(op))
      return 0;
    
    return BASE_OPERATION_CYCLES.at(op);
  }

  cycle_t NES_CPU::run_instruction(opcode_t op) {
    if (!OPERATION_INSTRUCTIONS.count(op))
      return NOP(op);
    
    return (this->*OPERATION_INSTRUCTIONS.at(op))(op);
  }

  cycle_t NES_CPU::branch() {
    cycle_t additional_cycles = 1;
    WORD new_pc = PC() + addr_rel;

    if (get_page(new_pc) != get_page(PC()))
      additional_cycles++;

    PC() = new_pc;
    return additional_cycles;
  }

  void NES_CPU::jump() {
    PC() = addr_abs;
  }

  page_t NES_CPU::get_page(address_t address) {
    return address >> 8;
  }

  nes_addr_mode NES_CPU::get_address_mode(opcode_t op) {
    if (!OPERATION_ADDRESS_MODES.count(op))
      return nes_addr_mode::nes_addr_mode_imp;
    
    return OPERATION_ADDRESS_MODES.at(op);
  }

  cycle_t NES_CPU::set_value_for_address_mode(nes_addr_mode addr_mode) {
    if (!ADDRESS_MODE_FUNCTIONS.count(addr_mode))
      return IMP();
    
    return (this->*ADDRESS_MODE_FUNCTIONS.at(addr_mode))();
  }

  BYTE NES_CPU::get_value_for_address_mode(nes_addr_mode addr_mode) {
    if (MEMORY_ADDRESSING_MODES.count(addr_mode)) {
      return bus->cpu_read(addr_abs);
    } else if (addr_mode == nes_addr_mode::nes_addr_mode_acc) {
      return A();
    }

    return imm;
  }

  cycle_t NES_CPU::IMP() {
    return 0;
  }

  cycle_t NES_CPU::IMM() {
    imm = bus->cpu_read(PC()++);
    return 0;
  }

  cycle_t NES_CPU::ZP0() {
    addr_abs = bus->cpu_read(PC()++);
    addr_abs &= 0x00FF;
    return 0;
  }

  cycle_t NES_CPU::ZPX() {
    addr_abs = (address_t)bus->cpu_read(PC()++) + X();
    addr_abs &= 0x00FF;
    return 0;
  }

  cycle_t NES_CPU::ZPY() {
    addr_abs = (address_t)bus->cpu_read(PC()++) + Y();
    addr_abs &= 0x00FF;
    return 0;
  }

  cycle_t NES_CPU::ABS() {
    BYTE lo = bus->cpu_read(PC()++);
    BYTE hi = bus->cpu_read(PC()++);

    addr_abs = (hi << 8) | lo;
    return 0;
  }

  cycle_t NES_CPU::ABX() {
    BYTE lo = bus->cpu_read(PC()++);
    BYTE hi = bus->cpu_read(PC()++);

    addr_abs = (hi << 8) | lo;
    addr_abs += X();

    if (get_page(addr_abs) != hi) {
      return 1;
    }

    return 0;
  }

  cycle_t NES_CPU::ABY() {
    BYTE lo = bus->cpu_read(PC()++);
    BYTE hi = bus->cpu_read(PC()++);

    addr_abs = (hi << 8) | lo;
    addr_abs += Y();

    if (get_page(addr_abs) != hi) {
      return 1;
    }
    
    return 0;
  }

  cycle_t NES_CPU::IND() {
    BYTE lo = bus->cpu_read(PC()++);
    BYTE hi = bus->cpu_read(PC()++);

    address_t ptr = (hi << 8) | lo;

    if (lo == 0x00FF) {
      addr_abs = (bus->cpu_read(ptr & 0xFF00) << 8) | bus->cpu_read(ptr);
    } else {
      addr_abs = (bus->cpu_read(ptr + 1) << 8) | bus->cpu_read(ptr);
    }

    return 0;
  }

  cycle_t NES_CPU::IZX() {
    address_t ptr = (address_t)bus->cpu_read(PC()++) + X();
    ptr &= 0x00FF;

    addr_abs = (bus->cpu_read((ptr + 1) & 0x00FF) << 8) | bus->cpu_read(ptr);
    return 0;
  }

  cycle_t NES_CPU::IZY() {
    address_t ptr = bus->cpu_read(PC()++);
    ptr &= 0x00FF;

    addr_abs = (bus->cpu_read((ptr + 1) & 0x00FF) << 8) | bus->cpu_read(ptr);
    addr_abs += Y();

    if (get_page(addr_abs) != bus->cpu_read((ptr + 1) & 0x00FF)) {
      return 1;
    }

    return 0;
  }

  cycle_t NES_CPU::REL() {
    addr_rel = bus->cpu_read(PC()++);
    if (addr_rel & 0x80) {
      addr_rel |= 0xFF00;
    }

    return 0;
  }

  // Instructions
  cycle_t NES_CPU::ADC(opcode_t op) {
    /**
     * Add with carry.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    set_flag(C, (int)A() + val > 0x00FF);
    set_flag(V, 
      (A() < 0x80 && val < 0x80 && (int)A() + val >= 0x80) || 
      (A() > 0x80 && val > 0x80 && (int)A() + val < 0x80)
    );

    A() = (int)A() + val;

    // Calculate flags
    calc_alu_flags(A());

    // Cycle count
    return get_cpu_cycles(op);
  }

  cycle_t NES_CPU::AND(opcode_t op) {
    /**
     * And (with accumulator).
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    A() &= val;

    // Calculate flags
    calc_alu_flags(A());

    // Cycle count
    return cycles;
  }

  cycle_t NES_CPU::ASL(opcode_t op) {
    /**
     * Arithmetic shift left.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);

    if (addr_mode == nes_addr_mode::nes_addr_mode_acc) {
      set_flag(C, A() & 0x80);
      A() <<= 1;
      calc_alu_flags(A());
    } else {
      BYTE data = bus->cpu_read(addr_abs);
      set_flag(C, data & 0x80);
      data <<= 1;
      calc_alu_flags(data);
      bus->cpu_write(addr_abs, data);
    }

    return cycles;
  }

  cycle_t NES_CPU::BCC(opcode_t op) {
    /**
     * Branch on carry clear.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (~P() & C) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BCS(opcode_t op) {
    /**
     * Branch on carry set.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (P() & C) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BEQ(opcode_t op) {
    /**
     * Branch on equal (zero set).
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (P() & Z) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BIT(opcode_t op) {
    /**
     * Bit test.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    calc_alu_flags(A() & val);
    set_flag(N, val & 0b10000000);
    set_flag(V, val & 0b01000000);

    return cycles;
  }

  cycle_t NES_CPU::BMI(opcode_t op) {
    /**
     * Branch on minus (negative set).
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (P() & N) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BNE(opcode_t op) {
    /**
     * Branch on not equal (zero clear).
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (~P() & Z) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BPL(opcode_t op) {
    /**
     * Branch on plus (negative clear).
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (~P() & N) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BRK(opcode_t op) {
    /**
     * Break / interrupt.
     */
    operand_t val = 0;

    

    // Cycle count
    return get_cpu_cycles(op);
  }

  cycle_t NES_CPU::BVC(opcode_t op) {
    /**
     * Branch on overflow clear.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (~P() & V) {
      cycles += branch();
    }

    return cycles;
  }

  cycle_t NES_CPU::BVS(opcode_t op) {
    /**
     * Branch on overflow set.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_rel;
    cycles += set_value_for_address_mode(addr_mode);

    if (P() & V) {
      cycles += branch();
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
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);
    WORD cmp;

    val = val ^ 0x00FF;
    cmp = (int)A() + val;

    set_flag(C, cmp > 0x00FF);
    set_flag(V, 
      (A() < 0x80 && val < 0x80 && cmp >= 0x80) || 
      (A() > 0x80 && val > 0x80 && cmp < 0x80)
    );

    calc_alu_flags(cmp);

    return cycles;
  }

  cycle_t NES_CPU::CPX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);
    WORD cmp;

    val = val ^ 0x00FF;
    cmp = (int)X() + val;

    set_flag(C, cmp > 0x00FF);
    set_flag(V, 
      (X() < 0x80 && val < 0x80 && cmp >= 0x80) || 
      (X() > 0x80 && val > 0x80 && cmp < 0x80)
    );

    calc_alu_flags(cmp);

    return cycles;
  }

  cycle_t NES_CPU::CPY(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);
    WORD cmp;

    val = val ^ 0x00FF;
    cmp = (int)Y() + val;

    set_flag(C, cmp > 0x00FF);
    set_flag(V, 
      (Y() < 0x80 && val < 0x80 && cmp >= 0x80) || 
      (Y() > 0x80 && val > 0x80 && cmp < 0x80)
    );

    calc_alu_flags(cmp);

    return cycles;
  }

  cycle_t NES_CPU::DEC(opcode_t op) {
    /**
     * Decrement Memory by one.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = bus->cpu_read(addr_abs);

    val -= 1;

    calc_alu_flags(val);
    bus->cpu_write(addr_abs, val);

    return cycles;
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
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    A() ^= val;

    calc_alu_flags(A());
    
    return cycles;
  }

  cycle_t NES_CPU::INC(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = bus->cpu_read(addr_abs);

    val = (int)val + 1;

    calc_alu_flags(val);
    bus->cpu_write(addr_abs, val);

    return cycles;
  }

  cycle_t NES_CPU::INX(opcode_t op) {
    /**
     * Increment X by one.
     */
    cycle_t cycles = get_cpu_cycles(op);
    X() = (int)X() + 1;

    calc_alu_flags(X());

    return cycles;
  }

  cycle_t NES_CPU::INY(opcode_t op) {
    /**
     * Increment Y by one.
     */
    cycle_t cycles = get_cpu_cycles(op);
    Y() = (int)Y() + 1;

    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::JMP(opcode_t op) {
    /**
     * Jump to new location.
     */
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);

    jump();

    return cycles;
  }

  cycle_t NES_CPU::JSR(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = nes_addr_mode::nes_addr_mode_abs_jmp;
    cycles += set_value_for_address_mode(addr_mode);

    bus->cpu_write(SP()++, PC() + 2);
    bus->cpu_write(SP()++, (PC() + 2) >> 8);

    jump();

    return cycles;
  }

  cycle_t NES_CPU::LDA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    A() = val;

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::LDX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    X() = val;

    calc_alu_flags(X());
    
    return cycles;
  }

  cycle_t NES_CPU::LDY(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    Y() = val;

    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::LSR(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    
    if (addr_mode == nes_addr_mode::nes_addr_mode_acc) {
      set_flag(C, A() & 1);
      A() >>= 1;
      calc_alu_flags(A());
    } else {
      BYTE data = bus->cpu_read(addr_abs);
      set_flag(C, data & 1);
      data >>= 1;
      calc_alu_flags(data);
      bus->cpu_write(addr_abs, data);
    }

    return cycles;
  }

  cycle_t NES_CPU::NOP(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    return cycles;
  }

  cycle_t NES_CPU::ORA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    A() |= val;

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::PHA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    bus->cpu_write(0x0100 + SP()--, A());

    return cycles;
  }

  cycle_t NES_CPU::PHP(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    BYTE pre_push = P() & 0xFF;

    set_flag(B, 1);
    set_flag(_, 1);

    bus->cpu_write(0x0100 + SP()--, P());
    P() = pre_push;

    return cycles;
  }

  cycle_t NES_CPU::PLA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);

    A() = bus->cpu_read(0x0100 + ++SP());

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::PLP(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    P() = bus->cpu_read(0x0100 + ++SP());

    set_flag(_, 0);
    set_flag(B, 0);

    return cycles;
  }

  cycle_t NES_CPU::ROL(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);

    if (addr_mode == nes_addr_mode::nes_addr_mode_acc) {
      WORD temp = (A() << 1) | C;

      set_flag(C, temp > 0x00FF);
      A() = temp & 0x00FF;
      calc_alu_flags(A());
    } else {
      WORD val = bus->cpu_read(addr_abs);

      val = (val << 1) | C;
      set_flag(C, val > 0x00FF);
      calc_alu_flags(val);
      bus->cpu_write(addr_abs, val & 0x00FF);
    }
  }

  cycle_t NES_CPU::ROR(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);

    if (addr_mode == nes_addr_mode::nes_addr_mode_acc) {
      A() = (C << 7) | (A() >> 1);
      set_flag(C, A() & 0x01);
      calc_alu_flags(A());
    } else {
      BYTE val = bus->cpu_read(addr_abs);

      val = (C << 7) | (val >> 1);
      set_flag(C, val & 0x01);
      calc_alu_flags(val);
      bus->cpu_write(addr_abs, val);
    }
  }

  cycle_t NES_CPU::RTI(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    P() = bus->cpu_read(SP()--);
    BYTE lo = bus->cpu_read(SP()--);
    BYTE hi = bus->cpu_read(SP()--);

    set_flag(_, 0);
    set_flag(B, 0);

    PC() = (hi << 8) | lo;

    return cycles;
  }

  cycle_t NES_CPU::RTS(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    BYTE lo = bus->cpu_read(SP()--);
    BYTE hi = bus->cpu_read(SP()--);

    PC() = (hi << 8) | lo;

    return cycles;
  }

  cycle_t NES_CPU::SBC(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    BYTE val = get_value_for_address_mode(addr_mode);

    val = val ^ 0x00FF;
    set_flag(C, (int)A() + val > 0x00FF);
    set_flag(V, 
      (A() < 0x80 && val < 0x80 && (int)A() + val >= 0x80) || 
      (A() > 0x80 && val > 0x80 && (int)A() + val < 0x80)
    );

    A() = (int)A() + val;

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::SEC(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(C, 1);

    return cycles;
  }

  cycle_t NES_CPU::SED(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(D, 1);

    return cycles;
  }

  cycle_t NES_CPU::SEI(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    set_flag(I, 1);

    return cycles;
  }

  cycle_t NES_CPU::STA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    
    bus->cpu_write(addr_abs, A());

    return cycles;
  }

  cycle_t NES_CPU::STX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    
    bus->cpu_write(addr_abs, X());

    return cycles;
  }

  cycle_t NES_CPU::STY(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    nes_addr_mode addr_mode = get_address_mode(op);
    cycles += set_value_for_address_mode(addr_mode);
    
    bus->cpu_write(addr_abs, Y());

    return cycles;
  }

  cycle_t NES_CPU::TAX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    X() = A();

    calc_alu_flags(X());

    return cycles;
  }

  cycle_t NES_CPU::TAY(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    Y() = A();

    calc_alu_flags(Y());

    return cycles;
  }

  cycle_t NES_CPU::TSX(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    X() = SP();

    calc_alu_flags(X());

    return cycles;
  }

  cycle_t NES_CPU::TXA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    A() = X();

    calc_alu_flags(A());

    return cycles;
  }

  cycle_t NES_CPU::TXS(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    SP() = X();

    return cycles;
  }

  cycle_t NES_CPU::TYA(opcode_t op) {
    cycle_t cycles = get_cpu_cycles(op);
    A() = Y();

    calc_alu_flags(A());

    return cycles;
  }
}
