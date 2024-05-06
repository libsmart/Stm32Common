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
 * Portions of the code are derived from Adrian McEwen's work,
 * which is licensed under the GNU Lesser General Public License. You can find the original work at:
 * <https://github.com/arduino/ArduinoCore-avr/>
 * ----------------------------------------------------------------------------
 */


#ifndef LIBSMART_STM32COMMON_PRINTABLE_HPP
#define LIBSMART_STM32COMMON_PRINTABLE_HPP

#include "libsmart_config.hpp"
#include <cstddef>

namespace Stm32Common {
    class Print;

    /**
     * The Printable class provides a way for new classes to allow themselves to be printed.
     * By deriving from Printable and implementing the printTo method, it will then be possible
     * for users to print out instances of this class by passing them into the usual
     * Print::print and Print::println methods.
     */
    class Printable {
    public:
        /**
         * @brief An abstract virtual function that allows classes to be printed.
         *
         * This virtual function needs to be implemented by any class that wants to be printable.
         * The derived class should override this function to define how it wants to be printed.
         *
         * @param printObject The Print object to which the class needs to be printed.
         * @return The number of characters that were printed.
         *
         * @see Print
         */
        virtual size_t printTo(Print &printObject) const = 0;
    };
}

#endif //LIBSMART_STM32COMMON_PRINTABLE_HPP
