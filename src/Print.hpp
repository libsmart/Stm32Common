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


#ifndef LIBSMART_STM32COMMON_PRINT_HPP
#define LIBSMART_STM32COMMON_PRINT_HPP

#include <libsmart_config.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include "BasePrint.hpp"
#include "Printable.hpp"

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

#ifdef LIBSMART_ENABLE_PRINTF
#define PRINTF_OVERRIDE override
#else
#define PRINTF_OVERRIDE
#endif

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
#define DIRECT_BUFFER_WRITE_OVERRIDE override
#else
#define DIRECT_BUFFER_WRITE_OVERRIDE
#endif

#ifdef LIBSMART_ENABLE_STD_STRING

#include <string>

#define OVERRIDE_STD_STRING override
#else
#define OVERRIDE_STD_STRING
#endif

namespace Stm32Common {
    /**
     * @brief The Print class provides a set of functions for printing data to an underlying device.
     *
     * This class defines a set of functions that can be used to print data to the underlying
     * device. It provides different overloaded versions of the print() function to handle
     * different types of data. These functions convert the input data into a character string
     * and write it to the underlying device. The size of the data printed is returned as the
     * output.
     *
     * Additionally, it allows formatted output using the printf() and vprintf() functions.
     *
     * The Print class is an abstract class and must be subclassed to implement the write() and
     * availableForWrite() functions, which are used to write data to the underlying device.
     */
    class Print : public BasePrint {
        int write_error = 0;

    protected:
        void setWriteError(int err = 1) { write_error = err; }

    public:
        [[nodiscard]] int getWriteError() const { return write_error; }

        void clearWriteError() { setWriteError(0); }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE

        /**
         * @brief Retrieves the write buffer.
         *
         * This is a pure virtual function that must be implemented by the derived class.
         * It returns the current buffer to be written to the underlying device.
         * Return buffer=nullptr to indicate that the underlying transport does not support direct buffer write.
         *
         * @param[out] buffer A reference to a pointer that will store the write buffer.
         * @return The size of the write buffer.
         * @note This function is an extension to the class Print in arduino. It allows direct write to the buffer.
         */
        virtual size_t getWriteBuffer(uint8_t *&buffer) = 0;

        /**
         * @brief Sets the number of bytes written to the underlying device.
         *
         * This is a pure virtual function that must be implemented by the derived class.
         * It sets the number of bytes written to the underlying device and returns the new value.
         *
         * @param size The number of bytes written to the underlying device.
         * @return The new value of the number of bytes written.
         * @note This function is an extension to the class Print in arduino. It allows direct write to the buffer.
         */
        virtual size_t setWrittenBytes(size_t size) = 0;

#endif

        /**
         * @brief Writes a single byte to the underlying device.
         *
         * This is a pure virtual function that must be implemented by the derived class.
         *
         * @param data The byte to be written.
         * @return The number of bytes written. In most cases, this will be 1, unless there was an error during writing.
         */
        size_t write(uint8_t data) override = 0;

        /**
         * @brief Writes the specified number of bytes to the underlying device.
         *
         * This function writes the specified number of bytes from the inputBytes
         * array to the underlying device and returns the actual number of bytes written.
         *
         * @param inputBytes A pointer to an array of bytes.
         * @param size The number of bytes to write.
         * @return The actual number of bytes written to the underlying device.
         */
        size_t write(const uint8_t *inputBytes, size_t size) override;

        /**
         * @brief Retrieves the number of bytes available for writing to the underlying device, before the device starts
         * blocking.
         *
         * This pure virtual function must be implemented by the derived class.
         * It returns the number of bytes available for writing to the underlying device.
         *
         * @return The number of bytes available for writing.
         */
        int availableForWrite() override = 0;

        /**
         * @brief Flushes the output of the function and waits for completion.
         *
         * This function is a pure virtual function meaning that it needs to be implemented by
         * the derived classes. It is used to flush any buffered output to the output channel.
         *
         * @note This function does not have a return value.
         */
        virtual void flush() = 0;

        using BasePrint::print;
        using BasePrint::println;
        using BasePrint::write;
    };
}

#endif //LIBSMART_STM32COMMON_PRINT_HPP
