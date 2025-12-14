#include "instructions.h"
#include "cpu.h"

InstructionSet::InstructionSet(Registers *regs, Interrupts *interrupts, Memory *memory) {
    this->regs = regs;
    this->memory = memory;
    this->interrupts = interrupts;

    // Initialize function pointer table
    instruction_table[0x00] = &InstructionSet::op_00; instruction_table[0x01] = &InstructionSet::op_01;
    instruction_table[0x02] = &InstructionSet::op_02; instruction_table[0x03] = &InstructionSet::op_03;
    instruction_table[0x04] = &InstructionSet::op_04; instruction_table[0x05] = &InstructionSet::op_05;
    instruction_table[0x06] = &InstructionSet::op_06; instruction_table[0x07] = &InstructionSet::op_07;
    instruction_table[0x08] = &InstructionSet::op_08; instruction_table[0x09] = &InstructionSet::op_09;
    instruction_table[0x0a] = &InstructionSet::op_0a; instruction_table[0x0b] = &InstructionSet::op_0b;
    instruction_table[0x0c] = &InstructionSet::op_0c; instruction_table[0x0d] = &InstructionSet::op_0d;
    instruction_table[0x0e] = &InstructionSet::op_0e; instruction_table[0x0f] = &InstructionSet::op_0f;
    instruction_table[0x10] = &InstructionSet::op_10; instruction_table[0x11] = &InstructionSet::op_11;
    instruction_table[0x12] = &InstructionSet::op_12; instruction_table[0x13] = &InstructionSet::op_13;
    instruction_table[0x14] = &InstructionSet::op_14; instruction_table[0x15] = &InstructionSet::op_15;
    instruction_table[0x16] = &InstructionSet::op_16; instruction_table[0x17] = &InstructionSet::op_17;
    instruction_table[0x18] = &InstructionSet::op_18; instruction_table[0x19] = &InstructionSet::op_19;
    instruction_table[0x1a] = &InstructionSet::op_1a; instruction_table[0x1b] = &InstructionSet::op_1b;
    instruction_table[0x1c] = &InstructionSet::op_1c; instruction_table[0x1d] = &InstructionSet::op_1d;
    instruction_table[0x1e] = &InstructionSet::op_1e; instruction_table[0x1f] = &InstructionSet::op_1f;
    instruction_table[0x20] = &InstructionSet::op_20; instruction_table[0x21] = &InstructionSet::op_21;
    instruction_table[0x22] = &InstructionSet::op_22; instruction_table[0x23] = &InstructionSet::op_23;
    instruction_table[0x24] = &InstructionSet::op_24; instruction_table[0x25] = &InstructionSet::op_25;
    instruction_table[0x26] = &InstructionSet::op_26; instruction_table[0x27] = &InstructionSet::op_27;
    instruction_table[0x28] = &InstructionSet::op_28; instruction_table[0x29] = &InstructionSet::op_29;
    instruction_table[0x2a] = &InstructionSet::op_2a; instruction_table[0x2b] = &InstructionSet::op_2b;
    instruction_table[0x2c] = &InstructionSet::op_2c; instruction_table[0x2d] = &InstructionSet::op_2d;
    instruction_table[0x2e] = &InstructionSet::op_2e; instruction_table[0x2f] = &InstructionSet::op_2f;
    instruction_table[0x30] = &InstructionSet::op_30; instruction_table[0x31] = &InstructionSet::op_31;
    instruction_table[0x32] = &InstructionSet::op_32; instruction_table[0x33] = &InstructionSet::op_33;
    instruction_table[0x34] = &InstructionSet::op_34; instruction_table[0x35] = &InstructionSet::op_35;
    instruction_table[0x36] = &InstructionSet::op_36; instruction_table[0x37] = &InstructionSet::op_37;
    instruction_table[0x38] = &InstructionSet::op_38; instruction_table[0x39] = &InstructionSet::op_39;
    instruction_table[0x3a] = &InstructionSet::op_3a; instruction_table[0x3b] = &InstructionSet::op_3b;
    instruction_table[0x3c] = &InstructionSet::op_3c; instruction_table[0x3d] = &InstructionSet::op_3d;
    instruction_table[0x3e] = &InstructionSet::op_3e; instruction_table[0x3f] = &InstructionSet::op_3f;
    instruction_table[0x40] = &InstructionSet::op_40; instruction_table[0x41] = &InstructionSet::op_41;
    instruction_table[0x42] = &InstructionSet::op_42; instruction_table[0x43] = &InstructionSet::op_43;
    instruction_table[0x44] = &InstructionSet::op_44; instruction_table[0x45] = &InstructionSet::op_45;
    instruction_table[0x46] = &InstructionSet::op_46; instruction_table[0x47] = &InstructionSet::op_47;
    instruction_table[0x48] = &InstructionSet::op_48; instruction_table[0x49] = &InstructionSet::op_49;
    instruction_table[0x4a] = &InstructionSet::op_4a; instruction_table[0x4b] = &InstructionSet::op_4b;
    instruction_table[0x4c] = &InstructionSet::op_4c; instruction_table[0x4d] = &InstructionSet::op_4d;
    instruction_table[0x4e] = &InstructionSet::op_4e; instruction_table[0x4f] = &InstructionSet::op_4f;
    instruction_table[0x50] = &InstructionSet::op_50; instruction_table[0x51] = &InstructionSet::op_51;
    instruction_table[0x52] = &InstructionSet::op_52; instruction_table[0x53] = &InstructionSet::op_53;
    instruction_table[0x54] = &InstructionSet::op_54; instruction_table[0x55] = &InstructionSet::op_55;
    instruction_table[0x56] = &InstructionSet::op_56; instruction_table[0x57] = &InstructionSet::op_57;
    instruction_table[0x58] = &InstructionSet::op_58; instruction_table[0x59] = &InstructionSet::op_59;
    instruction_table[0x5a] = &InstructionSet::op_5a; instruction_table[0x5b] = &InstructionSet::op_5b;
    instruction_table[0x5c] = &InstructionSet::op_5c; instruction_table[0x5d] = &InstructionSet::op_5d;
    instruction_table[0x5e] = &InstructionSet::op_5e; instruction_table[0x5f] = &InstructionSet::op_5f;
    instruction_table[0x60] = &InstructionSet::op_60; instruction_table[0x61] = &InstructionSet::op_61;
    instruction_table[0x62] = &InstructionSet::op_62; instruction_table[0x63] = &InstructionSet::op_63;
    instruction_table[0x64] = &InstructionSet::op_64; instruction_table[0x65] = &InstructionSet::op_65;
    instruction_table[0x66] = &InstructionSet::op_66; instruction_table[0x67] = &InstructionSet::op_67;
    instruction_table[0x68] = &InstructionSet::op_68; instruction_table[0x69] = &InstructionSet::op_69;
    instruction_table[0x6a] = &InstructionSet::op_6a; instruction_table[0x6b] = &InstructionSet::op_6b;
    instruction_table[0x6c] = &InstructionSet::op_6c; instruction_table[0x6d] = &InstructionSet::op_6d;
    instruction_table[0x6e] = &InstructionSet::op_6e; instruction_table[0x6f] = &InstructionSet::op_6f;
    instruction_table[0x70] = &InstructionSet::op_70; instruction_table[0x71] = &InstructionSet::op_71;
    instruction_table[0x72] = &InstructionSet::op_72; instruction_table[0x73] = &InstructionSet::op_73;
    instruction_table[0x74] = &InstructionSet::op_74; instruction_table[0x75] = &InstructionSet::op_75;
    instruction_table[0x76] = &InstructionSet::op_76; instruction_table[0x77] = &InstructionSet::op_77;
    instruction_table[0x78] = &InstructionSet::op_78; instruction_table[0x79] = &InstructionSet::op_79;
    instruction_table[0x7a] = &InstructionSet::op_7a; instruction_table[0x7b] = &InstructionSet::op_7b;
    instruction_table[0x7c] = &InstructionSet::op_7c; instruction_table[0x7d] = &InstructionSet::op_7d;
    instruction_table[0x7e] = &InstructionSet::op_7e; instruction_table[0x7f] = &InstructionSet::op_7f;
    instruction_table[0x80] = &InstructionSet::op_80; instruction_table[0x81] = &InstructionSet::op_81;
    instruction_table[0x82] = &InstructionSet::op_82; instruction_table[0x83] = &InstructionSet::op_83;
    instruction_table[0x84] = &InstructionSet::op_84; instruction_table[0x85] = &InstructionSet::op_85;
    instruction_table[0x86] = &InstructionSet::op_86; instruction_table[0x87] = &InstructionSet::op_87;
    instruction_table[0x88] = &InstructionSet::op_88; instruction_table[0x89] = &InstructionSet::op_89;
    instruction_table[0x8a] = &InstructionSet::op_8a; instruction_table[0x8b] = &InstructionSet::op_8b;
    instruction_table[0x8c] = &InstructionSet::op_8c; instruction_table[0x8d] = &InstructionSet::op_8d;
    instruction_table[0x8e] = &InstructionSet::op_8e; instruction_table[0x8f] = &InstructionSet::op_8f;
    instruction_table[0x90] = &InstructionSet::op_90; instruction_table[0x91] = &InstructionSet::op_91;
    instruction_table[0x92] = &InstructionSet::op_92; instruction_table[0x93] = &InstructionSet::op_93;
    instruction_table[0x94] = &InstructionSet::op_94; instruction_table[0x95] = &InstructionSet::op_95;
    instruction_table[0x96] = &InstructionSet::op_96; instruction_table[0x97] = &InstructionSet::op_97;
    instruction_table[0x98] = &InstructionSet::op_98; instruction_table[0x99] = &InstructionSet::op_99;
    instruction_table[0x9a] = &InstructionSet::op_9a; instruction_table[0x9b] = &InstructionSet::op_9b;
    instruction_table[0x9c] = &InstructionSet::op_9c; instruction_table[0x9d] = &InstructionSet::op_9d;
    instruction_table[0x9e] = &InstructionSet::op_9e; instruction_table[0x9f] = &InstructionSet::op_9f;
    instruction_table[0xa0] = &InstructionSet::op_a0; instruction_table[0xa1] = &InstructionSet::op_a1;
    instruction_table[0xa2] = &InstructionSet::op_a2; instruction_table[0xa3] = &InstructionSet::op_a3;
    instruction_table[0xa4] = &InstructionSet::op_a4; instruction_table[0xa5] = &InstructionSet::op_a5;
    instruction_table[0xa6] = &InstructionSet::op_a6; instruction_table[0xa7] = &InstructionSet::op_a7;
    instruction_table[0xa8] = &InstructionSet::op_a8; instruction_table[0xa9] = &InstructionSet::op_a9;
    instruction_table[0xaa] = &InstructionSet::op_aa; instruction_table[0xab] = &InstructionSet::op_ab;
    instruction_table[0xac] = &InstructionSet::op_ac; instruction_table[0xad] = &InstructionSet::op_ad;
    instruction_table[0xae] = &InstructionSet::op_ae; instruction_table[0xaf] = &InstructionSet::op_af;
    instruction_table[0xb0] = &InstructionSet::op_b0; instruction_table[0xb1] = &InstructionSet::op_b1;
    instruction_table[0xb2] = &InstructionSet::op_b2; instruction_table[0xb3] = &InstructionSet::op_b3;
    instruction_table[0xb4] = &InstructionSet::op_b4; instruction_table[0xb5] = &InstructionSet::op_b5;
    instruction_table[0xb6] = &InstructionSet::op_b6; instruction_table[0xb7] = &InstructionSet::op_b7;
    instruction_table[0xb8] = &InstructionSet::op_b8; instruction_table[0xb9] = &InstructionSet::op_b9;
    instruction_table[0xba] = &InstructionSet::op_ba; instruction_table[0xbb] = &InstructionSet::op_bb;
    instruction_table[0xbc] = &InstructionSet::op_bc; instruction_table[0xbd] = &InstructionSet::op_bd;
    instruction_table[0xbe] = &InstructionSet::op_be; instruction_table[0xbf] = &InstructionSet::op_bf;
    instruction_table[0xc0] = &InstructionSet::op_c0; instruction_table[0xc1] = &InstructionSet::op_c1;
    instruction_table[0xc2] = &InstructionSet::op_c2; instruction_table[0xc3] = &InstructionSet::op_c3;
    instruction_table[0xc4] = &InstructionSet::op_c4; instruction_table[0xc5] = &InstructionSet::op_c5;
    instruction_table[0xc6] = &InstructionSet::op_c6; instruction_table[0xc7] = &InstructionSet::op_c7;
    instruction_table[0xc8] = &InstructionSet::op_c8; instruction_table[0xc9] = &InstructionSet::op_c9;
    instruction_table[0xca] = &InstructionSet::op_ca; instruction_table[0xcb] = &InstructionSet::op_cb;
    instruction_table[0xcc] = &InstructionSet::op_cc; instruction_table[0xcd] = &InstructionSet::op_cd;
    instruction_table[0xce] = &InstructionSet::op_ce; instruction_table[0xcf] = &InstructionSet::op_cf;
    instruction_table[0xd0] = &InstructionSet::op_d0; instruction_table[0xd1] = &InstructionSet::op_d1;
    instruction_table[0xd2] = &InstructionSet::op_d2; instruction_table[0xd3] = &InstructionSet::op_d3;
    instruction_table[0xd4] = &InstructionSet::op_d4; instruction_table[0xd5] = &InstructionSet::op_d5;
    instruction_table[0xd6] = &InstructionSet::op_d6; instruction_table[0xd7] = &InstructionSet::op_d7;
    instruction_table[0xd8] = &InstructionSet::op_d8; instruction_table[0xd9] = &InstructionSet::op_d9;
    instruction_table[0xda] = &InstructionSet::op_da; instruction_table[0xdb] = &InstructionSet::op_db;
    instruction_table[0xdc] = &InstructionSet::op_dc; instruction_table[0xdd] = &InstructionSet::op_dd;
    instruction_table[0xde] = &InstructionSet::op_de; instruction_table[0xdf] = &InstructionSet::op_df;
    instruction_table[0xe0] = &InstructionSet::op_e0; instruction_table[0xe1] = &InstructionSet::op_e1;
    instruction_table[0xe2] = &InstructionSet::op_e2; instruction_table[0xe3] = &InstructionSet::op_e3;
    instruction_table[0xe4] = &InstructionSet::op_e4; instruction_table[0xe5] = &InstructionSet::op_e5;
    instruction_table[0xe6] = &InstructionSet::op_e6; instruction_table[0xe7] = &InstructionSet::op_e7;
    instruction_table[0xe8] = &InstructionSet::op_e8; instruction_table[0xe9] = &InstructionSet::op_e9;
    instruction_table[0xea] = &InstructionSet::op_ea; instruction_table[0xeb] = &InstructionSet::op_eb;
    instruction_table[0xec] = &InstructionSet::op_ec; instruction_table[0xed] = &InstructionSet::op_ed;
    instruction_table[0xee] = &InstructionSet::op_ee; instruction_table[0xef] = &InstructionSet::op_ef;
    instruction_table[0xf0] = &InstructionSet::op_f0; instruction_table[0xf1] = &InstructionSet::op_f1;
    instruction_table[0xf2] = &InstructionSet::op_f2; instruction_table[0xf3] = &InstructionSet::op_f3;
    instruction_table[0xf4] = &InstructionSet::op_f4; instruction_table[0xf5] = &InstructionSet::op_f5;
    instruction_table[0xf6] = &InstructionSet::op_f6; instruction_table[0xf7] = &InstructionSet::op_f7;
    instruction_table[0xf8] = &InstructionSet::op_f8; instruction_table[0xf9] = &InstructionSet::op_f9;
    instruction_table[0xfa] = &InstructionSet::op_fa; instruction_table[0xfb] = &InstructionSet::op_fb;
    instruction_table[0xfc] = &InstructionSet::op_fc; instruction_table[0xfd] = &InstructionSet::op_fd;
    instruction_table[0xfe] = &InstructionSet::op_fe; instruction_table[0xff] = &InstructionSet::op_ff;

    // Initialize extended function pointer table
    extended_instruction_table[0x00] = &InstructionSet::ext_00; extended_instruction_table[0x01] = &InstructionSet::ext_01;
    extended_instruction_table[0x02] = &InstructionSet::ext_02; extended_instruction_table[0x03] = &InstructionSet::ext_03;
    extended_instruction_table[0x04] = &InstructionSet::ext_04; extended_instruction_table[0x05] = &InstructionSet::ext_05;
    extended_instruction_table[0x06] = &InstructionSet::ext_06; extended_instruction_table[0x07] = &InstructionSet::ext_07;
    extended_instruction_table[0x08] = &InstructionSet::ext_08; extended_instruction_table[0x09] = &InstructionSet::ext_09;
    extended_instruction_table[0x0a] = &InstructionSet::ext_0a; extended_instruction_table[0x0b] = &InstructionSet::ext_0b;
    extended_instruction_table[0x0c] = &InstructionSet::ext_0c; extended_instruction_table[0x0d] = &InstructionSet::ext_0d;
    extended_instruction_table[0x0e] = &InstructionSet::ext_0e; extended_instruction_table[0x0f] = &InstructionSet::ext_0f;
    extended_instruction_table[0x10] = &InstructionSet::ext_10; extended_instruction_table[0x11] = &InstructionSet::ext_11;
    extended_instruction_table[0x12] = &InstructionSet::ext_12; extended_instruction_table[0x13] = &InstructionSet::ext_13;
    extended_instruction_table[0x14] = &InstructionSet::ext_14; extended_instruction_table[0x15] = &InstructionSet::ext_15;
    extended_instruction_table[0x16] = &InstructionSet::ext_16; extended_instruction_table[0x17] = &InstructionSet::ext_17;
    extended_instruction_table[0x18] = &InstructionSet::ext_18; extended_instruction_table[0x19] = &InstructionSet::ext_19;
    extended_instruction_table[0x1a] = &InstructionSet::ext_1a; extended_instruction_table[0x1b] = &InstructionSet::ext_1b;
    extended_instruction_table[0x1c] = &InstructionSet::ext_1c; extended_instruction_table[0x1d] = &InstructionSet::ext_1d;
    extended_instruction_table[0x1e] = &InstructionSet::ext_1e; extended_instruction_table[0x1f] = &InstructionSet::ext_1f;
    extended_instruction_table[0x20] = &InstructionSet::ext_20; extended_instruction_table[0x21] = &InstructionSet::ext_21;
    extended_instruction_table[0x22] = &InstructionSet::ext_22; extended_instruction_table[0x23] = &InstructionSet::ext_23;
    extended_instruction_table[0x24] = &InstructionSet::ext_24; extended_instruction_table[0x25] = &InstructionSet::ext_25;
    extended_instruction_table[0x26] = &InstructionSet::ext_26; extended_instruction_table[0x27] = &InstructionSet::ext_27;
    extended_instruction_table[0x28] = &InstructionSet::ext_28; extended_instruction_table[0x29] = &InstructionSet::ext_29;
    extended_instruction_table[0x2a] = &InstructionSet::ext_2a; extended_instruction_table[0x2b] = &InstructionSet::ext_2b;
    extended_instruction_table[0x2c] = &InstructionSet::ext_2c; extended_instruction_table[0x2d] = &InstructionSet::ext_2d;
    extended_instruction_table[0x2e] = &InstructionSet::ext_2e; extended_instruction_table[0x2f] = &InstructionSet::ext_2f;
    extended_instruction_table[0x30] = &InstructionSet::ext_30; extended_instruction_table[0x31] = &InstructionSet::ext_31;
    extended_instruction_table[0x32] = &InstructionSet::ext_32; extended_instruction_table[0x33] = &InstructionSet::ext_33;
    extended_instruction_table[0x34] = &InstructionSet::ext_34; extended_instruction_table[0x35] = &InstructionSet::ext_35;
    extended_instruction_table[0x36] = &InstructionSet::ext_36; extended_instruction_table[0x37] = &InstructionSet::ext_37;
    extended_instruction_table[0x38] = &InstructionSet::ext_38; extended_instruction_table[0x39] = &InstructionSet::ext_39;
    extended_instruction_table[0x3a] = &InstructionSet::ext_3a; extended_instruction_table[0x3b] = &InstructionSet::ext_3b;
    extended_instruction_table[0x3c] = &InstructionSet::ext_3c; extended_instruction_table[0x3d] = &InstructionSet::ext_3d;
    extended_instruction_table[0x3e] = &InstructionSet::ext_3e; extended_instruction_table[0x3f] = &InstructionSet::ext_3f;
    extended_instruction_table[0x40] = &InstructionSet::ext_40; extended_instruction_table[0x41] = &InstructionSet::ext_41;
    extended_instruction_table[0x42] = &InstructionSet::ext_42; extended_instruction_table[0x43] = &InstructionSet::ext_43;
    extended_instruction_table[0x44] = &InstructionSet::ext_44; extended_instruction_table[0x45] = &InstructionSet::ext_45;
    extended_instruction_table[0x46] = &InstructionSet::ext_46; extended_instruction_table[0x47] = &InstructionSet::ext_47;
    extended_instruction_table[0x48] = &InstructionSet::ext_48; extended_instruction_table[0x49] = &InstructionSet::ext_49;
    extended_instruction_table[0x4a] = &InstructionSet::ext_4a; extended_instruction_table[0x4b] = &InstructionSet::ext_4b;
    extended_instruction_table[0x4c] = &InstructionSet::ext_4c; extended_instruction_table[0x4d] = &InstructionSet::ext_4d;
    extended_instruction_table[0x4e] = &InstructionSet::ext_4e; extended_instruction_table[0x4f] = &InstructionSet::ext_4f;
    extended_instruction_table[0x50] = &InstructionSet::ext_50; extended_instruction_table[0x51] = &InstructionSet::ext_51;
    extended_instruction_table[0x52] = &InstructionSet::ext_52; extended_instruction_table[0x53] = &InstructionSet::ext_53;
    extended_instruction_table[0x54] = &InstructionSet::ext_54; extended_instruction_table[0x55] = &InstructionSet::ext_55;
    extended_instruction_table[0x56] = &InstructionSet::ext_56; extended_instruction_table[0x57] = &InstructionSet::ext_57;
    extended_instruction_table[0x58] = &InstructionSet::ext_58; extended_instruction_table[0x59] = &InstructionSet::ext_59;
    extended_instruction_table[0x5a] = &InstructionSet::ext_5a; extended_instruction_table[0x5b] = &InstructionSet::ext_5b;
    extended_instruction_table[0x5c] = &InstructionSet::ext_5c; extended_instruction_table[0x5d] = &InstructionSet::ext_5d;
    extended_instruction_table[0x5e] = &InstructionSet::ext_5e; extended_instruction_table[0x5f] = &InstructionSet::ext_5f;
    extended_instruction_table[0x60] = &InstructionSet::ext_60; extended_instruction_table[0x61] = &InstructionSet::ext_61;
    extended_instruction_table[0x62] = &InstructionSet::ext_62; extended_instruction_table[0x63] = &InstructionSet::ext_63;
    extended_instruction_table[0x64] = &InstructionSet::ext_64; extended_instruction_table[0x65] = &InstructionSet::ext_65;
    extended_instruction_table[0x66] = &InstructionSet::ext_66; extended_instruction_table[0x67] = &InstructionSet::ext_67;
    extended_instruction_table[0x68] = &InstructionSet::ext_68; extended_instruction_table[0x69] = &InstructionSet::ext_69;
    extended_instruction_table[0x6a] = &InstructionSet::ext_6a; extended_instruction_table[0x6b] = &InstructionSet::ext_6b;
    extended_instruction_table[0x6c] = &InstructionSet::ext_6c; extended_instruction_table[0x6d] = &InstructionSet::ext_6d;
    extended_instruction_table[0x6e] = &InstructionSet::ext_6e; extended_instruction_table[0x6f] = &InstructionSet::ext_6f;
    extended_instruction_table[0x70] = &InstructionSet::ext_70; extended_instruction_table[0x71] = &InstructionSet::ext_71;
    extended_instruction_table[0x72] = &InstructionSet::ext_72; extended_instruction_table[0x73] = &InstructionSet::ext_73;
    extended_instruction_table[0x74] = &InstructionSet::ext_74; extended_instruction_table[0x75] = &InstructionSet::ext_75;
    extended_instruction_table[0x76] = &InstructionSet::ext_76; extended_instruction_table[0x77] = &InstructionSet::ext_77;
    extended_instruction_table[0x78] = &InstructionSet::ext_78; extended_instruction_table[0x79] = &InstructionSet::ext_79;
    extended_instruction_table[0x7a] = &InstructionSet::ext_7a; extended_instruction_table[0x7b] = &InstructionSet::ext_7b;
    extended_instruction_table[0x7c] = &InstructionSet::ext_7c; extended_instruction_table[0x7d] = &InstructionSet::ext_7d;
    extended_instruction_table[0x7e] = &InstructionSet::ext_7e; extended_instruction_table[0x7f] = &InstructionSet::ext_7f;
    extended_instruction_table[0x80] = &InstructionSet::ext_80; extended_instruction_table[0x81] = &InstructionSet::ext_81;
    extended_instruction_table[0x82] = &InstructionSet::ext_82; extended_instruction_table[0x83] = &InstructionSet::ext_83;
    extended_instruction_table[0x84] = &InstructionSet::ext_84; extended_instruction_table[0x85] = &InstructionSet::ext_85;
    extended_instruction_table[0x86] = &InstructionSet::ext_86; extended_instruction_table[0x87] = &InstructionSet::ext_87;
    extended_instruction_table[0x88] = &InstructionSet::ext_88; extended_instruction_table[0x89] = &InstructionSet::ext_89;
    extended_instruction_table[0x8a] = &InstructionSet::ext_8a; extended_instruction_table[0x8b] = &InstructionSet::ext_8b;
    extended_instruction_table[0x8c] = &InstructionSet::ext_8c; extended_instruction_table[0x8d] = &InstructionSet::ext_8d;
    extended_instruction_table[0x8e] = &InstructionSet::ext_8e; extended_instruction_table[0x8f] = &InstructionSet::ext_8f;
    extended_instruction_table[0x90] = &InstructionSet::ext_90; extended_instruction_table[0x91] = &InstructionSet::ext_91;
    extended_instruction_table[0x92] = &InstructionSet::ext_92; extended_instruction_table[0x93] = &InstructionSet::ext_93;
    extended_instruction_table[0x94] = &InstructionSet::ext_94; extended_instruction_table[0x95] = &InstructionSet::ext_95;
    extended_instruction_table[0x96] = &InstructionSet::ext_96; extended_instruction_table[0x97] = &InstructionSet::ext_97;
    extended_instruction_table[0x98] = &InstructionSet::ext_98; extended_instruction_table[0x99] = &InstructionSet::ext_99;
    extended_instruction_table[0x9a] = &InstructionSet::ext_9a; extended_instruction_table[0x9b] = &InstructionSet::ext_9b;
    extended_instruction_table[0x9c] = &InstructionSet::ext_9c; extended_instruction_table[0x9d] = &InstructionSet::ext_9d;
    extended_instruction_table[0x9e] = &InstructionSet::ext_9e; extended_instruction_table[0x9f] = &InstructionSet::ext_9f;
    extended_instruction_table[0xa0] = &InstructionSet::ext_a0; extended_instruction_table[0xa1] = &InstructionSet::ext_a1;
    extended_instruction_table[0xa2] = &InstructionSet::ext_a2; extended_instruction_table[0xa3] = &InstructionSet::ext_a3;
    extended_instruction_table[0xa4] = &InstructionSet::ext_a4; extended_instruction_table[0xa5] = &InstructionSet::ext_a5;
    extended_instruction_table[0xa6] = &InstructionSet::ext_a6; extended_instruction_table[0xa7] = &InstructionSet::ext_a7;
    extended_instruction_table[0xa8] = &InstructionSet::ext_a8; extended_instruction_table[0xa9] = &InstructionSet::ext_a9;
    extended_instruction_table[0xaa] = &InstructionSet::ext_aa; extended_instruction_table[0xab] = &InstructionSet::ext_ab;
    extended_instruction_table[0xac] = &InstructionSet::ext_ac; extended_instruction_table[0xad] = &InstructionSet::ext_ad;
    extended_instruction_table[0xae] = &InstructionSet::ext_ae; extended_instruction_table[0xaf] = &InstructionSet::ext_af;
    extended_instruction_table[0xb0] = &InstructionSet::ext_b0; extended_instruction_table[0xb1] = &InstructionSet::ext_b1;
    extended_instruction_table[0xb2] = &InstructionSet::ext_b2; extended_instruction_table[0xb3] = &InstructionSet::ext_b3;
    extended_instruction_table[0xb4] = &InstructionSet::ext_b4; extended_instruction_table[0xb5] = &InstructionSet::ext_b5;
    extended_instruction_table[0xb6] = &InstructionSet::ext_b6; extended_instruction_table[0xb7] = &InstructionSet::ext_b7;
    extended_instruction_table[0xb8] = &InstructionSet::ext_b8; extended_instruction_table[0xb9] = &InstructionSet::ext_b9;
    extended_instruction_table[0xba] = &InstructionSet::ext_ba; extended_instruction_table[0xbb] = &InstructionSet::ext_bb;
    extended_instruction_table[0xbc] = &InstructionSet::ext_bc; extended_instruction_table[0xbd] = &InstructionSet::ext_bd;
    extended_instruction_table[0xbe] = &InstructionSet::ext_be; extended_instruction_table[0xbf] = &InstructionSet::ext_bf;
    extended_instruction_table[0xc0] = &InstructionSet::ext_c0; extended_instruction_table[0xc1] = &InstructionSet::ext_c1;
    extended_instruction_table[0xc2] = &InstructionSet::ext_c2; extended_instruction_table[0xc3] = &InstructionSet::ext_c3;
    extended_instruction_table[0xc4] = &InstructionSet::ext_c4; extended_instruction_table[0xc5] = &InstructionSet::ext_c5;
    extended_instruction_table[0xc6] = &InstructionSet::ext_c6; extended_instruction_table[0xc7] = &InstructionSet::ext_c7;
    extended_instruction_table[0xc8] = &InstructionSet::ext_c8; extended_instruction_table[0xc9] = &InstructionSet::ext_c9;
    extended_instruction_table[0xca] = &InstructionSet::ext_ca; extended_instruction_table[0xcb] = &InstructionSet::ext_cb;
    extended_instruction_table[0xcc] = &InstructionSet::ext_cc; extended_instruction_table[0xcd] = &InstructionSet::ext_cd;
    extended_instruction_table[0xce] = &InstructionSet::ext_ce; extended_instruction_table[0xcf] = &InstructionSet::ext_cf;
    extended_instruction_table[0xd0] = &InstructionSet::ext_d0; extended_instruction_table[0xd1] = &InstructionSet::ext_d1;
    extended_instruction_table[0xd2] = &InstructionSet::ext_d2; extended_instruction_table[0xd3] = &InstructionSet::ext_d3;
    extended_instruction_table[0xd4] = &InstructionSet::ext_d4; extended_instruction_table[0xd5] = &InstructionSet::ext_d5;
    extended_instruction_table[0xd6] = &InstructionSet::ext_d6; extended_instruction_table[0xd7] = &InstructionSet::ext_d7;
    extended_instruction_table[0xd8] = &InstructionSet::ext_d8; extended_instruction_table[0xd9] = &InstructionSet::ext_d9;
    extended_instruction_table[0xda] = &InstructionSet::ext_da; extended_instruction_table[0xdb] = &InstructionSet::ext_db;
    extended_instruction_table[0xdc] = &InstructionSet::ext_dc; extended_instruction_table[0xdd] = &InstructionSet::ext_dd;
    extended_instruction_table[0xde] = &InstructionSet::ext_de; extended_instruction_table[0xdf] = &InstructionSet::ext_df;
    extended_instruction_table[0xe0] = &InstructionSet::ext_e0; extended_instruction_table[0xe1] = &InstructionSet::ext_e1;
    extended_instruction_table[0xe2] = &InstructionSet::ext_e2; extended_instruction_table[0xe3] = &InstructionSet::ext_e3;
    extended_instruction_table[0xe4] = &InstructionSet::ext_e4; extended_instruction_table[0xe5] = &InstructionSet::ext_e5;
    extended_instruction_table[0xe6] = &InstructionSet::ext_e6; extended_instruction_table[0xe7] = &InstructionSet::ext_e7;
    extended_instruction_table[0xe8] = &InstructionSet::ext_e8; extended_instruction_table[0xe9] = &InstructionSet::ext_e9;
    extended_instruction_table[0xea] = &InstructionSet::ext_ea; extended_instruction_table[0xeb] = &InstructionSet::ext_eb;
    extended_instruction_table[0xec] = &InstructionSet::ext_ec; extended_instruction_table[0xed] = &InstructionSet::ext_ed;
    extended_instruction_table[0xee] = &InstructionSet::ext_ee; extended_instruction_table[0xef] = &InstructionSet::ext_ef;
    extended_instruction_table[0xf0] = &InstructionSet::ext_f0; extended_instruction_table[0xf1] = &InstructionSet::ext_f1;
    extended_instruction_table[0xf2] = &InstructionSet::ext_f2; extended_instruction_table[0xf3] = &InstructionSet::ext_f3;
    extended_instruction_table[0xf4] = &InstructionSet::ext_f4; extended_instruction_table[0xf5] = &InstructionSet::ext_f5;
    extended_instruction_table[0xf6] = &InstructionSet::ext_f6; extended_instruction_table[0xf7] = &InstructionSet::ext_f7;
    extended_instruction_table[0xf8] = &InstructionSet::ext_f8; extended_instruction_table[0xf9] = &InstructionSet::ext_f9;
    extended_instruction_table[0xfa] = &InstructionSet::ext_fa; extended_instruction_table[0xfb] = &InstructionSet::ext_fb;
    extended_instruction_table[0xfc] = &InstructionSet::ext_fc; extended_instruction_table[0xfd] = &InstructionSet::ext_fd;
    extended_instruction_table[0xfe] = &InstructionSet::ext_fe; extended_instruction_table[0xff] = &InstructionSet::ext_ff;
}

