#pragma once

#include "memory.h"
#include <stdint.h>
#include <iostream>

class Interrupts;

class Registers;

class InstructionSet {
   private:
    Registers *regs;
    Memory *memory;
    Interrupts *interrupts;

    typedef void (InstructionSet::*InstructionFunc)();
    typedef void (InstructionSet::*ExtendedInstructionFunc)();
    InstructionFunc instruction_table[256];
    ExtendedInstructionFunc extended_instruction_table[256];
    
    void write_short_stack(uint16_t value);
    uint16_t read_short_stack();

    // Individual instruction methods
    void op_00(); void op_01(); void op_02(); void op_03(); void op_04(); void op_05(); void op_06(); void op_07();
    void op_08(); void op_09(); void op_0a(); void op_0b(); void op_0c(); void op_0d(); void op_0e(); void op_0f();
    void op_10(); void op_11(); void op_12(); void op_13(); void op_14(); void op_15(); void op_16(); void op_17();
    void op_18(); void op_19(); void op_1a(); void op_1b(); void op_1c(); void op_1d(); void op_1e(); void op_1f();
    void op_20(); void op_21(); void op_22(); void op_23(); void op_24(); void op_25(); void op_26(); void op_27();
    void op_28(); void op_29(); void op_2a(); void op_2b(); void op_2c(); void op_2d(); void op_2e(); void op_2f();
    void op_30(); void op_31(); void op_32(); void op_33(); void op_34(); void op_35(); void op_36(); void op_37();
    void op_38(); void op_39(); void op_3a(); void op_3b(); void op_3c(); void op_3d(); void op_3e(); void op_3f();
    void op_40(); void op_41(); void op_42(); void op_43(); void op_44(); void op_45(); void op_46(); void op_47();
    void op_48(); void op_49(); void op_4a(); void op_4b(); void op_4c(); void op_4d(); void op_4e(); void op_4f();
    void op_50(); void op_51(); void op_52(); void op_53(); void op_54(); void op_55(); void op_56(); void op_57();
    void op_58(); void op_59(); void op_5a(); void op_5b(); void op_5c(); void op_5d(); void op_5e(); void op_5f();
    void op_60(); void op_61(); void op_62(); void op_63(); void op_64(); void op_65(); void op_66(); void op_67();
    void op_68(); void op_69(); void op_6a(); void op_6b(); void op_6c(); void op_6d(); void op_6e(); void op_6f();
    void op_70(); void op_71(); void op_72(); void op_73(); void op_74(); void op_75(); void op_76(); void op_77();
    void op_78(); void op_79(); void op_7a(); void op_7b(); void op_7c(); void op_7d(); void op_7e(); void op_7f();
    void op_80(); void op_81(); void op_82(); void op_83(); void op_84(); void op_85(); void op_86(); void op_87();
    void op_88(); void op_89(); void op_8a(); void op_8b(); void op_8c(); void op_8d(); void op_8e(); void op_8f();
    void op_90(); void op_91(); void op_92(); void op_93(); void op_94(); void op_95(); void op_96(); void op_97();
    void op_98(); void op_99(); void op_9a(); void op_9b(); void op_9c(); void op_9d(); void op_9e(); void op_9f();
    void op_a0(); void op_a1(); void op_a2(); void op_a3(); void op_a4(); void op_a5(); void op_a6(); void op_a7();
    void op_a8(); void op_a9(); void op_aa(); void op_ab(); void op_ac(); void op_ad(); void op_ae(); void op_af();
    void op_b0(); void op_b1(); void op_b2(); void op_b3(); void op_b4(); void op_b5(); void op_b6(); void op_b7();
    void op_b8(); void op_b9(); void op_ba(); void op_bb(); void op_bc(); void op_bd(); void op_be(); void op_bf();
    void op_c0(); void op_c1(); void op_c2(); void op_c3(); void op_c4(); void op_c5(); void op_c6(); void op_c7();
    void op_c8(); void op_c9(); void op_ca(); void op_cb(); void op_cc(); void op_cd(); void op_ce(); void op_cf();
    void op_d0(); void op_d1(); void op_d2(); void op_d3(); void op_d4(); void op_d5(); void op_d6(); void op_d7();
    void op_d8(); void op_d9(); void op_da(); void op_db(); void op_dc(); void op_dd(); void op_de(); void op_df();
    void op_e0(); void op_e1(); void op_e2(); void op_e3(); void op_e4(); void op_e5(); void op_e6(); void op_e7();
    void op_e8(); void op_e9(); void op_ea(); void op_eb(); void op_ec(); void op_ed(); void op_ee(); void op_ef();
    void op_f0(); void op_f1(); void op_f2(); void op_f3(); void op_f4(); void op_f5(); void op_f6(); void op_f7();
    void op_f8(); void op_f9(); void op_fa(); void op_fb(); void op_fc(); void op_fd(); void op_fe(); void op_ff();

