/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_HELPER_HPP
#define LIBSMART_STM32COMMON_HELPER_HPP

#include <libsmart_config.hpp>
#include <main.h>

// #define F(x) x
typedef char __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
#define BREAKPOINT __asm volatile("BKPT #0");
#define LIBSMART_BREAKPOINT __asm volatile("BKPT #0");
#define LIBSMART_UNUSED(X) (void)X
#define LIBSMART_NOP(...) __asm volatile ("nop")
#define LIBSMART_NOF(...)

#ifndef __cplusplus
#include <stdbool.h>
#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    unsigned long millis();
    unsigned long long micros();
    void delay(unsigned long ms);
    void delayMicroseconds(uint64_t us);
    long map(long value, long inMin, long inMax, long outMin, long outMax);
    bool isInIsr();

#ifdef __cplusplus
}
#endif

#endif
