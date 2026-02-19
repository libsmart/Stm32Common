/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 *
 * This file is part of libsmart/Stm32Common, which is distributed under the terms
 * of the BSD 3-Clause License. You should have received a copy of the BSD 3-Clause
 * License along with libsmart/Stm32Common. If not, see <https://spdx.org/licenses/BSD-3-Clause.html>.
 *
 * ----------------------------------------------------------------------------
 * Portions of the code are derived from David A. Mellis's work,
 * which is licensed under the GNU Lesser General Public License. You can find the original work at:
 * <https://github.com/arduino/ArduinoCore-avr/>
 * ----------------------------------------------------------------------------
 */

#include <cstdio>
#include <cstdlib>
#include "BasePrint.hpp"

using namespace Stm32Common;

#if defined(LIBSMART_ENABLE_PRINTF)
size_t BasePrint::printNumber(unsigned long n, uint8_t base) {
    switch (base) {
        case BIN: {
            char buf[8 * sizeof(long) + 1];
            utoa(n, buf, 2);
            write(buf);
        }
        case OCT:
            return printf("%o", n);
        case HEX:
            return printf("0x%x", n);
        default:
            return printf("%lu", n);
    }
}

size_t BasePrint::printFloat(double number, uint8_t digits) {
    return printf("%.*lf", digits, number);
}
#endif

#if !defined(LIBSMART_ENABLE_PRINTF)
#include "math.h"
size_t BasePrint::printNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if (base < 2) base = 10;

    do {
        char c = n % base;
        n /= base;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t BasePrint::printFloat(double number, uint8_t digits) {
    size_t n = 0;

    if (isnan(number)) return print("nan");
    if (isinf(number)) return print("inf");
    if (number > 4294967040.0) return print("ovf"); // constant determined empirically
    if (number < -4294967040.0) return print("ovf"); // constant determined empirically

    // Handle negative numbers
    if (number < 0.0) {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = number - (double) int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0) {
        n += print('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0) {
        remainder *= 10.0;
        unsigned int toPrint = (unsigned int) (remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}
#endif

size_t BasePrint::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while (size--) {
        if (write(*buffer++)) n++;
        else break;
    }
    return n;
}

#if defined(LIBSMART_ENABLE_STD_STRING)
size_t BasePrint::print(const std::string &prnt_string) {
    return write(prnt_string.c_str(), prnt_string.size());
}
#endif

size_t BasePrint::print(const char *prnt_cstring) {
    return write(prnt_cstring);
}

size_t BasePrint::print(char prnt_char) {
    return write(prnt_char);
}

size_t BasePrint::print(unsigned char prnt_unsigned_char, int base) {
    return print((unsigned long) prnt_unsigned_char, base);
}

size_t BasePrint::print(int prnt_int, int base) {
    return print((long) prnt_int, base);
}

size_t BasePrint::print(unsigned int prnt_unsigned_int, int base) {
    return print((unsigned long) prnt_unsigned_int, base);
}

size_t BasePrint::print(long prnt_long, int base) {
    if (base == DEC) {
        if (prnt_long < 0) {
            int t = print('-');
            prnt_long = -prnt_long;
            return printNumber(prnt_long, DEC) + t;
        }
        return printNumber(prnt_long, DEC);
    }
    return printNumber(prnt_long, (base < 2) ? 10 : base);
}

size_t BasePrint::print(unsigned long prnt_unsigned_long, int base) {
    return printNumber(prnt_unsigned_long, (base < 2) ? 10 : base);
}

size_t BasePrint::print(double prnt_double, int digits) {
    return printFloat(prnt_double, digits);
}

size_t BasePrint::print(const Printable &prnt_object) {
    return prnt_object.printTo(*this);
}


#ifdef LIBSMART_ENABLE_PRINTF

size_t BasePrint::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    auto ret = vprintf(format, args);
    va_end(args);
    return ret;
}


size_t BasePrint::vprintf(const char *format, va_list args) {
    uint8_t buffer[LIBSMART_STM32COMMON_VPRINTF_BUFFER_SIZE]{};
    constexpr auto szBuffer = sizeof(buffer);
    constexpr auto lenBuffer = szBuffer - 1;
    int len = ::vsnprintf(reinterpret_cast<char *>(buffer), szBuffer, format, args);
    if (len < 0) return 0;
    const auto bytesToWrite = len < lenBuffer ? len : lenBuffer;
    return write(buffer, bytesToWrite);
}

#endif

#if defined(LIBSMART_ENABLE_STD_STRING)
size_t BasePrint::println(const std::string &prnt_string) {
    size_t n = print(prnt_string);
    n += println();
    return n;
}
#endif

size_t BasePrint::println(const char *prnt_cstring) {
    size_t n = print(prnt_cstring);
    n += println();
    return n;
}

size_t BasePrint::println(char prnt_char) {
    size_t n = print(prnt_char);
    n += println();
    return n;
}

size_t BasePrint::println(unsigned char prnt_unsigned_char, int base) {
    size_t n = print(prnt_unsigned_char, base);
    n += println();
    return n;
}

size_t BasePrint::println(int prnt_int, int base) {
    size_t n = print(prnt_int, base);
    n += println();
    return n;
}

size_t BasePrint::println(unsigned int prnt_unsigned_int, int base) {
    size_t n = print(prnt_unsigned_int, base);
    n += println();
    return n;
}

size_t BasePrint::println(long prnt_long, int base) {
    size_t n = print(prnt_long, base);
    n += println();
    return n;
}

size_t BasePrint::println(unsigned long prnt_unsigned_long, int base) {
    size_t n = print(prnt_unsigned_long, base);
    n += println();
    return n;
}

size_t BasePrint::println(double prnt_double, int digits) {
    size_t n = print(prnt_double, digits);
    n += println();
    return n;
}

size_t BasePrint::println(const Printable &prnt_object) {
    size_t n = print(prnt_object);
    n += println();
    return n;
}

size_t BasePrint::println() { return write("\r\n"); }

size_t BasePrint::printBinary(const uint64_t value, const uint8_t bits, const uint8_t groupsize) {
    size_t printed{};

    for (int8_t currentBit = bits - 1; currentBit >= 0; currentBit--) {
        printed += print(((value >> currentBit) & 1) ? '1' : '0');
        if ((currentBit % groupsize) == 0) printed += print(' ');
    }

    return printed;
}