    // Extended instruction methods
    void ext_00(); void ext_01(); void ext_02(); void ext_03(); void ext_04(); void ext_05(); void ext_06(); void ext_07();
    void ext_08(); void ext_09(); void ext_0a(); void ext_0b(); void ext_0c(); void ext_0d(); void ext_0e(); void ext_0f();
    void ext_10(); void ext_11(); void ext_12(); void ext_13(); void ext_14(); void ext_15(); void ext_16(); void ext_17();
    void ext_18(); void ext_19(); void ext_1a(); void ext_1b(); void ext_1c(); void ext_1d(); void ext_1e(); void ext_1f();
    void ext_20(); void ext_21(); void ext_22(); void ext_23(); void ext_24(); void ext_25(); void ext_26(); void ext_27();
    void ext_28(); void ext_29(); void ext_2a(); void ext_2b(); void ext_2c(); void ext_2d(); void ext_2e(); void ext_2f();
    void ext_30(); void ext_31(); void ext_32(); void ext_33(); void ext_34(); void ext_35(); void ext_36(); void ext_37();
    void ext_38(); void ext_39(); void ext_3a(); void ext_3b(); void ext_3c(); void ext_3d(); void ext_3e(); void ext_3f();
    void ext_40(); void ext_41(); void ext_42(); void ext_43(); void ext_44(); void ext_45(); void ext_46(); void ext_47();
    void ext_48(); void ext_49(); void ext_4a(); void ext_4b(); void ext_4c(); void ext_4d(); void ext_4e(); void ext_4f();
    void ext_50(); void ext_51(); void ext_52(); void ext_53(); void ext_54(); void ext_55(); void ext_56(); void ext_57();
    void ext_58(); void ext_59(); void ext_5a(); void ext_5b(); void ext_5c(); void ext_5d(); void ext_5e(); void ext_5f();
    void ext_60(); void ext_61(); void ext_62(); void ext_63(); void ext_64(); void ext_65(); void ext_66(); void ext_67();
    void ext_68(); void ext_69(); void ext_6a(); void ext_6b(); void ext_6c(); void ext_6d(); void ext_6e(); void ext_6f();
    void ext_70(); void ext_71(); void ext_72(); void ext_73(); void ext_74(); void ext_75(); void ext_76(); void ext_77();
    void ext_78(); void ext_79(); void ext_7a(); void ext_7b(); void ext_7c(); void ext_7d(); void ext_7e(); void ext_7f();
    void ext_80(); void ext_81(); void ext_82(); void ext_83(); void ext_84(); void ext_85(); void ext_86(); void ext_87();
    void ext_88(); void ext_89(); void ext_8a(); void ext_8b(); void ext_8c(); void ext_8d(); void ext_8e(); void ext_8f();
    void ext_90(); void ext_91(); void ext_92(); void ext_93(); void ext_94(); void ext_95(); void ext_96(); void ext_97();
    void ext_98(); void ext_99(); void ext_9a(); void ext_9b(); void ext_9c(); void ext_9d(); void ext_9e(); void ext_9f();
    void ext_a0(); void ext_a1(); void ext_a2(); void ext_a3(); void ext_a4(); void ext_a5(); void ext_a6(); void ext_a7();
    void ext_a8(); void ext_a9(); void ext_aa(); void ext_ab(); void ext_ac(); void ext_ad(); void ext_ae(); void ext_af();
    void ext_b0(); void ext_b1(); void ext_b2(); void ext_b3(); void ext_b4(); void ext_b5(); void ext_b6(); void ext_b7();
    void ext_b8(); void ext_b9(); void ext_ba(); void ext_bb(); void ext_bc(); void ext_bd(); void ext_be(); void ext_bf();
    void ext_c0(); void ext_c1(); void ext_c2(); void ext_c3(); void ext_c4(); void ext_c5(); void ext_c6(); void ext_c7();
    void ext_c8(); void ext_c9(); void ext_ca(); void ext_cb(); void ext_cc(); void ext_cd(); void ext_ce(); void ext_cf();
    void ext_d0(); void ext_d1(); void ext_d2(); void ext_d3(); void ext_d4(); void ext_d5(); void ext_d6(); void ext_d7();
    void ext_d8(); void ext_d9(); void ext_da(); void ext_db(); void ext_dc(); void ext_dd(); void ext_de(); void ext_df();
    void ext_e0(); void ext_e1(); void ext_e2(); void ext_e3(); void ext_e4(); void ext_e5(); void ext_e6(); void ext_e7();
    void ext_e8(); void ext_e9(); void ext_ea(); void ext_eb(); void ext_ec(); void ext_ed(); void ext_ee(); void ext_ef();
    void ext_f0(); void ext_f1(); void ext_f2(); void ext_f3(); void ext_f4(); void ext_f5(); void ext_f6(); void ext_f7();
    void ext_f8(); void ext_f9(); void ext_fa(); void ext_fb(); void ext_fc(); void ext_fd(); void ext_fe(); void ext_ff();

