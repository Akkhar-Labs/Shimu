/* 
 * main.c - The Binary Loader and Entry Point for Shimu Simulator
 * Updated: Direct logging to file with Bengali formatting.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#include "shimu.h"
#include "cpu.h"
#include "utils/formatting.h"
#include <stdio.h>

// ভার্চুয়াল মেমোরি (৬৫,৫৩৬ বাইট)
uint8_t memory[MEMORY_SIZE];

int main(int argc, char *argv[]) {
    printf("--- শিমু (Shimu) সিমুলেটর v১.০ ---\n");
    printf("স্থাপত্য: নির্দেশিকা-১৬ | আর্কিটেক্ট: রাহাত হাসান\n\n");

    // ১. কমান্ড লাইন আর্গুমেন্ট চেক করা
    if (argc < 2) {
        printf("ব্যবহার: shimu <ফাইল_পাথ.ব>\n");
        return 1;
    }

    // ২. বাইনারি (.ব) ফাইল ওপেন করা
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        printf("ভুল: ফাইলটি খোলা সম্ভব হয়নি! পাথ চেক করুন।\n");
        return 1;
    }

    // ৩. আউটপুট লগ ফাইল ওপেন করা
    FILE *log_file = fopen("সিমুলেটর_ফলাফল.txt", "w");
    if (!log_file) {
        printf("ভুল: আউটপুট লগ ফাইল তৈরি করা যায়নি।\n");
        fclose(file);
        return 1;
    }

    // ৪. ফাইল থেকে মেমোরিতে র-বাইনারি লোড করা
    size_t bytes_read = fread(memory, 1, MEMORY_SIZE, file);
    printf("সিস্টেম: %zu বাইট মেমোরিতে লোড করা হয়েছে।\n", bytes_read);
    fclose(file);

    // লগে প্রাথমিক তথ্য লেখা
    fprintf(log_file, "--- নির্দেশিকা-১৬ সিমুলেশন রিপোর্ট ---\n");
    fprintf(log_file, "আর্কিটেক্ট: রাহাত হাসান | অক্ষর ল্যাবস\n\n");

    // ৫. সিপিইউ রিসেট এবং শুরু করা
    CPU cpu;
    init_cpu(&cpu);

    printf("সিস্টেম: এক্সিকিউশন শুরু হচ্ছে... ফলাফল 'সিমুলেটর_ফলাফল.txt' ফাইলে দেখুন।\n");
    
    // ৬. সিমুলেশন লুপ (Fetch-Decode-Execute Cycle)
    while (!cpu.is_halted) {
        // সরাসরি লগে আউটপুট পাঠানো হচ্ছে
        step_cpu(&cpu, memory, log_file);
        
        if (cpu.pc >= MEMORY_SIZE) {
            fprintf(log_file, "\nভুল: মেমোরি আউট-অফ-বাউন্ড এরর!\n");
            break;
        }
    }

    // ৭. চূড়ান্ত অবস্থা লগে দেখানো
    fprintf(log_file, "\nসিস্টেম: এক্সিকিউশন সফলভাবে সম্পন্ন হয়েছে।\n");
    dump_cpu_state(&cpu, log_file);

    fclose(log_file);
    printf("---------------------------------------\n");
    printf("সফল: সিমুলেশন রিপোর্ট তৈরি হয়েছে।\n");

    return 0;
}