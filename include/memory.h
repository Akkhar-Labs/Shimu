/* 
 * memory.h - Virtual RAM Management for Shimu Simulator
 * Architect: Rahat Hasan | Akkhar Labs
 */

#ifndef MEMORY_H
#define MEMORY_H

#include "shimu.h"
#include "arch/nirdeshika16.h"

/* --- মেমোরি ফাংশনসমূহ --- */

/**
 * ভার্চুয়াল মেমোরি পরিষ্কার (Reset) করা।
 */
void init_memory();

/**
 * মেমোরির একটি নির্দিষ্ট ঠিকানা থেকে ১ বাইট উপাত্ত পড়া।
 */
uint8_t memory_read_byte(uint16_t address);

/**
 * মেমোরির একটি নির্দিষ্ট ঠিকানায় ১ বাইট উপাত্ত সংরক্ষণ করা।
 */
void memory_write_byte(uint16_t address, uint8_t value);

/**
 * মেমোরি থেকে ১৬-বিটের একটি পূর্ণ নির্দেশ বা উপাত্ত পড়া (Little-endian)।
 */
uint16_t memory_read_word(uint16_t address);

/**
 * মেমোরিতে ১৬-বিটের একটি পূর্ণ উপাত্ত সংরক্ষণ করা।
 */
void memory_write_word(uint16_t address, uint16_t value);

/**
 * সরাসরি মেমোরি বাফার এক্সেস করার জন্য (শুধুমাত্র বাইনারি লোড করার সময়)।
 */
uint8_t* get_memory_ptr();

#endif // MEMORY_H