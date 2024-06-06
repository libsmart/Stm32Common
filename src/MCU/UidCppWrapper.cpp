/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "UidCppWrapper.hpp"
#include "Uid.hpp"

uint32_t Stm32Common_MCU_Uid_getUid32() {
    return Stm32Common::MCU::Uid::getUid32();
}
