/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 *
 * This file is part of libsmart/Stm32Common, which is distributed under the terms
 * of the BSD 3-Clause License. You should have received a copy of the BSD 3-Clause
 * License along with libsmart/Stm32Common. If not, see <https://spdx.org/licenses/BSD-3-Clause.html>.
 *
 * ----------------------------------------------------------------------------
 * Portions of the code are derived from David A. Mellis's work,
 * which is licensed under the GNU Lesser General Public License. You can find the original work at:
 * <https://github.com/arduino/ArduinoCore-avr/>
 * ----------------------------------------------------------------------------
 */

#include <cstdio>
#include <cstdlib>
#include "Print.hpp"

using namespace Stm32Common;

size_t Print::write(const uint8_t *inputBytes, size_t size) {
#if defined(LIBSMART_ENABLE_DIRECT_BUFFER_WRITE)
    uint8_t *txBuffer;
    auto szTxBuffer = getWriteBuffer(txBuffer);
    if (txBuffer == nullptr) {
        return BasePrint::write(inputBytes, size);
    }
    if (size > szTxBuffer) {
        size = szTxBuffer;
    }
    memcpy(txBuffer, inputBytes, size);
    return setWrittenBytes(size);
#endif
#if !defined(LIBSMART_ENABLE_DIRECT_BUFFER_WRITE)
    return BasePrint::write(inputBytes, size);
#endif
}