void InstructionSet::execute(uint8_t opcode) {
    memory->cpu_clock.instruction_cycles += instructionTicks[opcode];
    
    if (instruction_table[opcode] != nullptr) {
        (this->*instruction_table[opcode])();
    } else {
        std::cout << "FF40: " << +(memory->read_memory_byte(0xFF40)) << " FF41: " << +(memory->read_memory_byte(0xFF41))
                  << " FF42: " << +(memory->read_memory_byte(0xFF42)) << " FF44: " << +(memory->read_memory_byte(0xFF44)) << std::endl;
        regs->print_flags();
        regs->print_registers();
        printf("Unsupported opcode: 0x%02x at 0x%04x\n", opcode, this->regs->pc);
        printf("DIV: %d\n", memory->timer_registers.div_counter);
        printf("Cycles: %d \n\n\n", memory->cpu_clock.total_cycles);
        return;
    }
}

void InstructionSet::write_short_stack(uint16_t value) { memory->write_stack_short(&regs->sp, value); }

uint16_t InstructionSet::read_short_stack() { return memory->read_stack_short(&regs->sp); }

void InstructionSet::call(bool condition) {
    uint16_t operand = memory->read_memory_short(regs->pc);
    regs->pc += 2;

    memory->cpu_clock.instruction_cycles += 12;
    if (condition) {
        memory->write_stack_short(&regs->sp, regs->pc);
        regs->pc = operand;
        memory->cpu_clock.instruction_cycles += 12;
    }
}

