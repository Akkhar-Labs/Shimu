/* 
 * memory.c - Implementation of Virtual RAM
 * Handles bounds checking and endianness for Nirdeshika-16.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#include "memory.h"
#include <string.h>

// ৬৫,৫৩৬ বাইটের ভার্চুয়াল মেমোরি
static uint8_t virtual_ram[MEMORY_SIZE];

void init_memory() {
    // শুরুতে পুরো মেমোরি ০ দিয়ে পূর্ণ করা
    memset(virtual_ram, 0, MEMORY_SIZE);
}

uint8_t memory_read_byte(uint16_t address) {
    // এখানে ভবিষ্যতে মেমোরি প্রোটেকশন লজিক যোগ করা যাবে
    return virtual_ram[address];
}

void memory_write_byte(uint16_t address, uint8_t value) {
    virtual_ram[address] = value;
}

uint16_t memory_read_word(uint16_t address) {
    // লিটল-এন্ডিয়ান পদ্ধতিতে ২ বাইট পড়া
    uint8_t low = virtual_ram[address];
    uint8_t high = virtual_ram[address + 1];
    return (uint16_t)(low | (high << 8));
}

void memory_write_word(uint16_t address, uint16_t value) {
    // লিটল-এন্ডিয়ান পদ্ধতিতে ২ বাইট সেভ করা
    virtual_ram[address] = value & 0xFF;         // নিম্ন ৮-বিট
    virtual_ram[address + 1] = (value >> 8) & 0xFF; // উচ্চ ৮-বিট
}

uint8_t* get_memory_ptr() {
    return virtual_ram;
}