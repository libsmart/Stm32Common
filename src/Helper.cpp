/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Helper.hpp"
#include "main.h"

/**
 * @brief Get the current millis value.
 *
 * This function returns the number of milliseconds since the device was powered on or reset.
 *
 * @return The current millis value.
 */
unsigned long millis() {
#if TICK_INT_PRIORITY > 14
#warning Please give the tick interrupt a higher priority than 15
#endif

#ifdef tx_time_get
    return tx_time_get() * (1000 / TX_TIMER_TICK_PER_SECOND)
#else
    return HAL_GetTick() * uwTickFreq;
#endif
}
