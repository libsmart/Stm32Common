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

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include "Printable.hpp"

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

#define ENABLE_PRINTF
#define ENABLE_DIRECT_BUFFER_WRITE
#define ENABLE_STD_STRING

#ifdef ENABLE_PRINTF
#define PRINTF_OVERRIDE override
#else
#define PRINTF_OVERRIDE
#endif

#ifdef ENABLE_DIRECT_BUFFER_WRITE
#define DIRECT_BUFFER_WRITE_OVERRIDE override
#else
#define DIRECT_BUFFER_WRITE_OVERRIDE
#endif

#ifdef ENABLE_STD_STRING

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
    class Print {
    public:
        virtual ~Print() = default;

    private:
        int write_error = 0;

        /**
         * @brief Prints a number to the underlying device.
         *
         * This function converts an unsigned long number into a character string and writes it to the underlying device.
         * The number is converted to the specified base and the resulting character string is left aligned.
         * The function returns the number of bytes written to the underlying device.
         *
         * @param n The number to be printed.
         * @param base The base to use for conversion (default is 10).
         * @return The number of bytes written to the underlying device.
         */
        size_t printNumber(unsigned long n, uint8_t base);

        /**
         * @brief Prints a floating-point number to the underlying device.
         *
         * This function converts a double precision floating-point number into a character string and writes it to
         * the underlying device. The number is rounded to the specified number of digits after the decimal point.
         *
         * @param number The number to be printed.
         * @param digits The number of digits after the decimal point (default is 2).
         * @return The number of bytes written to the underlying device.
         */
        size_t printFloat(double number, uint8_t digits);

    protected:
        void setWriteError(int err = 1) { write_error = err; }

    public:
        Print() : write_error(0) {}

        [[nodiscard]] int getWriteError() const { return write_error; }

        void clearWriteError() { setWriteError(0); }

#ifdef ENABLE_DIRECT_BUFFER_WRITE

        /**
         * @brief Retrieves the write buffer.
         *
         * This is a pure virtual function that must be implemented by the derived class.
         * It returns the current buffer to be written to the underlying device.
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
        virtual size_t write(uint8_t data) = 0;

        /**
         * @brief Writes a null-terminated string to the underlying device.
         *
         * This function writes a null-terminated string to the underlying device.
         * The string is represented by the inputString parameter, which is a pointer to a const char array.
         * The function returns the number of bytes written to the underlying device.
         *
         * @param inputString A pointer to a null-terminated string to write.
         * @return The number of bytes written to the underlying device.
         */
        virtual size_t write(const char *inputString) {
            if (inputString == nullptr) return 0;
            return write((const uint8_t *) inputString, strlen(inputString));
        }

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
        virtual size_t write(const uint8_t *inputBytes, size_t size);

        /**
         * @brief Writes the specified number of bytes to the underlying device.
         *
         * This function writes the specified number of bytes from the inputBytes
         * array to the underlying device and returns the actual number of bytes written.
         *
         * @param inputBytes A pointer to an array of characters.
         * @param size The number of bytes to write.
         * @return The actual number of bytes written to the underlying device.
         */
        size_t write(const char *inputBytes, size_t size) {
            return write((const uint8_t *) inputBytes, size);
        }

        /**
         * @brief Retrieves the number of bytes available for writing to the underlying device, before the device starts
         * blocking.
         *
         * This pure virtual function must be implemented by the derived class.
         * It returns the number of bytes available for writing to the underlying device.
         *
         * @return The number of bytes available for writing.
         */
        virtual int availableForWrite() = 0;


        //        size_t print(const __FlashStringHelper *);
        //        size_t print(const String &);
#ifdef ENABLE_STD_STRING

        /**
         * @brief Prints a string to the underlying device.
         *
         * @param printString The string to be printed.
         * @return The number of bytes written to the underlying device.
         */
        size_t print(const std::string &printString);

#endif

        /**
         * @brief Writes a null-terminated string to the underlying device.
         *
         * @param inputString A pointer to a null-terminated string to write.
         * @return The number of bytes written to the underlying device.
         */
        size_t print(const char printString[]);

        /**
         * @brief Writes a single character to the underlying device.
         *
         * @param printChar The character to be written.
         * @return The number of bytes written to the underlying device.
         */
        size_t print(char printChar);

        /**
         * @brief Writes an unsigned character value to the underlying device.
         *
         * @param printUChar The unsigned character value to be printed.
         * @param base The base to use for conversion (default is DEC).
         * @return The number of bytes written to the underlying device.
         */
        size_t print(unsigned char printUChar, int base = DEC);

        /**
         * @brief Prints an integer to the underlying device.
         *
         * @param printInt The integer to be printed.
         * @param base The base to use for conversion (default is DEC).
         * @return The number of bytes written to the underlying device.
         */
        size_t print(int printInt, int base = DEC);

        /**
         * @brief Prints an unsigned integer to the underlying device.
         *
         * @param printUInt The unsigned integer to be printed.
         * @param base The base to use for conversion (default is DEC).
         * @return The number of bytes written to the underlying device.
         */
        size_t print(unsigned int printUInt, int base = DEC);

        /**
         * @brief Prints a long integer to the underlying device.
         *
         * @param printLong The long integer to be printed.
         * @param base The base to use for conversion (default is 10).
         * @return The number of bytes written to the underlying device.
         */
        size_t print(long printLong, int base = DEC);

        /**
         * @brief Prints an unsigned long integer to the underlying device.
         *
         * @param printULong The unsigned long integer to be printed.
         * @param base The base to use for conversion (default is 10).
         * @return The number of characters printed.
         */
        size_t print(unsigned long printULong, int base = DEC);

        /**
         * @brief Prints a floating-point number to the underlying device.
         *
         * @param printDouble The floating-point number to be printed.
         * @param digits The number of decimal places to round the number to (default is 2).
         * @return The number of characters printed to the standard output.
         */
        size_t print(double printDouble, int digits = 2);

        /**
         * @brief Prints the given object using its printTo() function.
         *
         * @param printObject The object to be printed.
         * @return The number of characters printed.
         *
         * @see Printable::printTo()
         */
        size_t print(const Printable &printObject);

#ifdef ENABLE_PRINTF

        /**
         * @brief Writes formatted output to the underlying device using a variable argument list.
         *
         * This function is similar to the standard C library function vprintf().
         * It takes a format string and a variable argument list to generate formatted output.
         * The formatted output is written to the underlying device.
         *
         * @param format The format string.
         * @param args The variable argument list.
         * @return The number of bytes written to the underlying device.
         * @note This function is an extension to the class Print in arduino.
         */
        virtual size_t printf(const char *format, ...);

        /**
         * @brief Writes formatted output to the underlying device using a variable argument list.
         *
         * This function is similar to the standard C library function vprintf().
         * It takes a format string and a variable argument list to generate formatted output.
         * The formatted output is written to the underlying device.
         *
         * @param format The format string.
         * @param args The variable argument list.
         * @return The number of bytes written to the underlying device.
         * @note This function is an extension to the class Print in arduino.
         */
        virtual size_t vprintf(const char *format, va_list args);

#endif

//        size_t println(const __FlashStringHelper *);
//        size_t println(const String &s);
#ifdef ENABLE_STD_STRING

        /**
         * @brief Prints a string followed by a new line.
         *
         * @param printString The string to be printed.
         * @return The number of characters printed, including the new line character.
         * @note This function is an extension to the class Print in arduino.
         */
        size_t println(const std::string &printString);

#endif

        /**
         * @brief Prints the specified string followed by a newline character.
         *
         * @param printString The string to be printed.
         * @return The number of characters printed.
         */
        size_t println(const char printString[]);

        /**
         * @brief Prints a single character followed by a newline character.
         *
         * @param printChar The character to be printed.
         * @return The total number of characters printed, including the newline character.
         */
        size_t println(char printChar);

        /**
         * @brief Prints an unsigned char value followed by a newline character.
         *
         * @param printUChar The unsigned char value to print.
         * @param base The base to use for printing the value (default is DEC).
         * @return The total number of characters printed (including the newline character).
         */
        size_t println(unsigned char printUChar, int base = DEC);

        /**
         * @brief Prints an integer followed by a newline character.
         *
         * @param printInt The integer to be printed.
         * @param base The base of the number system used to format the integer (default: DEC).
         * @return The number of characters printed.
         */
        size_t println(int printInt, int base = DEC);

        /**
         * @brief Prints an unsigned integer followed by a newline character.
         *
         * @param printUInt The unsigned integer to be printed.
         * @param base The base in which the value should be printed. It defaults to DEC (decimal).
         * @return The number of characters printed, including the newline character.
         */
        size_t println(unsigned int printUInt, int base = DEC);

        /**
         * @brief Prints a long value followed by a line break.
         *
         * @param printLong The long value to be printed.
         * @param base (optional) The base in which the value should be printed. Defaults to DEC (decimal).
         * @return The number of characters printed.
         */
        size_t println(long printLong, int base = DEC);

        /**
         * @brief Prints an unsigned long value followed by a newline character.
         *
         * @param printULong The unsigned long value to be printed.
         * @param base The base in which the value should be printed (default is decimal).
         * @return The number of characters printed, including the newline character.
         */
        size_t println(unsigned long printULong, int base = DEC);

        /**
         * @brief Prints a double value followed by a newline character.
         *
         * @param printDouble The double value to be printed.
         * @param digits The number of decimal places to display. By default, it is set to 2.
         * @return The number of characters that were printed.
         */
        size_t println(double printDouble, int digits = 2);

        /**
         * @brief Prints the given printable object followed by a newline character.
         *
         * @param x The printable object to print.
         * @return The number of characters printed.
         */
        size_t println(const Printable &x);

        /**
         * @brief Prints a new line character followed by a carriage return.
         *
         * @return size_t The number of characters printed (always 2).
         *
         * @details This function prints a new line character ('\\n') followed by a carriage return character ('\\r').
         * The newline character creates a new line in the output, and the carriage return character moves the cursor
         * to the beginning of the current line.
         */
        virtual size_t println();

        /**
         * @brief Flushes the output of the function and waits for completion.
         *
         * This function is a pure virtual function meaning that it needs to be implemented by
         * the derived classes. It is used to flush any buffered output to the output channel.
         *
         * @note This function does not have a return value.
         */
        virtual void flush() = 0;
    };

}

#endif //LIBSMART_STM32COMMON_PRINT_HPP
