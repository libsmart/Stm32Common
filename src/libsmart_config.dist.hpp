/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_LIBSMART_CONFIG_DIST_HPP
#define LIBSMART_STM32COMMON_LIBSMART_CONFIG_DIST_HPP

#define LIBSMART_STM32COMMON


#define CCMRAM __attribute__((section(".ccmram")))


/**
 * Enable or disable the use of std::function.
 */
#undef LIBSMART_ENABLE_STD_FUNCTION
#define LIBSMART_ENABLE_STD_FUNCTION



/**
 * Enable or disable the use of std::string.
 */
#undef LIBSMART_ENABLE_STD_STRING
#define LIBSMART_ENABLE_STD_STRING



/**
 * Enable or disable the use of printf.
 */
#undef LIBSMART_ENABLE_PRINTF
#define LIBSMART_ENABLE_PRINTF



/**
 * Enable or disable direct buffer read.
 * Enables functions that allow direct buffer reads to external
 * classes.
 */
#undef LIBSMART_ENABLE_DIRECT_BUFFER_READ
#define LIBSMART_ENABLE_DIRECT_BUFFER_READ



/**
 * Enable or disable direct buffer write.
 * Enables functions that allow direct buffer writes to external
 * classes.
 */
#undef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
#define LIBSMART_ENABLE_DIRECT_BUFFER_WRITE



/**
 * Enable or disable overwriting of the verbose_terminate_handler.
 * Overwriting reduces the binary size by several 10kB.
 * @see __gnu_cxx::__verbose_terminate_handler()
 */
#undef LIBSMART_OVERWRITE_verbose_terminate_handler
#define LIBSMART_OVERWRITE_verbose_terminate_handler



/**
 * Enable or disable the use of ThreadX.
 */
#undef LIBSMART_USE_THREADX
// #define LIBSMART_USE_THREADX

#endif
