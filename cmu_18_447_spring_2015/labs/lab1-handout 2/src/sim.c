#include <stdio.h>
#include "shell.h"

uint32_t instr = 0;

uint32_t get_part(uint8_t lbit, uint8_t hbit)
{
  return ((instr >> lbit) & ((1 << (hbit - lbit + 1)) - 1));
}

uint32_t sign_extend(uint32_t im)
{
  uint32_t ret = im;
  if ((ret & (1 << 15))) {
    uint32_t upper = 1; 
    upper = (upper << 16) - 1; 
    upper <<= 16;
    ret |= upper;
  }
  return ret;
}

uint32_t sign_extend_8(uint32_t im)
{
  uint32_t ret = im;
  if ((ret & (1 << 7))) {
    uint32_t upper = 1; 
    upper = (upper << 24) - 1; 
    upper <<= 8;
    ret |= upper;
  }
  return ret;
}

void add() 
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
}

void addi() 
{
  uint32_t im = get_part(0, 15);
  im = sign_extend(im);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + im;
}

void addiu() 
{
  uint32_t im = get_part(0, 15);
  im = sign_extend(im);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + im;
}

void addu() 
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
}

void and() 
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
}

void andi() 
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & im;
}

void beq() 
{
  uint32_t im = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
    NEXT_STATE.PC = CURRENT_STATE.PC + im;
  }
}

void bgez() 
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  if (!(CURRENT_STATE.REGS[rs] & (1u << 31))) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void bgezal() 
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;

  if (!(CURRENT_STATE.REGS[rs] & (1u << 31))) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void blez()
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] & (1u << 31)) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
  else if (CURRENT_STATE.REGS[rs] == 0) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void bgtz()
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  if ((!(CURRENT_STATE.REGS[rs] & (1u << 31))) && CURRENT_STATE.REGS[rs] > 0) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void bltz()
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] & (1u << 31)) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void bltzal()
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;

  if (CURRENT_STATE.REGS[rs] & (1u << 31)) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void bne()
{
  uint32_t of = sign_extend((((uint16_t) get_part(0, 15)) << 2));
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
    NEXT_STATE.PC = sign_extend(CURRENT_STATE.PC + of);
  }
}

void div()
{
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  int32_t numerator = (int32_t) CURRENT_STATE.REGS[rs];
  int32_t denominator = (int32_t) CURRENT_STATE.REGS[rt];

  NEXT_STATE.LO = numerator / denominator;
  NEXT_STATE.HI = numerator % denominator;
}

void divu()
{
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  uint32_t numerator = CURRENT_STATE.REGS[rs];
  uint32_t denominator = CURRENT_STATE.REGS[rt];

  NEXT_STATE.LO = numerator / denominator;
  NEXT_STATE.HI = numerator % denominator;
}

void j()
{
  uint32_t target = get_part(0, 25);
  target <<= 2; 
  NEXT_STATE.PC = ((CURRENT_STATE.PC & 0xF0000000) | target);
}

void jal() 
{
  uint32_t target = get_part(0, 25);
  target <<= 2; 
  NEXT_STATE.PC = ((CURRENT_STATE.PC & 0xF0000000) | target);
  NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
}

void jalr()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
  NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
}

void jr()
{
  uint8_t rs = get_part(21, 25);
  NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
}

void lb()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + of;
  uint8_t x = (addr & 0x3);

  uint32_t bytes = mem_read_32(addr - x); 
  uint8_t byte = (bytes >> (8 * x)) & 0xFF;

  NEXT_STATE.REGS[rt] = sign_extend_8(byte);
}

void lbu()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + of;
  uint8_t x = (addr & 0x3);

  uint32_t bytes = mem_read_32(addr - x); 
  uint8_t byte = (bytes >> (8 * x)) & 0xFF;

  NEXT_STATE.REGS[rt] = (uint8_t) byte;
}

// Precondition: LSB of effective address is non-zero 
void lh()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + of;
  uint8_t x = (addr & 0x3);

  uint32_t bytes = mem_read_32(addr - x); 
  uint16_t half_word = (bytes >> (8 * x)) & 0xFFFF;

  NEXT_STATE.REGS[rt] = sign_extend(half_word);
}

// Precondition: LSB of effective address is non-zero 
void lhu()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + of;
  uint8_t x = (addr & 0x3);

  uint32_t bytes = mem_read_32(addr - x); 
  uint16_t half_word = (bytes >> (8 * x)) & 0xFFFF;

  NEXT_STATE.REGS[rt] = (uint32_t) half_word;
}

void lui()
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);

  NEXT_STATE.REGS[rt] = ((uint32_t) im) << 16;
}

void lw()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + of;
  uint8_t x = (addr & 0x3);

  uint32_t bytes = mem_read_32(addr - x); 
  NEXT_STATE.REGS[rt] = bytes;
}

