/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_MCU_UIDCPPWRAPPER_HPP
#define LIBSMART_STM32COMMON_MCU_UIDCPPWRAPPER_HPP

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieves the 32-bit unique identifier (UID) of the device.
 *
 * This function retrieves the 32-bit UID from hardware registers and returns it.
 * The UID is calculated using the 96-bit UID value retrieved from the hardware registers
 * and a hash value calculated using the MurmurHash3 algorithm.
 * The hash value is stored in a static variable and returned subsequently calls without recalculation.
 *
 * @return The 32-bit UID of the device.
 */
extern uint32_t Stm32Common_MCU_Uid_getUid32();

#ifdef __cplusplus
}
#endif

#endif
