/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Helper.hpp"
#include "main.h"

unsigned long millis() {
    return HAL_GetTick();
}