void mfhi()
{
  uint8_t rd = get_part(11, 15);
  NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
}

void mflo() 
{
  uint8_t rd = get_part(11, 15);
  NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
}

void mthi()
{
  uint8_t rs = get_part(21, 25);
  NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}

void mtlo()
{
  uint8_t rs = get_part(21, 25);
  NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
}

void mult()
{
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  int64_t res = ((int64_t) CURRENT_STATE.REGS[rs]) * ((int32_t) CURRENT_STATE.REGS[rt]);

  NEXT_STATE.LO = (((uint64_t) res) & (0xFFFFFFFF));
  NEXT_STATE.HI = ((((uint64_t) res) >> 32) & (0xFFFFFFFF));
}

void multu()
{
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  uint64_t res = ((uint64_t) CURRENT_STATE.REGS[rs]) * CURRENT_STATE.REGS[rt];

  NEXT_STATE.LO = (res & (0xFFFFFFFF));
  NEXT_STATE.HI = ((res >> 32) & (0xFFFFFFFF));
}

void nor()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
}

void or()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
}

void ori()
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | im;
}

void sb()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + sign_extend(of);
  uint8_t x = (addr & 0x3);

  uint8_t byte = (CURRENT_STATE.REGS[rt] & 0xFF);

  uint32_t bytes = mem_read_32(addr - x);

  bytes &= (0xFFFFFFFF - (0xFF << (8 * x)));
  bytes |= (byte << (8 * x));

  mem_write_32(addr - x, bytes);
}

// Precondition: LSB of effective address is zero
void sh()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + sign_extend(of);
  uint8_t x = (addr & 0x3);

  uint16_t half_word = (CURRENT_STATE.REGS[rt] & 0xFFFF);

  uint32_t word = mem_read_32(addr - x);

  word &= (0xFFFFFFFF - (0xFFFF << (8 * x)));
  word |= (half_word << (8 * x));

  mem_write_32(addr - x, word);
}

void sll()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t sa = get_part(6, 10);

  NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] << sa);
}

void sllv()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] << (CURRENT_STATE.REGS[rs] & 0x1F));
}

void slt()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if ( (int32_t) CURRENT_STATE.REGS[rs] < (int32_t) CURRENT_STATE.REGS[rt]) {
    NEXT_STATE.REGS[rd] = 1;
  }
  else {
    NEXT_STATE.REGS[rd] = 0;
  }
}

void slti()
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if ( (int32_t) CURRENT_STATE.REGS[rs] < (int32_t) sign_extend(im)) {
    NEXT_STATE.REGS[rt] = 1;
  }
  else {
    NEXT_STATE.REGS[rt] = 0;
  }
}

void sltiu()
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] < (uint32_t) sign_extend(im)) {
    NEXT_STATE.REGS[rt] = 1;
  }
  else {
    NEXT_STATE.REGS[rt] = 0;
  }
}

void sltu()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) {
    NEXT_STATE.REGS[rd] = 1;
  }
  else {
    NEXT_STATE.REGS[rd] = 0;
  }
}

void sra()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t sa = get_part(6, 10);

  NEXT_STATE.REGS[rd] = (((int32_t) CURRENT_STATE.REGS[rt]) >> sa);
}

void srav()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  uint16_t shift = (CURRENT_STATE.REGS[rs] & 0x1F);
  int32_t x = ((int32_t) CURRENT_STATE.REGS[rt]) | (1 << 31);
  NEXT_STATE.REGS[rd] = (x >> shift);
}

void srl()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t sa = get_part(6, 10);

  NEXT_STATE.REGS[rd] = (CURRENT_STATE.REGS[rt] >> sa);
}

void srlv()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = ((CURRENT_STATE.REGS[rt]) >> (CURRENT_STATE.REGS[rs] & 0x1F));
}

void sub() 
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
}

void subu() 
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
}

// Precondition: The effective address is a multiple of four
void sw()
{
  uint32_t of = get_part(0, 15);
  of = sign_extend(of);

  uint8_t rt = get_part(16, 20);
  uint8_t base = get_part(21, 25);

  uint32_t addr = CURRENT_STATE.REGS[base] + sign_extend(of);
  mem_write_32(addr, CURRENT_STATE.REGS[rt]);
}

void syscall() 
{
  if (CURRENT_STATE.REGS[2] == 0x0A) {
    RUN_BIT = 0;
  }
}

