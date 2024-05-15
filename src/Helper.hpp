/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_HELPER_HPP
#define LIBSMART_STM32COMMON_HELPER_HPP

#include <libsmart_config.hpp>


// #define F(x) x
typedef char __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))


#ifndef __cplusplus
#define min(a, b) ((a)<(b)?(a):(b))
#define max(a, b) ((a)>(b)?(a):(b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

    unsigned long millis();
    void delay(unsigned long ms);

#ifdef __cplusplus
}
#endif

#endif //LIBSMART_STM32COMMON_HELPER_HPP
