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

#ifndef LIBSMART_STM32COMMON_STREAM_HPP
#define LIBSMART_STM32COMMON_STREAM_HPP

#include "libsmart_config.hpp"
#include "Print.hpp"

#define NO_IGNORE_CHAR  '\x01' // a char not found in a valid ASCII numeric field

namespace Stm32Common {
    // This enumeration provides the lookahead options for parseInt(), parseFloat()
    // The rules set out here are used until either the first valid character is found
    // or a time out occurs due to lack of input.
    enum LookaheadMode {
        SKIP_ALL, // All invalid characters are ignored.
        SKIP_NONE, // Nothing is skipped, and the stream is not touched unless the first waiting character is valid.
        SKIP_WHITESPACE // Only tabs, spaces, line feeds & carriage returns are skipped.
    };


    /**
     * @brief The Stream class is an abstract base class that provides a common interface for derived classes that represent input streams of data.
     *
     * The Stream class defines several pure virtual functions that must be implemented by derived classes. These functions include reading a byte
     * from the input stream, getting the next character from the input stream without removing it, and checking how many bytes are available to read
     * from the input stream.
     *
     * The Stream class also provides functions for setting and getting the timeout for stream operations, searching for a target string in the stream,
     * parsing integers and floats from the stream, and reading bytes from the stream into a buffer.
     */
    class Stream : public Print {
    protected:
        /** number of milliseconds to wait for the next char before aborting timed read */
        unsigned long _timeout;
        /** used for timeout measurement */
        unsigned long _startMillis{};

        /**
         * @brief Reads a byte from the input stream within a specified timeout period.
         *
         * The timedRead() function reads a byte from the input stream with a specified timeout period. It waits for the next character to become available
         * until either a byte is read or the timeout period is exceeded. If a byte is read, it is returned. If the timeout period is exceeded, -1 is
         * returned to indicate a timeout.
         *
         * @return int The byte read from the input stream, or -1 if a timeout occurs.
         */
        int timedRead();

        /**
         * @brief Retrieves the next character from the input stream without removing it.
         *
         * The timedPeek() function retrieves the next character from the input stream without removing it. It sets a timeout period, and if no character is received within the timeout, it returns -1 to indicate a timeout. If a character is received within the timeout, it is returned.
         *
         * @return int The next character from the input stream, or -1 if a timeout occurs.
         */
        int timedPeek();

        /**
         * @brief Retrieves the next digit from the input stream.
         *
         * The peekNextDigit() function retrieves the next digit character from the input stream. It takes two parameters: the lookahead mode and the detectDecimal flag.
         * The lookahead mode determines how invalid characters are handled, and the detectDecimal flag determines if the function should consider a period ('.') as a valid digit.
         *
         * If a valid digit is found, it is returned as an integer. If no valid digit is found and SKIP_NONE lookahead mode is selected, -1 is returned to indicate failure.
         * If no valid digit is found and SKIP_WHITESPACE lookahead mode is selected, whitespace characters are skipped until a non-whitespace character is encountered.
         * If no valid digit is found and SKIP_ALL lookahead mode is selected, all characters are skipped until a valid digit or the end of the stream is reached.
         *
         * @param lookahead The lookahead mode to determine how invalid characters are handled.
         * @param detectDecimal Specifies if the function should consider a period ('.') as a valid digit.
         * @return int The next digit character from the input stream, or -1 if no valid digit is found and SKIP_NONE lookahead mode is selected.
         */
        int peekNextDigit(LookaheadMode lookahead,
                          bool detectDecimal);

    public:
        Stream() { _timeout = 1000; }

        /**
         * @brief Returns the number of bytes available to read from the input stream.
         *
         * The available() function is a pure virtual function defined in the Stream class that should be implemented
         * by all derived classes. It returns the number of bytes available to read from the input stream.
         *
         * @return The number of bytes available to read from the input stream.
         */

        virtual int available() = 0;

        /**
         * @brief This is a pure virtual function reads a byte from the input stream.
         *
         * The derived classes of the Stream class should override this function to provide their own implementation of reading a byte from the input stream.
         *
         * @return int The byte read from the input stream.
         */
        virtual int read() = 0;

        /**
         * @brief Gets the next character from the input stream without removing it.
         *
         * This function is a pure virtual function defined in the Stream class that should be implemented
         * by all derived classes. It returns the next character from the input stream without removing it.
         *
         * @return int The next character from the input stream.
         */
        virtual int peek() = 0;

        /**
         * @brief Sets the timeout period for stream operations.
         *
         * The setTimeout function sets the timeout period for stream operations. This determines the maximum amount of time to wait for the next character to become available during a timed read.
         *
         * @param timeout The timeout period in milliseconds.
         */
        void setTimeout(unsigned long timeout);