void InstructionSet::ret(bool condition) {
    if (condition) {
        regs->pc = memory->read_stack_short(&regs->sp);
        memory->cpu_clock.instruction_cycles += 20;
    } else {
        memory->cpu_clock.instruction_cycles += 8;
    }
}

void InstructionSet::jump_add(bool condition) {
    if (condition) {
        regs->pc += 1 + (int8_t)(memory->read_memory_byte(regs->pc));
        memory->cpu_clock.instruction_cycles += 12;
    } else {
        regs->pc++;
        memory->cpu_clock.instruction_cycles += 8;
    }
}

void InstructionSet::jump(bool condition) {
    if (condition) {
        regs->pc = memory->read_memory_short(regs->pc);
        memory->cpu_clock.instruction_cycles += 16;
    } else {
        regs->pc += 2;
        memory->cpu_clock.instruction_cycles += 12;
    }
}
void InstructionSet::inc(uint8_t *value) {
    regs->set_flags(Flags::HALF_CARRY, (*value & 0x0f) == 0x0f);

    *value += 1;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT, false);
}

void InstructionSet::dec(uint8_t *value) {
    regs->set_flags(Flags::HALF_CARRY, !(*value & 0x0f));

    *value -= 1;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT, true);
}

void InstructionSet::cp(uint8_t value) {
    uint8_t temp_val = regs->a;
    regs->set_flags(Flags::CARRY, value > temp_val);
    regs->set_flags(Flags::HALF_CARRY, (value & 0x0f) > (temp_val & 0x0f));

    temp_val -= value;

    regs->set_flags(Flags::ZERO, !temp_val);
    regs->set_flags(Flags::SUBTRACT, true);
}

