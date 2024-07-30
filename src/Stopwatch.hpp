/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STOPWATCH_HPP
#define LIBSMART_STM32COMMON_STOPWATCH_HPP

#include <libsmart_config.hpp>
#include <cstdint>
#include <functional>
#include "Helper.hpp"


namespace Stm32Common {
    class Stopwatch {
    public:
#ifdef LIBSMART_ENABLE_STD_FUNCTION
        using fn_t = std::function<void()>;

        void measure(const fn_t &measured_fn) {
            lastStartMicros = micros();

            measured_fn();

            lastDurationMicros = micros() - lastStartMicros;
            longestDurationMicros = std::max(longestDurationMicros, lastDurationMicros);
            shortestDurationMicros = std::min(shortestDurationMicros, lastDurationMicros);
            rollingAverageDurationMicros = rollingAverageDurationMicros == 0
                                               ? lastDurationMicros
                                               : (rollingAverageDurationMicros + lastDurationMicros) / 2;
        }

#endif

        void printResult(Stm32Common::Print *printable) {
            printable->print("lastDurationMicros           = ");
            printable->print((uint32_t) lastDurationMicros);
            printable->println("us");
            printable->print("longestDurationMicros        = ");
            printable->print((uint32_t) longestDurationMicros);
            printable->println("us");
            printable->print("shortestDurationMicros       = ");
            printable->print((uint32_t) shortestDurationMicros);
            printable->println("us");
            printable->print("rollingAverageDurationMicros = ");
            printable->print((uint32_t) rollingAverageDurationMicros);
            printable->println("us");
        }

    private:
        uint64_t lastStartMicros = 0;
        uint64_t lastDurationMicros = 0;
        uint64_t longestDurationMicros = 0;
        uint64_t shortestDurationMicros = UINT64_MAX;
        uint64_t rollingAverageDurationMicros = 0;
    };
}

#endif