void xor()
{
  uint8_t rd = get_part(11, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
}

void xori()
{
  uint32_t im = get_part(0, 15);
  uint8_t rt = get_part(16, 20);
  uint8_t rs = get_part(21, 25);

  NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ im;
}

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */ 

    instr = mem_read_32(CURRENT_STATE.PC);

    NEXT_STATE = CURRENT_STATE; 
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

    int32_t part_1 = get_part(26, 28);
    int32_t part_2 = get_part(29, 31);

    // SPECIAL
    if (part_1 == 0 && part_2 == 0) {
      int32_t part_3 = get_part(0, 2);
      int32_t part_4 = get_part(3, 5);

      if (part_3 == 0 && part_4 == 4) {
        add();
      }

      else if (part_3 == 1 && part_4 == 4) {
        addu();
      }

      else if (part_3 == 4 && part_4 == 4) {
        and();
      }

      else if (part_3 == 2 && part_4 == 3) {
        div();
      }

      else if (part_3 == 3 && part_4 == 3) {
        divu();
      }

      else if (part_3 == 1 && part_4 == 1) {
        jalr();
      }

      else if (part_3 == 0 && part_4 == 1) {
        jr();
      }

      else if (part_3 == 0 && part_4 == 2) {
        mfhi();
      }

      else if (part_3 == 2 && part_4 == 2) {
        mflo();
      }

      else if (part_3 == 1 && part_4 == 2) {
        mthi();
      }

      else if (part_3 == 3 && part_4 == 2) {
        mtlo();
      }

      else if (part_3 == 0 && part_4 == 3) {
        mult();
      }

      else if (part_3 == 1 && part_4 == 3) {
        multu();
      }

      else if (part_3 == 7 && part_4 == 4) {
        nor();
      }

      else if (part_3 == 5 && part_4 == 4) {
        or();
      }

      else if (part_3 == 0 && part_4 == 0) {
        sll();
      }

      else if (part_3 == 4 && part_4 == 0) {
        sllv();
      }

      else if (part_3 == 2 && part_4 == 5) {
        slt();
      }

      else if (part_3 == 3 && part_4 == 5) {
        sltu();
      }

      else if (part_3 == 3 && part_4 == 0) {
        sra();
      }

      else if (part_3 == 7 && part_4 == 0) {
        srav();
      }

      else if (part_3 == 2 && part_4 == 0) {
        srl();
      }

      else if (part_3 == 6 && part_4 == 0) {
        srlv();
      }

      else if (part_3 == 2 && part_4 == 4) {
        sub();
      }

      else if (part_3 == 3 && part_4 == 4) {
        subu();
      }

      else if (part_3 == 4 && part_4 == 1) {
        syscall();
      }

      else if (part_3 == 6 && part_4 == 4) {
        xor();
      }
    }

    // REGIMM
    else if (part_1 == 1 && part_2 == 0) {
      int32_t part_3 = get_part(16, 18);
      int32_t part_4 = get_part(19, 20);

      if (part_3 == 1 && part_4 == 0) {
        bgez();
      }

      else if (part_3 == 1 && part_4 == 2) {
        bgezal();
      }

      else if (part_3 == 0 && part_4 == 0) {
        bltz();
      }

      else if (part_3 == 0 && part_4 == 2) {
        bltzal();
      }
    }

    else if (part_1 == 0 && part_2 == 1) {
      addi();
    }

    else if (part_1 == 1 && part_2 == 1) {
      addiu();
    }

    else if (part_1 == 4 && part_2 == 1) {
      andi();
    }

    else if (part_1 == 4 && part_2 == 0) {
      beq();
    }

    else if (part_1 == 7 && part_2 == 0) {
      bgtz();
    }

    else if (part_1 == 6 && part_2 == 0) {
      blez();
    }

    else if (part_1 == 5 && part_2 == 0) {
      bne();
    }

    else if (part_1 == 2 && part_2 == 0) {
      j();
    }

    else if (part_1 == 3 && part_2 == 0) {
      jal();
    }

    else if (part_1 == 0 && part_2 == 4) {
      lb();
    }

    else if (part_1 == 4 && part_2 == 4) {
      lbu();
    }

    else if (part_1 == 1 && part_2 == 4) {
      lh();
    }

    else if (part_1 == 5 && part_2 == 4) {
      lhu();
    }

    else if (part_1 == 7 && part_2 == 1) {
      lui();
    }

    else if (part_1 == 3 && part_2 == 4) {
      lw();
    }

    else if (part_1 == 5 && part_2 == 1) {
      ori();
    }

    else if (part_1 == 0 && part_2 == 5) {
      sb();
    }

    else if (part_1 == 1 && part_2 == 5) {
      sh();
    }

    else if (part_1 == 2 && part_2 == 1) {
      slti();
    }

    else if (part_1 == 3 && part_2 == 1) {
      sltiu();
    }

    else if (part_1 == 3 && part_2 == 5) {
      sw();
    }

    else if (part_1 == 6 && part_2 == 1) {
      xori();
    }
}