void InstructionSet::add(uint8_t *destination, uint8_t value) {
    uint16_t result = *destination + value;

    regs->set_flags(Flags::CARRY, result > 0xff);
    regs->set_flags(Flags::HALF_CARRY, ((*destination & 0x0f) + (value & 0x0f)) > 0x0f);

    *destination = result;

    regs->set_flags(Flags::ZERO, !*destination);
    regs->set_flags(Flags::SUBTRACT, false);
}

void InstructionSet::add(uint16_t *destination, uint16_t value) {
    uint32_t result = *destination + value;

    regs->set_flags(Flags::CARRY, result > 0xffff);
    regs->set_flags(Flags::HALF_CARRY, ((*destination & 0x0fff) + (value & 0x0fff)) > 0x0fff);

    *destination = (uint16_t)result;

    regs->set_flags(Flags::SUBTRACT, false);
}

void InstructionSet::add(uint16_t *destination, int8_t value) {
    uint16_t result = *destination + value;

    regs->set_flags(Flags::CARRY, ((regs->sp ^ value ^ (result & 0xFFFF)) & 0x100) == 0x100);
    regs->set_flags(Flags::HALF_CARRY, ((regs->sp ^ value ^ (result & 0xFFFF)) & 0x10) == 0x10);

    *destination = result & 0xFFFF;

    regs->set_flags(Flags::SUBTRACT | Flags::ZERO, false);
}
void InstructionSet::ldhl(int8_t value) {
    uint16_t result = regs->sp + value;

    regs->set_flags(Flags::CARRY, ((regs->sp ^ value ^ result) & 0x100) == 0x100);
    regs->set_flags(Flags::HALF_CARRY, ((regs->sp ^ value ^ result) & 0x10) == 0x10);

    regs->hl = result;

    regs->set_flags(Flags::SUBTRACT | Flags::ZERO, false);
}