    const uint8_t instructionTicks[256] = {
        4,  12, 8, 8,  4,  4,  8,  4,  20, 8,  8,  8, 4, 4,  8, 4,   // 0x0_
        4,  12, 8, 8,  4,  4,  8,  4,  12, 8,  8,  8, 4, 4,  8, 4,   // 0x1_
        0,  12, 8, 8,  4,  4,  8,  4,  0,  8,  8,  8, 4, 4,  8, 4,   // 0x2_
        0,  12, 8, 8,  12, 12, 12, 4,  0,  8,  8,  8, 4, 4,  8, 4,   // 0x3_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x4_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x5_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x6_
        8,  8,  8, 8,  8,  8,  4,  8,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x7_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x8_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0x9_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0xa_
        4,  4,  4, 4,  4,  4,  8,  4,  4,  4,  4,  4, 4, 4,  8, 4,   // 0xb_
        0,  12, 0, 16, 0,  16, 8,  16, 0,  16, 0,  0, 0, 24, 8, 16,  // 0xc_
        0,  12, 0, 0,  0,  16, 8,  16, 0,  16, 0,  0, 0, 0,  8, 16,  // 0xd_
        12, 12, 8, 0,  0,  16, 8,  16, 16, 4,  16, 0, 0, 0,  8, 16,  // 0xe_
        12, 12, 8, 4,  0,  16, 8,  16, 12, 8,  16, 4, 0, 0,  8, 16   // 0xf_
    };
    const uint8_t extendedInstructionTicks[256] = {
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x0_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x1_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x2_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x3_
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,  // 0x4_
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,  // 0x5_
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,  // 0x6_
        8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,  // 0x7_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x8_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0x9_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0xa_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0xb_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0xc_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0xd_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,  // 0xe_
        8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8   // 0xf_
    };

   public:
    InstructionSet(Registers *regs, Interrupts *interrupts, Memory *memory);
    void execute(uint8_t opcode);

    void ret(bool condition);
    void xor_(uint8_t value);
    void inc(uint8_t *value);
    void dec(uint8_t *value);
    void add(uint8_t *destination, uint8_t value);
    void add(uint16_t *destination, uint16_t value);
    void add(uint16_t *destination, int8_t value);
    void ldhl(int8_t value);
    void adc(uint8_t value);
    void sbc(uint8_t value);
    void sub(uint8_t value);
    void and_(uint8_t value);
    void or_(uint8_t value);
    void cp(uint8_t value);
    void call(bool condition);
    void jump(bool condition);
    void jump_add(bool condition);
    void cp_n(uint8_t value);


    // Extended instructions
    void extended_execute(uint8_t opcode);
    void bit(uint8_t bit, uint8_t value);
    void res(uint8_t bit, uint8_t *rgst);
    void set(uint8_t bit, uint8_t *rgst);
    void rl(uint8_t *value);
    void rlc(uint8_t *value);
    void rr(uint8_t *value);
    void rrc(uint8_t *value);
    void sla(uint8_t *value);
    void sra(uint8_t *value);
    void srl(uint8_t *value);
    void swap(uint8_t *value);
};
