/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "libsmart_config.hpp"
#include "Helper.hpp"
#include "main.h"
#ifdef LIBSMART_USE_THREADX
#include "tx_api.h"
#endif

/**
 * @brief Get the current millis value.
 *
 * This function returns the number of milliseconds since the device was powered on or reset.
 *
 * @return The current millis value.
 */
unsigned long millis() {
#ifdef TICK_INT_PRIORITY
#if TICK_INT_PRIORITY > 14
#warning Please give the tick interrupt a higher priority than 15
#endif
#endif

#ifdef tx_time_get
    return tx_time_get() * (1000 / TX_TIMER_TICKS_PER_SECOND);
#else
    return HAL_GetTick() * uwTickFreq;
#endif
}

/**
 * @brief Delays the program execution for a specified number of milliseconds.
 *
 * This function provides a portable way to introduce a delay in the program execution. The actual implementation
 * depends on the underlying platform.
 *
 * @param ms The number of milliseconds to delay.
 *
 * @note This function does not guarantee precise timing due to variations in system clock frequency and other factors.
 *
 * @see HAL_Delay
 */
void delay(unsigned long ms) {
#ifdef tx_thread_sleep
    tx_thread_sleep((ms * TX_TIMER_TICKS_PER_SECOND) / 1000);
#else
    HAL_Delay(ms);
#endif

}


#ifdef LIBSMART_OVERWRITE_verbose_terminate_handler
#if __EXCEPTIONS
namespace __gnu_cxx {
    /**
     * @brief Terminate handler for verbose termination.
     *
     * This function is called when a C++ exception propagates out of a function that
     * does not have an exception specification.
     * It calls the Error_Handler() function.
     */
    void __verbose_terminate_handler() {
        Error_Handler();
    }
}
#endif
#endif
