/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_HALSTATUS_HPP
#define LIBSMART_STM32COMMON_HALSTATUS_HPP

#include <libsmart_config.hpp>

namespace Stm32Common {
    enum class HalStatus {
        HAL_OK = 0x00U,
        HAL_ERROR = 0x01U,
        HAL_BUSY = 0x02U,
        HAL_TIMEOUT = 0x03U
    };
}
#endif