        /**
         * @brief Returns the timeout period for stream operations.
         *
         * The getTimeout() function returns the timeout period for stream operations. This determines the maximum amount of time to wait for the next character to become available during a timed read.
         *
         * @return The timeout period in milliseconds.
         */
        unsigned long getTimeout();

        /**
         * @brief Searches for a target string in the input stream.
         *
         * The find() function searches for the specified target string in the input stream. It returns true if the target string is found.
         * Otherwise, it returns false.
         *
         * @param target A null-terminated string to be searched in the input stream.
         * @return True if the target string is found, false otherwise.
         */
        bool find(char *target);

        /**
         * @brief Searches for a target string in the input stream.
         *
         * The find() function searches for the specified target string in the input stream. It returns true if the target string is found.
         * Otherwise, it returns false.
         *
         * @param target A null-terminated string to be searched in the input stream.
         * @return True if the target string is found, false otherwise.
         */
        bool find(uint8_t *target);

        /**
         * @brief Searches for a target string in the input stream.
         *
         * The find() function searches for the specified target string in the input stream.
         * It returns true if the target string is found, otherwise it returns false.
         *
         * @param target A null-terminated string to be searched in the input stream.
         * @param length The length of the target string.
         * @return True if the target string is found, false otherwise.
         */
        bool find(char *target, size_t length);

        /**
         * @brief Searches for a target string in the input stream.
         *
         * The find() function searches for the specified target string in the input stream. It reads data from the stream until the target string of given length is found. It returns true if the target string is found, otherwise it returns false.
         *
         * @param target A null-terminated string of characters to be searched in the input stream.
         * @param length The length of the target string.
         * @return True if the target string is found in the input stream, false otherwise.
         */
        bool find(uint8_t *target, size_t length);

        /**
         * @brief Searches for a target character in the input stream.
         *
         * The find() function searches for the specified target string in the input stream.
         * It reads data from the stream until the target string of given length is found.
         * It returns true if the target string is found, otherwise it returns false.
         *
         * @param target A character to be searched in the input stream.
         * @return True if the target character is found in the input stream, false otherwise.
         */
        bool find(char target);

        /**
         * @brief Searches for a target string in the input stream until a terminator string is found.
         *
         * The findUntil() function searches for the specified target string in the input stream. It reads data from the stream
         * until the terminator string is found. It returns true if the terminator string is found before the end of the stream,
         * otherwise it returns false.
         *
         * @param target A null-terminated string of characters to be searched in the input stream.
         * @param terminator A null-terminated string of characters that marks the end of the search.
         * @return True if the terminator string is found before the end of the stream, false otherwise.
         */
        bool findUntil(char *target, char *terminator);

        /**
         * @brief Searches for a sequence of bytes until a specified character is found.
         *
         * This function searches for a sequence of bytes in the stream until a specified
         * character is found or the end of the stream is reached.
         *
         * @param target The sequence of bytes to search for in the stream.
         * @param terminator The character that marks the end of the search.
         * @return true if the sequence of bytes is found before the terminator, false otherwise.
         */
        bool findUntil(uint8_t *target, char *terminator);

        /**
         * @brief Finds the target string within the given stream until the terminator string is found.
         *
         * This function searches for the target string within the stream until the terminator string is found.
         * It returns true if the target string is found before the terminator string is found or if the search times out,
         * otherwise it returns false.
         *
         * @param target A pointer to the target string.
         * @param targetLen The length of the target string.
         * @param terminator A pointer to the terminator string.
         * @param termLen The length of the terminator string.
         *
         * @return true if the target string is found before the terminator string or if the search times out,
         *         false otherwise.
         */
        bool findUntil(char *target, size_t targetLen, char *terminator, size_t termLen);

        /**
          * @brief Find a sequence of bytes in the stream until a specified terminator is encountered.
          *
          * This function searches for a sequence of bytes in the stream until a specified terminator is encountered.
          * The terminator can be a character or a sequence of characters.
          *
          * @param[in] target A pointer to an array of bytes representing the sequence to search for.
          * @param[in] targetLen The length of the sequence to search for.
          * @param[in] terminator A pointer to a character array representing the terminator to stop searching for.
          * @param[in] termLen The length of the terminator sequence.
          *
          * @return True if the sequence is found before the terminator or false if the terminator is encountered first.
          */
        bool findUntil(uint8_t *target, size_t targetLen, char *terminator, size_t termLen);


        /**
         * @brief Parses a long integer value from the stream.
         *
         * This function reads characters from the stream and converts them into a long integer value.
         * The function stops parsing when it encounters a character that is not a valid part of an integer.
         *
         * @param lookahead The lookahead mode used to determine how the function looks ahead in the stream.
         *                  Default value is SKIP_ALL.
         * @param ignore The character to be ignored while parsing. Default value is NO_IGNORE_CHAR.
         *
         * @return The first valid (long) integer value from the current position in the stream.
         *         Returns 0 if timeout occurs before parsing starts.
         */
        long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

