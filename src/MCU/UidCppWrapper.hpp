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

    extern uint32_t Stm32Common_MCU_Uid_getUid32();

#ifdef __cplusplus
}
#endif

#endif
