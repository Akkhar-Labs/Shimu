/* 
 * cpu.h - CPU Core State and Instruction Cycle Prototypes
 * Updated: Standardized for modular memory and stream-based I/O.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#ifndef CPU_H
#define CPU_H

#include "shimu.h"
#include "arch/nirdeshika16.h"
#include <stdio.h>

/* --- সিপিইউ স্টেট কাঠামো (CPU State Structure) --- 
 * এটি প্রসেসরের বর্তমান অবস্থা ধারণ করে।
 */
typedef struct {
    uint16_t registers[NUM_REGISTERS]; // ১৬টি সাধারণ ও বিশেষ সাময়িক (S0-S15)
    uint16_t pc;                       // নির্দেশ-নির্দেশক (Program Counter)
    uint16_t sp;                       // শৃঙ্খল-নির্দেশক (Stack Pointer)
    uint16_t flags;                    // অবস্থা-সাময়িক (Status Flags)
    
    int is_halted;                     // প্রসেসর কি বন্ধ (Halt) অবস্থায় আছে?
} CPU;

/* --- সিপিইউ কোর ফাংশনসমূহ --- */

// সিপিইউ এবং রেজিস্টারসমূহ প্রাথমিক অবস্থায় (Reset) আনা
void init_cpu(CPU *cpu);

// একটি পূর্ণ নির্দেশ চক্র চালানো (মেমোরি এখন মডিউল থেকে রিড হবে)
void step_cpu(CPU *cpu, FILE *output_stream);

// প্রসেসরের বর্তমান অবস্থা নির্দিষ্ট স্ট্রিমে দেখানো
void dump_cpu_state(CPU *cpu, FILE *output_stream);

/* --- বিট ম্যানিপুলেশন সাহায্যকারী (Helper) --- */

// নির্দিষ্ট ফ্ল্যাগ বিট সেট করা
void set_flag(CPU *cpu, int flag_bit, int value);

// নির্দিষ্ট ফ্ল্যাগ বিটের মান দেখা
int get_flag(CPU *cpu, int flag_bit);

#endif // CPU_H