/* 
 * cpu.c - Implementation of the Nirdeshika-16 Core Engine
 * Updated: Implemented Multiplication (OP_MUL) and Division (OP_DIV) with safety checks.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#include "cpu.h"
#include "memory.h"
#include "utils/formatting.h"
#include <string.h>

// সিপিইউ এবং রেজিস্টারসমূহ প্রাথমিক অবস্থায় আনা
void init_cpu(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers));
    cpu->pc = 0;
    cpu->sp = 0;
    cpu->flags = 0;
    cpu->is_halted = 0;
}

// অবস্থা-সাময়িক (Status Flags) বিট সেট করা
void set_flag(CPU *cpu, int flag_bit, int value) {
    if (value) {
        cpu->registers[FLAG_REG] |= (1 << flag_bit);
    } else {
        cpu->registers[FLAG_REG] &= ~(1 << flag_bit);
    }
}

// নির্দিষ্ট নির্দেশের সাইকেল চালানো (মেমোরি মডিউল ব্যবহার করে)
void step_cpu(CPU *cpu, FILE *output_stream) {
    if (cpu->is_halted) return;

    // ১. নির্দেশ সংগ্রহ (Fetch)
    uint16_t instr = memory_read_word(cpu->pc);
    
    // ২. নির্দেশ বিশ্লেষণ (Decode)
    uint8_t opcode = GET_OPCODE(instr);
    uint8_t reg_id = GET_REGID(instr);
    uint8_t operand = GET_VALUE(instr);

    // ৩. নির্দেশ কার্যকর (Execute)
    switch (opcode) {
        case OP_NOP: break;

        case OP_LOAD:
            cpu->registers[reg_id] = operand;
            break;

        case OP_ADD:
            cpu->registers[reg_id] += cpu->registers[operand];
            set_flag(cpu, FLAG_ZERO, (cpu->registers[reg_id] == 0));
            break;

        case OP_SUB:
            cpu->registers[reg_id] -= cpu->registers[operand];
            set_flag(cpu, FLAG_ZERO, (cpu->registers[reg_id] == 0));
            break;

        case OP_MUL: {
            // গুণফল ১৬-বিটের বেশি হতে পারে (উপচয় চেক করার জন্য ৩২-বিট ব্যবহার)
            uint32_t result = (uint32_t)cpu->registers[reg_id] * (uint32_t)cpu->registers[operand];
            cpu->registers[reg_id] = (uint16_t)(result & 0xFFFF);
            
            // ফ্ল্যাগ আপডেট
            set_flag(cpu, FLAG_ZERO, (cpu->registers[reg_id] == 0));
            set_flag(cpu, FLAG_OVERFLOW, (result > 0xFFFF));
            break;
        }

        case OP_DIV: {
            // শূন্য দিয়ে ভাগ (Division by Zero) সুরক্ষা
            if (cpu->registers[operand] == 0) {
                fprintf(output_stream, "ভুল: শূন্য দিয়ে ভাগ করার চেষ্টা (Division by Zero)!\n");
                cpu->is_halted = 1;
            } else {
                cpu->registers[reg_id] /= cpu->registers[operand];
                set_flag(cpu, FLAG_ZERO, (cpu->registers[reg_id] == 0));
            }
            break;
        }

        case OP_OUT:
            fprintf(output_stream, "[আউটপুট] সাময়িক-");
            fprint_bengali_int(output_stream, reg_id);
            fprintf(output_stream, " এর বর্তমান মান: ");
            fprint_bengali_int(output_stream, cpu->registers[reg_id]);
            fprintf(output_stream, " (ষোড়শিক: ");
            fprint_bengali_hex(output_stream, cpu->registers[reg_id]);
            fprintf(output_stream, ")\n");
            break;

        case OP_HALT:
            cpu->is_halted = 1;
            fprintf(output_stream, "\nসিস্টেম: প্রসেসর থামানো হয়েছে (থামো/HALT).\n");
            break;

        default:
            fprintf(output_stream, "ভুল: অজানা ওকোড ");
            fprint_bengali_hex(output_stream, opcode);
            fprintf(output_stream, "\n");
            cpu->is_halted = 1;
            break;
    }

    // সাময়িক-০ হার্ডওয়্যার গ্রাউন্ডিং
    cpu->registers[0] = 0;

    if (!cpu->is_halted) {
        cpu->pc += 2;
        cpu->registers[PC_REG] = cpu->pc;
    }
}

// রেজিস্টারের অবস্থা নির্দিষ্ট স্ট্রিমে দেখানো
void dump_cpu_state(CPU *cpu, FILE *output_stream) {
    fprintf(output_stream, "\n--- রেজিস্টার অবস্থা (চূড়ান্ত অবস্থা) ---\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        fprintf(output_stream, "সাময়িক-");
        fprint_bengali_int(output_stream, i);
        fprintf(output_stream, ": ");
        fprint_bengali_hex(output_stream, cpu->registers[i]);
        fprintf(output_stream, "   ");
        if ((i + 1) % 4 == 0) fprintf(output_stream, "\n");
    }
    fprintf(output_stream, "---------------------------------------\n");
}