        /**
         * @brief Parses a floating point number from the input stream.
         *
         * This function reads characters from the input stream until a valid floating
         * point number is parsed or the end of the stream is reached. The function
         * skips leading non-numeric characters and supports negative numbers and
         * fractions.
         *
         * @param lookahead The lookahead mode for the input stream. Defaults to
         *                  SKIP_ALL, which skips all non-numeric characters.
         * @param ignore    The character to ignore when encountered. Defaults to
         *                  NO_IGNORE_CHAR, which means no characters are ignored.
         * @return          The parsed floating point number.
         *
         * @note            If no valid floating point number is found, the function
         *                  returns 0. If a numeric character is not followed by a
         *                  valid floating point representation, the function stops
         *                  parsing and returns the parsed number up to that point.
         */
        float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

        /**
         * @brief Reads a specified number of bytes from a stream and stores them in a buffer.
         *
         * This function reads characters from a stream and stores them in a buffer. It terminates
         * if the specified number of characters have been read or if a timeout occurs. The timeout
         * is determined by the `setTimeout()` function of the Stream class.
         *
         * @param buffer Pointer to a char array where the read characters will be stored.
         * @param length The number of characters to read from the stream.
         * @return The number of characters successfully read and placed in the buffer.
         *
         * @see Stream::setTimeout()
         *
         * @note The buffer is not null terminated. It is the responsibility of the caller to ensure
         *       that the buffer is large enough to hold the specified number of characters.
         */
        size_t readBytes(char *buffer, size_t length);

        /**
         * @brief Reads a specified number of bytes from the input stream and stores them in a buffer.
         *
         * This function reads up to the specified number of bytes from the input stream and stores them in the
         * provided buffer. The function returns the number of bytes actually read.
         *
         * @param buffer A pointer to the buffer where the read bytes will be stored.
         * @param length The maximum number of bytes to read.
         * @return The number of bytes actually read from the input stream.
         */
        size_t readBytes(uint8_t *buffer, size_t length);

        /**
         * @brief Read characters from a stream until a specified terminator character is found or the maximum length is reached.
         *
         * This function reads characters from the stream until either the specified terminator character is found,
         * the maximum length is reached, or a timeout occurs. The characters are stored in the provided buffer.
         *
         * @param terminator The terminator character to search for.
         * @param buffer A pointer to the buffer where the read characters will be stored.
         * @param length The maximum number of characters to read.
         * @return The number of characters placed in the buffer. If no valid data is found, 0 is returned.
         */
        size_t readBytesUntil(char terminator, char *buffer, size_t length);

        /**
         * @brief Reads bytes from the stream until a specified terminator character is encountered.
         *
         * This function reads characters from the stream into a buffer until either the specified terminator character is encountered or the specified length is reached. The characters are stored as uint8_t values in the buffer. The function returns the number of bytes read.
         *
         * @param terminator The terminator character that specifies the end of the bytes to be read.
         * @param buffer     A pointer to the buffer where the bytes will be stored.
         * @param length     The maximum number of bytes to read.
         *
         * @return The number of bytes read from the stream.
         */
        size_t readBytesUntil(char terminator, uint8_t *buffer, size_t length);

        //TODO: Arduino String functions to be added here
        //        String readString();
        //        String readStringUntil(char terminator);

    protected:
        /**
         * @brief Parses a long integer from the input stream, ignoring any leading characters specified.
         *
         * This function reads characters from the input stream until it encounters a non-numeric character or the end of the stream,
         * and then converts the read characters into a long integer. Leading characters specified in the 'ignore' parameter are skipped.
         *
         * @param ignore The character to be ignored. Leading characters matching this value will be skipped during parsing.
         * @return long The parsed long integer value.
         */
        long parseInt(char ignore);

        /**
         * @brief Parses a float value from the input stream.
         *
         * This function is used to extract a float value from the input stream.
         *
         * @param ignore The character to be ignored.
         *
         * @return The parsed float value.
         *
         * @see Stream::parseFloat(char)
         */
        float parseFloat(char ignore);

        struct MultiTarget {
            const char *str; // string you're searching for
            size_t len; // length of string you're searching for
            size_t index; // index used by the search routine.
        };

        /**
         * @brief Search for an arbitrary number of strings.
         *
         * This function searches for a target string among a list of target strings.
         * It returns the index of the first target that is found or -1 if a timeout occurs.
         *
         * @param targets A pointer to the array of target strings.
         * @param tCount The number of target strings in the array.
         * @return The index of the first target string that is found, or -1 if a timeout occurs.
         */
        int findMulti(struct MultiTarget *targets, int tCount);
    };
}
#undef NO_IGNORE_CHAR
#endif //LIBSMART_STM32COMMON_STREAM_HPP
