/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMRXTXINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAMRXTXINTERFACE_HPP

#include "Stream.hpp"
#include "StringBuffer.hpp"

namespace Stm32Common {
    class StreamRxTxInterface : public Stream {
    public:

        /**
         * @brief Retrieves the receive buffer.
         *
         * This method is used to obtain a pointer to the receive buffer, which implements the StringBufferInterface.
         * Derived classes must provide the concrete implementation of this method.
         *
         * @return A pointer to the receive buffer implementing the StringBufferInterface.
         */
        virtual StringBufferInterface *getRxBuffer() = 0;

        /**
         * @brief Retrieves the transmit buffer.
         *
         * This method is used to obtain a pointer to the transmit buffer, which implements the StringBufferInterface.
         * Derived classes must provide the concrete implementation of this method.
         *
         * @return A pointer to the transmit buffer implementing the StringBufferInterface.
         */
        virtual StringBufferInterface *getTxBuffer() = 0;
    };
}
#endif
