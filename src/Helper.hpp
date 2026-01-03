/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_HELPER_HPP
#define LIBSMART_STM32COMMON_HELPER_HPP

#include <libsmart_config.hpp>
#include <main.h>

typedef char __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
#define BREAKPOINT __asm volatile("BKPT #0");
#define LIBSMART_BREAKPOINT __asm volatile("BKPT #0");
#define LIBSMART_UNUSED(X) (void)X
#define LIBSMART_NOP(...) __asm volatile ("nop")
#define LIBSMART_NOF(...)
#define LIBSMART_REVERSE_BYTES(pData, nDataSize) \
  {unsigned char swap, *lo = ((unsigned char *)(pData)), *hi = ((unsigned char *)(pData)) + (nDataSize) - 1; \
  while (lo < hi) { swap = *lo; *lo++ = *hi; *hi-- = swap; }}
#define LIBSMART_MS_TO_US(t) ((t) * 1000UL)
#define LIBSMART_SECONDS_TO_MS(t) ((t) * 1000UL)
#define LIBSMART_MINUTES_TO_MS(t) LIBSMART_SECONDS_TO_MS((t) * 60UL)
#define LIBSMART_HOURS_TO_MS(t) LIBSMART_MINUTES_TO_MS((t) * 60UL)
#define LIBSMART_DAYS_TO_MS(t) LIBSMART_HOURS_TO_MS((t) * 24UL)
#define LIBSMART_WEEKS_TO_MS(t) LIBSMART_DAYS_TO_MS((t) * 7UL)

#define LIBSMART_ARRAYFILL(x) {std::memset(x, 0, sizeof(x));}
#define LIBSMART_ARRAYSIZE(x) (sizeof(x) / sizeof((x)[0]))

#define LIBSMART_CEIL_DIV(x, y) (((x) + (y) - 1) / (y))

#if __EXCEPTIONS
#include <stdexcept>
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                 \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1];                \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
throw std::runtime_error(buffer);                                       \
} while (0);
#else
#define LIBSMART_HANDLE_ERROR(fmt, ...)                                 \
do {                                                                    \
char buffer[snprintf(nullptr, 0, fmt, __VA_ARGS__) + 1];                \
snprintf(buffer, sizeof(buffer), fmt, __VA_ARGS__);                     \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(buffer); \
return ret;                                                             \
} while (0);
#endif

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
