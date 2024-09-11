/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <libsmart_config.hpp>
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
 * @brief Get the current micros value.
 *
 * This function returns the number of microseconds since the device was powered on or reset.
 *
 * @return The current micros value.
 */
uint64_t micros() {
    uint64_t micros = HAL_GetTick() * 1000; // Millisekunden in Mikrosekunden umrechnen
    micros += (SysTick->LOAD - SysTick->VAL) /
            (SystemCoreClock / 1000000); // Zeit seit dem letzten Millisekunden-Überlauf hinzufügen
    return micros;
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


/**
 * @brief Delay execution for the specified amount of microseconds.
 *
 * This function delays the execution for the given number of microseconds.
 * It starts by getting the current microsecond count using the `micros()` function.
 * Then it enters a loop, continuously checking the current microsecond count until the desired delay has passed.
 * Inside the while loop, a no-operation instruction `__NOP()` is used to prevent compiler optimization from removing the loop.
 * The delay is calculated by subtracting the start microsecond count from the current microsecond count and comparing it with the desired delay.
 *
 * @param us The desired delay time in microseconds.
 *
 * @note This function also depends on the __NOP() instruction from cmsis_gcc.h which is used to prevent compiler optimization
 * and ensure that the loop is not optimized out during compilation.
 *
 * @see micros()
 */
void delayMicroseconds(uint64_t us) {
    const auto startMicros = micros();
    while (micros() - startMicros < us) { __NOP(); };
}


/**
 * @brief Maps a value from one range to another range.
 *
 * This function takes an input value and maps it from the input range [inMin, inMax] to the output range [outMin, outMax].
 *
 * @param value The value to be mapped.
 * @param inMin The minimum value of the input range.
 * @param inMax The maximum value of the input range.
 * @param outMin The minimum value of the output range.
 * @param outMax The maximum value of the output range.
 * @return The mapped value in the output range.
 */
long map(const long value, const long inMin, const long inMax, const long outMin, const long outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}


/**
 * @brief Check if the system is currently handling an interrupt.
 *
 * This function determines if the system is in an interrupt service routine (ISR)
 * by checking the VECTACTIVE field of the Interrupt Control and State Register (ICSR).
 *
 * @return `true` if the system is within an ISR, `false` otherwise.
 */
bool isInIsr() {
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
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