void InstructionSet::adc(uint8_t value) {
    int carry = regs->is_flag_set(Flags::CARRY) ? 1 : 0;
    int result = regs->a + value + carry;

    regs->set_flags(Flags::ZERO, !(int8_t)result);
    regs->set_flags(Flags::CARRY, result > 0xff);
    regs->set_flags(Flags::HALF_CARRY, ((regs->a & 0x0F) + (value & 0x0f) + carry) > 0x0F);
    regs->set_flags(Flags::SUBTRACT, false);

    regs->a = (int8_t)(result & 0xff);
}

void InstructionSet::and_(uint8_t value) {
    regs->a = regs->a & value;

    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::HALF_CARRY, true);
    regs->set_flags(Flags::SUBTRACT | Flags::CARRY, false);
}

void InstructionSet::or_(uint8_t value) {
    regs->a |= value;

    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::CARRY | Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::sub(uint8_t value) {
    regs->set_flags(Flags::CARRY, value > regs->a);
    regs->set_flags(Flags::HALF_CARRY, (value & 0x0f) > (regs->a & 0x0f));

    regs->a -= value;

    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::SUBTRACT, true);
}

void InstructionSet::sbc(uint8_t value) {
    bool is_carry = regs->is_flag_set(Flags::CARRY);

    regs->set_flags(Flags::CARRY, (value + is_carry) > regs->a);
    regs->set_flags(Flags::HALF_CARRY, ((value & 0x0f) + is_carry) > (regs->a & 0x0f));

    regs->a -= (value + is_carry);

    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::SUBTRACT, true);
}
void InstructionSet::xor_(uint8_t value) {
    regs->a ^= value;

    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::CARRY | Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::cp_n(uint8_t operand) {
    regs->set_flags(Flags::SUBTRACT, true);
    regs->set_flags(Flags::ZERO, regs->a == operand);
    regs->set_flags(Flags::CARRY, operand > regs->a);
    regs->set_flags(Flags::HALF_CARRY, (operand & 0x0f) > (regs->a & 0x0f));
}

// Individual instruction methods
void InstructionSet::op_00() { }
void InstructionSet::op_01() { regs->bc = memory->read_memory_short(regs->pc); regs->pc += 2; }
void InstructionSet::op_02() { memory->write_memory_byte(regs->bc, regs->a); }
void InstructionSet::op_03() { regs->bc++; }
void InstructionSet::op_04() { inc(&regs->b); }
void InstructionSet::op_05() { dec(&regs->b); }
void InstructionSet::op_06() { regs->b = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_07() { rlc(&regs->a); regs->set_flags(Flags::ZERO, false); }
void InstructionSet::op_08() { memory->write_memory_short(memory->read_memory_short(regs->pc), regs->sp); regs->pc += 2; }
void InstructionSet::op_09() { add(&regs->hl, regs->bc); }
void InstructionSet::op_0a() { regs->a = memory->read_memory_byte(regs->bc); }
void InstructionSet::op_0b() { regs->bc--; }
void InstructionSet::op_0c() { inc(&regs->c); }
void InstructionSet::op_0d() { dec(&regs->c); }
void InstructionSet::op_0e() { regs->c = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_0f() { rrc(&regs->a); regs->set_flags(Flags::ZERO, false); }
void InstructionSet::op_10() { }
void InstructionSet::op_11() { regs->de = memory->read_memory_short(regs->pc); regs->pc += 2; }
void InstructionSet::op_12() { memory->write_memory_byte(regs->de, regs->a); }
void InstructionSet::op_13() { regs->de++; }
void InstructionSet::op_14() { inc(&regs->d); }
void InstructionSet::op_15() { dec(&regs->d); }
void InstructionSet::op_16() { regs->d = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_17() { rl(&regs->a); regs->set_flags(Flags::ZERO, false); }
void InstructionSet::op_18() { uint8_t operand = memory->read_memory_byte(regs->pc++); regs->pc += (int8_t)(operand); }
void InstructionSet::op_19() { add(&regs->hl, regs->de); }
void InstructionSet::op_1a() { regs->a = memory->read_memory_byte(regs->de); }
void InstructionSet::op_1b() { regs->de--; }
void InstructionSet::op_1c() { inc(&regs->e); }
void InstructionSet::op_1d() { dec(&regs->e); }
void InstructionSet::op_1e() { regs->e = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_1f() { rr(&regs->a); regs->set_flags(Flags::ZERO, false); }
void InstructionSet::op_20() { jump_add(!regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_21() { regs->hl = memory->read_memory_short(regs->pc); regs->pc += 2; }
void InstructionSet::op_22() { memory->write_memory_byte(regs->hl++, regs->a); }
void InstructionSet::op_23() { regs->hl++; }
void InstructionSet::op_24() { inc(&regs->h); }
void InstructionSet::op_25() { dec(&regs->h); }
void InstructionSet::op_26() { regs->h = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_27() { 
    uint16_t value = regs->a;
    if (regs->is_flag_set(Flags::SUBTRACT)) {
        if (regs->is_flag_set(Flags::CARRY)) { value -= 0x60; }
        if (regs->is_flag_set(Flags::HALF_CARRY)) { value -= 0x6; }
    } else {
        if (regs->is_flag_set(Flags::CARRY) || value > 0x99) { value += 0x60; regs->set_flags(Flags::CARRY, true); }
        if (regs->is_flag_set(Flags::HALF_CARRY) || (value & 0xF) > 0x9) { value += 0x6; }
    }
    regs->a = value;
    regs->set_flags(Flags::ZERO, !regs->a);
    regs->set_flags(Flags::HALF_CARRY, false);
}
void InstructionSet::op_28() { jump_add(regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_29() { add(&regs->hl, regs->hl); }
void InstructionSet::op_2a() { regs->a = memory->read_memory_byte(regs->hl++); }
void InstructionSet::op_2b() { regs->hl--; }
void InstructionSet::op_2c() { inc(&regs->l); }
void InstructionSet::op_2d() { dec(&regs->l); }
void InstructionSet::op_2e() { regs->l = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_2f() { regs->a = ~regs->a; regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, true); }
void InstructionSet::op_30() { jump_add(!regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_31() { regs->sp = memory->read_memory_short(regs->pc); regs->pc += 2; }
void InstructionSet::op_32() { memory->write_memory_byte(regs->hl--, regs->a); }
void InstructionSet::op_33() { regs->sp++; }
void InstructionSet::op_34() { uint8_t tmp_val = memory->read_memory_byte(regs->hl); inc(&tmp_val); memory->write_memory_byte(regs->hl, tmp_val); }
void InstructionSet::op_35() { uint8_t tmp_val = memory->read_memory_byte(regs->hl); dec(&tmp_val); memory->write_memory_byte(regs->hl, tmp_val); }
void InstructionSet::op_36() { memory->write_memory_byte(regs->hl, memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_37() { regs->set_flags(Flags::CARRY, true); regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false); }
void InstructionSet::op_38() { jump_add(regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_39() { add(&regs->hl, regs->sp); }
void InstructionSet::op_3a() { regs->a = memory->read_memory_byte(regs->hl--); }
void InstructionSet::op_3b() { regs->sp--; }
void InstructionSet::op_3c() { inc(&regs->a); }
void InstructionSet::op_3d() { dec(&regs->a); }
void InstructionSet::op_3e() { regs->a = memory->read_memory_byte(regs->pc++); }
void InstructionSet::op_3f() { regs->set_flags(Flags::CARRY, !regs->is_flag_set(Flags::CARRY)); regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false); }
void InstructionSet::op_40() { }
void InstructionSet::op_41() { regs->b = regs->c; }
void InstructionSet::op_42() { regs->b = regs->d; }
void InstructionSet::op_43() { regs->b = regs->e; }
void InstructionSet::op_44() { regs->b = regs->h; }
void InstructionSet::op_45() { regs->b = regs->l; }
void InstructionSet::op_46() { regs->b = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_47() { regs->b = regs->a; }
void InstructionSet::op_48() { regs->c = regs->b; }
void InstructionSet::op_49() { }
void InstructionSet::op_4a() { regs->c = regs->d; }
void InstructionSet::op_4b() { regs->c = regs->e; }
void InstructionSet::op_4c() { regs->c = regs->h; }
void InstructionSet::op_4d() { regs->c = regs->l; }
void InstructionSet::op_4e() { regs->c = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_4f() { regs->c = regs->a; }
void InstructionSet::op_50() { regs->d = regs->b; }
void InstructionSet::op_51() { regs->d = regs->c; }
void InstructionSet::op_52() { }
void InstructionSet::op_53() { regs->d = regs->e; }
void InstructionSet::op_54() { regs->d = regs->h; }
void InstructionSet::op_55() { regs->d = regs->l; }
void InstructionSet::op_56() { regs->d = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_57() { regs->d = regs->a; }
void InstructionSet::op_58() { regs->e = regs->b; }
void InstructionSet::op_59() { regs->e = regs->c; }
void InstructionSet::op_5a() { regs->e = regs->d; }
void InstructionSet::op_5b() { }
void InstructionSet::op_5c() { regs->e = regs->h; }
void InstructionSet::op_5d() { regs->e = regs->l; }
void InstructionSet::op_5e() { regs->e = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_5f() { regs->e = regs->a; }
void InstructionSet::op_60() { regs->h = regs->b; }
void InstructionSet::op_61() { regs->h = regs->c; }
void InstructionSet::op_62() { regs->h = regs->d; }
void InstructionSet::op_63() { regs->h = regs->e; }
void InstructionSet::op_64() { }
void InstructionSet::op_65() { regs->h = regs->l; }
void InstructionSet::op_66() { regs->h = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_67() { regs->h = regs->a; }
void InstructionSet::op_68() { regs->l = regs->b; }
void InstructionSet::op_69() { regs->l = regs->c; }
void InstructionSet::op_6a() { regs->l = regs->d; }
void InstructionSet::op_6b() { regs->l = regs->e; }
void InstructionSet::op_6c() { regs->l = regs->h; }
void InstructionSet::op_6d() { }
void InstructionSet::op_6e() { regs->l = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_6f() { regs->l = regs->a; }
void InstructionSet::op_70() { memory->write_memory_byte(regs->hl, regs->b); }
void InstructionSet::op_71() { memory->write_memory_byte(regs->hl, regs->c); }
void InstructionSet::op_72() { memory->write_memory_byte(regs->hl, regs->d); }
void InstructionSet::op_73() { memory->write_memory_byte(regs->hl, regs->e); }
void InstructionSet::op_74() { memory->write_memory_byte(regs->hl, regs->h); }
void InstructionSet::op_75() { memory->write_memory_byte(regs->hl, regs->l); }
void InstructionSet::op_76() { 
    if (!interrupts->is_master_enabled() && (memory->read_memory_byte(0xFF0F) & memory->read_memory_byte(0xFFFF) & 0x1F)) {
        memory->memory_flags.halt_bug_triggered = true;
        memory->memory_flags.cpu_halted = false;
    } else
        memory->memory_flags.cpu_halted = true;
}
void InstructionSet::op_77() { memory->write_memory_byte(regs->hl, regs->a); }
void InstructionSet::op_78() { regs->a = regs->b; }
void InstructionSet::op_79() { regs->a = regs->c; }
void InstructionSet::op_7a() { regs->a = regs->d; }
void InstructionSet::op_7b() { regs->a = regs->e; }
void InstructionSet::op_7c() { regs->a = regs->h; }
void InstructionSet::op_7d() { regs->a = regs->l; }
void InstructionSet::op_7e() { regs->a = memory->read_memory_byte(regs->hl); }
void InstructionSet::op_7f() { }
void InstructionSet::op_80() { add(&regs->a, regs->b); }
void InstructionSet::op_81() { add(&regs->a, regs->c); }
void InstructionSet::op_82() { add(&regs->a, regs->d); }
void InstructionSet::op_83() { add(&regs->a, regs->e); }
void InstructionSet::op_84() { add(&regs->a, regs->h); }
void InstructionSet::op_85() { add(&regs->a, regs->l); }
void InstructionSet::op_86() { add(&regs->a, memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_87() { add(&regs->a, regs->a); }
void InstructionSet::op_88() { adc(regs->b); }
void InstructionSet::op_89() { adc(regs->c); }
void InstructionSet::op_8a() { adc(regs->d); }
void InstructionSet::op_8b() { adc(regs->e); }
void InstructionSet::op_8c() { adc(regs->h); }
void InstructionSet::op_8d() { adc(regs->l); }
void InstructionSet::op_8e() { adc(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_8f() { adc(regs->a); }
void InstructionSet::op_90() { sub(regs->b); }
void InstructionSet::op_91() { sub(regs->c); }
void InstructionSet::op_92() { sub(regs->d); }
void InstructionSet::op_93() { sub(regs->e); }
void InstructionSet::op_94() { sub(regs->h); }
void InstructionSet::op_95() { sub(regs->l); }
void InstructionSet::op_96() { sub(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_97() { sub(regs->a); }
void InstructionSet::op_98() { sbc(regs->b); }
void InstructionSet::op_99() { sbc(regs->c); }
void InstructionSet::op_9a() { sbc(regs->d); }
void InstructionSet::op_9b() { sbc(regs->e); }
void InstructionSet::op_9c() { sbc(regs->h); }
void InstructionSet::op_9d() { sbc(regs->l); }
void InstructionSet::op_9e() { sbc(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_9f() { sbc(regs->a); }
void InstructionSet::op_a0() { and_(regs->b); }
void InstructionSet::op_a1() { and_(regs->c); }
void InstructionSet::op_a2() { and_(regs->d); }
void InstructionSet::op_a3() { and_(regs->e); }
void InstructionSet::op_a4() { and_(regs->h); }
void InstructionSet::op_a5() { and_(regs->l); }
void InstructionSet::op_a6() { and_(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_a7() { and_(regs->a); }
void InstructionSet::op_a8() { xor_(regs->b); }
void InstructionSet::op_a9() { xor_(regs->c); }
void InstructionSet::op_aa() { xor_(regs->d); }
void InstructionSet::op_ab() { xor_(regs->e); }
void InstructionSet::op_ac() { xor_(regs->h); }
void InstructionSet::op_ad() { xor_(regs->l); }
void InstructionSet::op_ae() { xor_(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_af() { xor_(regs->a); }
void InstructionSet::op_b0() { or_(regs->b); }
void InstructionSet::op_b1() { or_(regs->c); }
void InstructionSet::op_b2() { or_(regs->d); }
void InstructionSet::op_b3() { or_(regs->e); }
void InstructionSet::op_b4() { or_(regs->h); }
void InstructionSet::op_b5() { or_(regs->l); }
void InstructionSet::op_b6() { or_(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_b7() { or_(regs->a); }
void InstructionSet::op_b8() { cp(regs->b); }
void InstructionSet::op_b9() { cp(regs->c); }
void InstructionSet::op_ba() { cp(regs->d); }
void InstructionSet::op_bb() { cp(regs->e); }
void InstructionSet::op_bc() { cp(regs->h); }
void InstructionSet::op_bd() { cp(regs->l); }
void InstructionSet::op_be() { cp(memory->read_memory_byte(regs->hl)); }
void InstructionSet::op_bf() { cp(regs->a); }
void InstructionSet::op_c0() { ret(!regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_c1() { regs->bc = memory->read_stack_short(&regs->sp); }
void InstructionSet::op_c2() { jump(!regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_c3() { regs->pc = memory->read_memory_short(regs->pc); }
void InstructionSet::op_c4() { call(!regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_c5() { memory->write_stack_short(&regs->sp, regs->bc); }
void InstructionSet::op_c6() { add(&regs->a, memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_c7() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0000; }
void InstructionSet::op_c8() { ret(regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_c9() { regs->pc = memory->read_stack_short(&regs->sp); }
void InstructionSet::op_ca() { jump(regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_cb() { extended_execute(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_cc() { call(regs->is_flag_set(Flags::ZERO)); }
void InstructionSet::op_cd() { 
    uint16_t operand = memory->read_memory_short(regs->pc);
    regs->pc += 2;
    memory->write_stack_short(&regs->sp, regs->pc);
    regs->pc = operand;
}
void InstructionSet::op_ce() { adc(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_cf() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0008; }
void InstructionSet::op_d0() { ret(!regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_d1() { regs->de = memory->read_stack_short(&regs->sp); }
void InstructionSet::op_d2() { jump(!regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_d3() { }
void InstructionSet::op_d4() { call(!regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_d5() { memory->write_stack_short(&regs->sp, regs->de); }
void InstructionSet::op_d6() { sub(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_d7() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0010; }
void InstructionSet::op_d8() { ret(regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_d9() { interrupts->set_master_flag(true); regs->pc = memory->read_stack_short(&regs->sp); }
void InstructionSet::op_da() { jump(regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_db() { }
void InstructionSet::op_dc() { call(regs->is_flag_set(Flags::CARRY)); }
void InstructionSet::op_dd() { }
void InstructionSet::op_de() { sbc(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_df() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0018; }
void InstructionSet::op_e0() { memory->write_memory_byte(0xff00 + memory->read_memory_byte(regs->pc++), regs->a); }
void InstructionSet::op_e1() { regs->hl = memory->read_stack_short(&regs->sp); }
void InstructionSet::op_e2() { memory->write_memory_byte(0xff00 + regs->c, regs->a); }
void InstructionSet::op_e3() { }
void InstructionSet::op_e4() { }
void InstructionSet::op_e5() { memory->write_stack_short(&regs->sp, regs->hl); }
void InstructionSet::op_e6() { and_(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_e7() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0020; }
void InstructionSet::op_e8() { add(&regs->sp, (int8_t)memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_e9() { regs->pc = regs->hl; }
void InstructionSet::op_ea() { memory->write_memory_byte(memory->read_memory_short(regs->pc), regs->a); regs->pc += 2; }
void InstructionSet::op_eb() { }
void InstructionSet::op_ec() { }
void InstructionSet::op_ed() { }
void InstructionSet::op_ee() { xor_(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_ef() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0028; }
void InstructionSet::op_f0() { regs->a = memory->read_memory_byte(0xff00 + memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_f1() { regs->af = memory->read_stack_short(&regs->sp); regs->f &= 0xf0; }
void InstructionSet::op_f2() { regs->a = memory->read_memory_byte(0xff00 + regs->c); }
void InstructionSet::op_f3() { interrupts->set_master_flag(false); }
void InstructionSet::op_f4() { }
void InstructionSet::op_f5() { memory->write_stack_short(&regs->sp, regs->af); }
void InstructionSet::op_f6() { or_(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_f7() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0030; }
void InstructionSet::op_f8() { ldhl(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_f9() { regs->sp = regs->hl; }
void InstructionSet::op_fa() { regs->a = memory->read_memory_byte(memory->read_memory_short(regs->pc)); regs->pc += 2; }
void InstructionSet::op_fb() { interrupts->set_master_flag(true); }
void InstructionSet::op_fc() { }
void InstructionSet::op_fd() { }
void InstructionSet::op_fe() { cp_n(memory->read_memory_byte(regs->pc++)); }
void InstructionSet::op_ff() { memory->write_stack_short(&regs->sp, regs->pc); regs->pc = 0x0038; }

void InstructionSet::extended_execute(uint8_t opcode) {
    memory->cpu_clock.instruction_cycles += extendedInstructionTicks[opcode];
    
    if (extended_instruction_table[opcode] != nullptr) {
        (this->*extended_instruction_table[opcode])();
    } else {
        printf("Unsupported CB opcode: 0x%02x at 0x%04x\n\n\n", opcode, this->regs->pc);
        return;
    }
}

void InstructionSet::bit(uint8_t bit, uint8_t value) {
    regs->set_flags(Flags::ZERO, !(value & bit));
    regs->set_flags(Flags::HALF_CARRY, true);
    regs->set_flags(Flags::SUBTRACT, false);
}

void InstructionSet::res(uint8_t bit, uint8_t *rgst) { *rgst &= ~(bit); }

void InstructionSet::set(uint8_t bit, uint8_t *rgst) { *rgst |= bit; }

void InstructionSet::rl(uint8_t *value) {
    int carry = regs->is_flag_set(Flags::CARRY);

    regs->set_flags(Flags::CARRY, *value & (1 << 7));

    *value <<= 1;
    *value += carry;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::rlc(uint8_t *value) {
    int carry = (*value >> 7) & 0x01;

    regs->set_flags(Flags::CARRY, *value & (1 << 7));

    *value <<= 1;
    *value += carry;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::rr(uint8_t *value) {
    int carry = regs->is_flag_set(Flags::CARRY);

    regs->set_flags(Flags::CARRY, *value & 0x01);

    *value >>= 1;
    *value |= (carry << 7);

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::rrc(uint8_t *value) {
    int carry = *value & 0x01;

    regs->set_flags(Flags::CARRY, carry);

    *value >>= 1;
    *value |= (carry << 7);

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::sla(uint8_t *value) {
    regs->set_flags(Flags::CARRY, *value & (1 << 7));

    *value <<= 1;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::sra(uint8_t *value) {
    regs->set_flags(Flags::CARRY, *value & 0x01);

    int msb = *value & (1 << 7);
    *value >>= 1;
    *value |= msb;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::srl(uint8_t *value) {
    regs->set_flags(Flags::CARRY, *value & 0x01);

    *value >>= 1;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY, false);
}

void InstructionSet::swap(uint8_t *value) {
    uint8_t lower = *value << 4;
    *value = (*value >> 4) | lower;

    regs->set_flags(Flags::ZERO, !*value);
    regs->set_flags(Flags::SUBTRACT | Flags::HALF_CARRY | Flags::CARRY, false);
}

// Individual extended instruction methods
void InstructionSet::ext_00() { rlc(&regs->b); }
void InstructionSet::ext_01() { rlc(&regs->c); }
void InstructionSet::ext_02() { rlc(&regs->d); }
void InstructionSet::ext_03() { rlc(&regs->e); }
void InstructionSet::ext_04() { rlc(&regs->h); }
void InstructionSet::ext_05() { rlc(&regs->l); }
void InstructionSet::ext_06() { uint8_t value = memory->read_memory_byte(regs->hl); rlc(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_07() { rlc(&regs->a); }
void InstructionSet::ext_08() { rrc(&regs->b); }
void InstructionSet::ext_09() { rrc(&regs->c); }
void InstructionSet::ext_0a() { rrc(&regs->d); }
void InstructionSet::ext_0b() { rrc(&regs->e); }
void InstructionSet::ext_0c() { rrc(&regs->h); }
void InstructionSet::ext_0d() { rrc(&regs->l); }
void InstructionSet::ext_0e() { uint8_t value = memory->read_memory_byte(regs->hl); rrc(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_0f() { rrc(&regs->a); }
void InstructionSet::ext_10() { rl(&regs->b); }
void InstructionSet::ext_11() { rl(&regs->c); }
void InstructionSet::ext_12() { rl(&regs->d); }
void InstructionSet::ext_13() { rl(&regs->e); }
void InstructionSet::ext_14() { rl(&regs->h); }
void InstructionSet::ext_15() { rl(&regs->l); }
void InstructionSet::ext_16() { uint8_t value = memory->read_memory_byte(regs->hl); rl(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_17() { rl(&regs->a); }
void InstructionSet::ext_18() { rr(&regs->b); }
void InstructionSet::ext_19() { rr(&regs->c); }
void InstructionSet::ext_1a() { rr(&regs->d); }
void InstructionSet::ext_1b() { rr(&regs->e); }
void InstructionSet::ext_1c() { rr(&regs->h); }
void InstructionSet::ext_1d() { rr(&regs->l); }
void InstructionSet::ext_1e() { uint8_t value = memory->read_memory_byte(regs->hl); rr(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_1f() { rr(&regs->a); }
void InstructionSet::ext_20() { sla(&regs->b); }
void InstructionSet::ext_21() { sla(&regs->c); }
void InstructionSet::ext_22() { sla(&regs->d); }
void InstructionSet::ext_23() { sla(&regs->e); }
void InstructionSet::ext_24() { sla(&regs->h); }
void InstructionSet::ext_25() { sla(&regs->l); }
void InstructionSet::ext_26() { uint8_t value = memory->read_memory_byte(regs->hl); sla(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_27() { sla(&regs->a); }
void InstructionSet::ext_28() { sra(&regs->b); }
void InstructionSet::ext_29() { sra(&regs->c); }
void InstructionSet::ext_2a() { sra(&regs->d); }
void InstructionSet::ext_2b() { sra(&regs->e); }
void InstructionSet::ext_2c() { sra(&regs->h); }
void InstructionSet::ext_2d() { sra(&regs->l); }
void InstructionSet::ext_2e() { uint8_t value = memory->read_memory_byte(regs->hl); sra(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_2f() { sra(&regs->a); }
void InstructionSet::ext_30() { swap(&regs->b); }
void InstructionSet::ext_31() { swap(&regs->c); }
void InstructionSet::ext_32() { swap(&regs->d); }
void InstructionSet::ext_33() { swap(&regs->e); }
void InstructionSet::ext_34() { swap(&regs->h); }
void InstructionSet::ext_35() { swap(&regs->l); }
void InstructionSet::ext_36() { uint8_t value = memory->read_memory_byte(regs->hl); swap(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_37() { swap(&regs->a); }
void InstructionSet::ext_38() { srl(&regs->b); }
void InstructionSet::ext_39() { srl(&regs->c); }
void InstructionSet::ext_3a() { srl(&regs->d); }
void InstructionSet::ext_3b() { srl(&regs->e); }
void InstructionSet::ext_3c() { srl(&regs->h); }
void InstructionSet::ext_3d() { srl(&regs->l); }
void InstructionSet::ext_3e() { uint8_t value = memory->read_memory_byte(regs->hl); srl(&value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_3f() { srl(&regs->a); }
void InstructionSet::ext_40() { bit(1 << 0, regs->b); }
void InstructionSet::ext_41() { bit(1 << 0, regs->c); }
void InstructionSet::ext_42() { bit(1 << 0, regs->d); }
void InstructionSet::ext_43() { bit(1 << 0, regs->e); }
void InstructionSet::ext_44() { bit(1 << 0, regs->h); }
void InstructionSet::ext_45() { bit(1 << 0, regs->l); }
void InstructionSet::ext_46() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 0, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_47() { bit(1 << 0, regs->a); }
void InstructionSet::ext_48() { bit(1 << 1, regs->b); }
void InstructionSet::ext_49() { bit(1 << 1, regs->c); }
void InstructionSet::ext_4a() { bit(1 << 1, regs->d); }
void InstructionSet::ext_4b() { bit(1 << 1, regs->e); }
void InstructionSet::ext_4c() { bit(1 << 1, regs->h); }
void InstructionSet::ext_4d() { bit(1 << 1, regs->l); }
void InstructionSet::ext_4e() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 1, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_4f() { bit(1 << 1, regs->a); }
void InstructionSet::ext_50() { bit(1 << 2, regs->b); }
void InstructionSet::ext_51() { bit(1 << 2, regs->c); }
void InstructionSet::ext_52() { bit(1 << 2, regs->d); }
void InstructionSet::ext_53() { bit(1 << 2, regs->e); }
void InstructionSet::ext_54() { bit(1 << 2, regs->h); }
void InstructionSet::ext_55() { bit(1 << 2, regs->l); }
void InstructionSet::ext_56() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 2, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_57() { bit(1 << 2, regs->a); }
void InstructionSet::ext_58() { bit(1 << 3, regs->b); }
void InstructionSet::ext_59() { bit(1 << 3, regs->c); }
void InstructionSet::ext_5a() { bit(1 << 3, regs->d); }
void InstructionSet::ext_5b() { bit(1 << 3, regs->e); }
void InstructionSet::ext_5c() { bit(1 << 3, regs->h); }
void InstructionSet::ext_5d() { bit(1 << 3, regs->l); }
void InstructionSet::ext_5e() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 3, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_5f() { bit(1 << 3, regs->a); }
void InstructionSet::ext_60() { bit(1 << 4, regs->b); }
void InstructionSet::ext_61() { bit(1 << 4, regs->c); }
void InstructionSet::ext_62() { bit(1 << 4, regs->d); }
void InstructionSet::ext_63() { bit(1 << 4, regs->e); }
void InstructionSet::ext_64() { bit(1 << 4, regs->h); }
void InstructionSet::ext_65() { bit(1 << 4, regs->l); }
void InstructionSet::ext_66() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 4, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_67() { bit(1 << 4, regs->a); }
void InstructionSet::ext_68() { bit(1 << 5, regs->b); }
void InstructionSet::ext_69() { bit(1 << 5, regs->c); }
void InstructionSet::ext_6a() { bit(1 << 5, regs->d); }
void InstructionSet::ext_6b() { bit(1 << 5, regs->e); }
void InstructionSet::ext_6c() { bit(1 << 5, regs->h); }
void InstructionSet::ext_6d() { bit(1 << 5, regs->l); }
void InstructionSet::ext_6e() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 5, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_6f() { bit(1 << 5, regs->a); }
void InstructionSet::ext_70() { bit(1 << 6, regs->b); }
void InstructionSet::ext_71() { bit(1 << 6, regs->c); }
void InstructionSet::ext_72() { bit(1 << 6, regs->d); }
void InstructionSet::ext_73() { bit(1 << 6, regs->e); }
void InstructionSet::ext_74() { bit(1 << 6, regs->h); }
void InstructionSet::ext_75() { bit(1 << 6, regs->l); }
void InstructionSet::ext_76() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 6, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_77() { bit(1 << 6, regs->a); }
void InstructionSet::ext_78() { bit(1 << 7, regs->b); }
void InstructionSet::ext_79() { bit(1 << 7, regs->c); }
void InstructionSet::ext_7a() { bit(1 << 7, regs->d); }
void InstructionSet::ext_7b() { bit(1 << 7, regs->e); }
void InstructionSet::ext_7c() { bit(1 << 7, regs->h); }
void InstructionSet::ext_7d() { bit(1 << 7, regs->l); }
void InstructionSet::ext_7e() { uint8_t value = memory->read_memory_byte(regs->hl); bit(1 << 7, value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_7f() { bit(1 << 7, regs->a); }
void InstructionSet::ext_80() { res(1 << 0, &regs->b); }
void InstructionSet::ext_81() { res(1 << 0, &regs->c); }
void InstructionSet::ext_82() { res(1 << 0, &regs->d); }
void InstructionSet::ext_83() { res(1 << 0, &regs->e); }
void InstructionSet::ext_84() { res(1 << 0, &regs->h); }
void InstructionSet::ext_85() { res(1 << 0, &regs->l); }
void InstructionSet::ext_86() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 0, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_87() { res(1 << 0, &regs->a); }
void InstructionSet::ext_88() { res(1 << 1, &regs->b); }
void InstructionSet::ext_89() { res(1 << 1, &regs->c); }
void InstructionSet::ext_8a() { res(1 << 1, &regs->d); }
void InstructionSet::ext_8b() { res(1 << 1, &regs->e); }
void InstructionSet::ext_8c() { res(1 << 1, &regs->h); }
void InstructionSet::ext_8d() { res(1 << 1, &regs->l); }
void InstructionSet::ext_8e() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 1, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_8f() { res(1 << 1, &regs->a); }
void InstructionSet::ext_90() { res(1 << 2, &regs->b); }
void InstructionSet::ext_91() { res(1 << 2, &regs->c); }
void InstructionSet::ext_92() { res(1 << 2, &regs->d); }
void InstructionSet::ext_93() { res(1 << 2, &regs->e); }
void InstructionSet::ext_94() { res(1 << 2, &regs->h); }
void InstructionSet::ext_95() { res(1 << 2, &regs->l); }
void InstructionSet::ext_96() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 2, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_97() { res(1 << 2, &regs->a); }
void InstructionSet::ext_98() { res(1 << 3, &regs->b); }
void InstructionSet::ext_99() { res(1 << 3, &regs->c); }
void InstructionSet::ext_9a() { res(1 << 3, &regs->d); }
void InstructionSet::ext_9b() { res(1 << 3, &regs->e); }
void InstructionSet::ext_9c() { res(1 << 3, &regs->h); }
void InstructionSet::ext_9d() { res(1 << 3, &regs->l); }
void InstructionSet::ext_9e() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 3, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_9f() { res(1 << 3, &regs->a); }
void InstructionSet::ext_a0() { res(1 << 4, &regs->b); }
void InstructionSet::ext_a1() { res(1 << 4, &regs->c); }
void InstructionSet::ext_a2() { res(1 << 4, &regs->d); }
void InstructionSet::ext_a3() { res(1 << 4, &regs->e); }
void InstructionSet::ext_a4() { res(1 << 4, &regs->h); }
void InstructionSet::ext_a5() { res(1 << 4, &regs->l); }
void InstructionSet::ext_a6() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 4, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_a7() { res(1 << 4, &regs->a); }
void InstructionSet::ext_a8() { res(1 << 5, &regs->b); }
void InstructionSet::ext_a9() { res(1 << 5, &regs->c); }
void InstructionSet::ext_aa() { res(1 << 5, &regs->d); }
void InstructionSet::ext_ab() { res(1 << 5, &regs->e); }
void InstructionSet::ext_ac() { res(1 << 5, &regs->h); }
void InstructionSet::ext_ad() { res(1 << 5, &regs->l); }
void InstructionSet::ext_ae() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 5, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_af() { res(1 << 5, &regs->a); }
void InstructionSet::ext_b0() { res(1 << 6, &regs->b); }
void InstructionSet::ext_b1() { res(1 << 6, &regs->c); }
void InstructionSet::ext_b2() { res(1 << 6, &regs->d); }
void InstructionSet::ext_b3() { res(1 << 6, &regs->e); }
void InstructionSet::ext_b4() { res(1 << 6, &regs->h); }
void InstructionSet::ext_b5() { res(1 << 6, &regs->l); }
void InstructionSet::ext_b6() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 6, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_b7() { res(1 << 6, &regs->a); }
void InstructionSet::ext_b8() { res(1 << 7, &regs->b); }
void InstructionSet::ext_b9() { res(1 << 7, &regs->c); }
void InstructionSet::ext_ba() { res(1 << 7, &regs->d); }
void InstructionSet::ext_bb() { res(1 << 7, &regs->e); }
void InstructionSet::ext_bc() { res(1 << 7, &regs->h); }
void InstructionSet::ext_bd() { res(1 << 7, &regs->l); }
void InstructionSet::ext_be() { uint8_t value = memory->read_memory_byte(regs->hl); res(1 << 7, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_bf() { res(1 << 7, &regs->a); }
void InstructionSet::ext_c0() { set(1 << 0, &regs->b); }
void InstructionSet::ext_c1() { set(1 << 0, &regs->c); }
void InstructionSet::ext_c2() { set(1 << 0, &regs->d); }
void InstructionSet::ext_c3() { set(1 << 0, &regs->e); }
void InstructionSet::ext_c4() { set(1 << 0, &regs->h); }
void InstructionSet::ext_c5() { set(1 << 0, &regs->l); }
void InstructionSet::ext_c6() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 0, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_c7() { set(1 << 0, &regs->a); }
void InstructionSet::ext_c8() { set(1 << 1, &regs->b); }
void InstructionSet::ext_c9() { set(1 << 1, &regs->c); }
void InstructionSet::ext_ca() { set(1 << 1, &regs->d); }
void InstructionSet::ext_cb() { set(1 << 1, &regs->e); }
void InstructionSet::ext_cc() { set(1 << 1, &regs->h); }
void InstructionSet::ext_cd() { set(1 << 1, &regs->l); }
void InstructionSet::ext_ce() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 1, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_cf() { set(1 << 1, &regs->a); }
void InstructionSet::ext_d0() { set(1 << 2, &regs->b); }
void InstructionSet::ext_d1() { set(1 << 2, &regs->c); }
void InstructionSet::ext_d2() { set(1 << 2, &regs->d); }
void InstructionSet::ext_d3() { set(1 << 2, &regs->e); }
void InstructionSet::ext_d4() { set(1 << 2, &regs->h); }
void InstructionSet::ext_d5() { set(1 << 2, &regs->l); }
void InstructionSet::ext_d6() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 2, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_d7() { set(1 << 2, &regs->a); }
void InstructionSet::ext_d8() { set(1 << 3, &regs->b); }
void InstructionSet::ext_d9() { set(1 << 3, &regs->c); }
void InstructionSet::ext_da() { set(1 << 3, &regs->d); }
void InstructionSet::ext_db() { set(1 << 3, &regs->e); }
void InstructionSet::ext_dc() { set(1 << 3, &regs->h); }
void InstructionSet::ext_dd() { set(1 << 3, &regs->l); }
void InstructionSet::ext_de() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 3, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_df() { set(1 << 3, &regs->a); }
void InstructionSet::ext_e0() { set(1 << 4, &regs->b); }
void InstructionSet::ext_e1() { set(1 << 4, &regs->c); }
void InstructionSet::ext_e2() { set(1 << 4, &regs->d); }
void InstructionSet::ext_e3() { set(1 << 4, &regs->e); }
void InstructionSet::ext_e4() { set(1 << 4, &regs->h); }
void InstructionSet::ext_e5() { set(1 << 4, &regs->l); }
void InstructionSet::ext_e6() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 4, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_e7() { set(1 << 4, &regs->a); }
void InstructionSet::ext_e8() { set(1 << 5, &regs->b); }
void InstructionSet::ext_e9() { set(1 << 5, &regs->c); }
void InstructionSet::ext_ea() { set(1 << 5, &regs->d); }
void InstructionSet::ext_eb() { set(1 << 5, &regs->e); }
void InstructionSet::ext_ec() { set(1 << 5, &regs->h); }
void InstructionSet::ext_ed() { set(1 << 5, &regs->l); }
void InstructionSet::ext_ee() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 5, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_ef() { set(1 << 5, &regs->a); }
void InstructionSet::ext_f0() { set(1 << 6, &regs->b); }
void InstructionSet::ext_f1() { set(1 << 6, &regs->c); }
void InstructionSet::ext_f2() { set(1 << 6, &regs->d); }
void InstructionSet::ext_f3() { set(1 << 6, &regs->e); }
void InstructionSet::ext_f4() { set(1 << 6, &regs->h); }
void InstructionSet::ext_f5() { set(1 << 6, &regs->l); }
void InstructionSet::ext_f6() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 6, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_f7() { set(1 << 6, &regs->a); }
void InstructionSet::ext_f8() { set(1 << 7, &regs->b); }
void InstructionSet::ext_f9() { set(1 << 7, &regs->c); }
void InstructionSet::ext_fa() { set(1 << 7, &regs->d); }
void InstructionSet::ext_fb() { set(1 << 7, &regs->e); }
void InstructionSet::ext_fc() { set(1 << 7, &regs->h); }
void InstructionSet::ext_fd() { set(1 << 7, &regs->l); }
void InstructionSet::ext_fe() { uint8_t value = memory->read_memory_byte(regs->hl); set(1 << 7, &value); memory->write_memory_byte(regs->hl, value); }
void InstructionSet::ext_ff() { set(1 << 7, &regs->a); }