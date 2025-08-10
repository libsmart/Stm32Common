/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <array>
#include <cstring>
#include "BasePrint.hpp"

namespace Stm32Common::String {
    /**
     * @class FixedString
     *
     * @brief A class to represent a fixed-size string.
     *
     * This class provides a string implementation where the size is fixed
     * at compile-time. It is useful for cases where memory usage needs to
     * be predictable or constrained.
     *
     * The FixedString class provides basic functionalities to manipulate
     * fixed-size strings, such as assignment, comparison, and modifications
     * within the predetermined size limit.
     */
    template<std::size_t N>
    class FixedString : public BasePrint, public Printable {
        // static constexpr auto N_CORR = N + N % 4 - 1;
        std::array<char, N + 1> _data{}; // +1 for null termination
        static constexpr std::size_t ZERO_SIZE = 0;
        inline static char zeroChar = '\0';

    public:
        FixedString() = default;

        explicit FixedString(const char *str) { FixedString::set(str); }

        explicit FixedString(const std::string_view str) { FixedString::set(str); }

        FixedString(const char *str, const size_t sz) { FixedString::set(std::string_view(str, sz)); }

        /**
         * @brief Retrieves a `std::string_view` representation of the fixed string.
         *
         * This method provides a lightweight, non-owning view of the internal string data.
         *
         * @return A `std::string_view` encapsulating the contents of the fixed string.
         */
        constexpr std::string_view string_view() const { return std::string_view(_data.data(), size()); }
        /**
         * @brief Implicitly converts the object to a `std::string_view`.
         *
         * This operator allows the object to be used or interpreted directly as a `std::string_view`,
         * representing the internal string content without copying or modifying the data.
         *
         * @return A `std::string_view` representing the internal string content.
         */
        constexpr operator std::string_view() const noexcept { return string_view(); }

        /**
         * @brief Provides a null-terminated C-style string representation.
         *
         * Returns a pointer to the internal buffer containing the current content of the object
         * in a null-terminated C-style format, suitable for use in APIs or functions that require
         * a `const char*`.
         *
         * @return A pointer to the internal null-terminated C-style string.
         */
        [[nodiscard]] virtual const char *c_str() const { return _data.data(); }
        /**
         * @brief Converts the FixedString to a C-style string.
         *
         * Provides a C-style null-terminated string representation of the
         * FixedString's current content. This conversion allows the FixedString
         * to be used in contexts where a `const char*` is required.
         *
         * @return A pointer to the internal null-terminated C-style string.
         */
        virtual explicit operator const char *() const { return c_str(); }

        /**
         * @brief Sets the content of the fixed string using a given `std::string_view`.
         *
         * This method clears the existing content of the fixed string and replaces it
         * with the content from the provided `std::string_view`, up to the capacity of
         * the fixed string.
         *
         * @param str The `std::string_view` containing the new content to be set.
         * @return A reference to the updated `FixedString` instance.
         */
        virtual FixedString &set(const std::string_view str) {
            clear();
            str.copy(_data.data(), std::min(str.size(), capacity()), 0);
            return *this;
        }

        /**
         * @brief Sets the value of the fixed string using a C-style null-terminated string.
         *
         * This method updates the contents of the fixed string to match the provided input.
         *
         * @param str A pointer to a null-terminated string. If the pointer is null, no change is made.
         * @return A reference to the updated FixedString instance.
         */
        virtual FixedString &set(const char *str) {
            if (str == nullptr) return *this;
            return set(std::string_view{str});
        }

        /**
         * @brief Assigns the content of the FixedString to the given string.
         *
         * Replaces the current content of the FixedString with the content provided
         * by the given `std::string_view`. The assignment operation internally uses
         * the `set` method to update the content.
         *
         * @param rhs The string to assign as the new content of the FixedString,
         *            provided as a `std::string_view`.
         *
         * @return A reference to the current FixedString.
         */
        FixedString &operator=(const std::string_view rhs) {
            return set(rhs);
        }

        /**
         * @brief Provides access to the character at the specified index.
         *
         * This operator returns a reference to the character at the given index within the string.
         * If the index is out of bounds, it returns a reference to a null character.
         *
         * @param idx The zero-based index of the character to be accessed.
         * @return A reference to the character at the specified index, or a reference to a null character if the index is out of bounds.
         */
        virtual const char &operator[](std::size_t idx) const { return (idx < N) ? _data[idx] : (zeroChar = '\0'); }

        /**
         * @brief Accesses the character at the specified index in the string.
         *
         * Provides direct access to the character at the given index. If the index is within bounds,
         * it returns a reference to the character at that position. If the index is out of bounds,
         * a null character (`'\0'`) is assigned to an internal variable and a reference to it is returned.
         *
         * @param idx The zero-based index of the character to access.
         * @return A reference to the character at the specified index if within bounds,
         *         otherwise a reference to a null character (`'\0`).
         */
        virtual char &operator[](std::size_t idx) { return (idx < N) ? _data[idx] : (zeroChar = '\0'); }

        /**
         * @brief Retrieves the maximum capacity of the FixedString.
         *
         * Calculates and returns the maximum number of characters
         * that the FixedString can hold, excluding the null terminator.
         *
         * @return The capacity of the FixedString as a `std::size_t`.
         */
        static constexpr std::size_t capacity() { return sizeof(_data) - 1; }

        /**
         * @brief Returns the current size of the FixedString.
         *
         * Computes and provides the number of characters currently stored
         * in the FixedString, excluding the null terminator.
         *
         * @return The size of the FixedString as a `std::size_t`.
         */
        constexpr std::size_t size() const { return strlen(_data.data()); }

        /**
         * @brief Fills the FixedString with the specified character.
         *
         * Replaces all characters in the FixedString with the given character
         * and null-terminates the string.
         *
         * @param c The character to fill the FixedString with.
         * @return A reference to the updated FixedString.
         */
        virtual FixedString &fill(const char &c) {
            _data.fill(c);
            _data[N] = '\0';
            return *this;
        }

        /**
         * @brief Clears the contents of the FixedString.
         *
         * Resets all characters in the FixedString to a null character ('\0').
         *
         * @return A reference to the cleared FixedString.
         */
        virtual FixedString &clear() { return fill('\0'); }

        /**
         * @brief Checks if the object is empty.
         *
         * Determines whether the object contains no elements.
         *
         * @return True if the object is empty, otherwise false.
         */
        [[nodiscard]] constexpr bool empty() const noexcept { return size() == ZERO_SIZE; }

        /**
         * @brief Writes a single byte of data to the fixed-size buffer.
         *
         * This method adds the specified byte to the buffer, provided there is
         * available space. If the buffer is full, the write operation will not take place.
         *
         * @param data The byte of data to be written to the buffer.
         * @return The number of bytes written. Returns 1 if the byte is successfully written,
         * or 0 if the buffer is full.
         */
        size_t write(uint8_t data) override {
            if (availableForWrite() == 0) return ZERO_SIZE;
            _data[size()] = data;
            return 1;
        };

        /**
         * @brief Determines the amount of space available for writing.
         *
         * This method calculates the remaining capacity in the fixed-size buffer
         * that can be written to, ensuring that the value does not drop below zero.
         *
         * @return The number of characters that can be written to the buffer without
         * exceeding its fixed capacity.
         */
        int availableForWrite() override { return std::max(N - size(), ZERO_SIZE); }

        /**
         * @brief Copies the contents of the string to the provided buffer.
         *
         * This method attempts to copy the string content to the specified buffer, ensuring
         * that the string is null-terminated. It will copy at most `size - 1` characters
         * to allow room for the null-terminator.
         *
         * @param buffer A pointer to the destination character buffer where the string will
         * be copied. The buffer must be large enough to store the copied characters and
         * the null-terminator.
         * @param size The size of the destination buffer, including space for the null-terminator.
         *             A size of zero will result in no operation.
         * @return The number of characters successfully copied to the buffer, excluding
         * the null-terminator.
         */
        virtual size_t copyTo(char *buffer, const size_t size) const {
            if (size == 0) return ZERO_SIZE;
            if (buffer == nullptr) return ZERO_SIZE;
            const std::string_view sv = string_view();
            const size_t toCopy = std::min(sv.size(), size - 1);
            if (toCopy > 0) {
                std::memmove(buffer, sv.data(), toCopy);
            }
            buffer[toCopy] = '\0';
            return toCopy;
        }

        /**
         * @brief Writes the contents of the string to the specified PrintInterface object.
         *
         * This method outputs the contents of the FixedString instance to a provided
         * PrintInterface object by utilizing its print method.
         *
         * @param printObject A reference to a PrintInterface object where the string
         * content will be printed.
         * @return The number of characters written to the PrintInterface object.
         */
        size_t printTo(PrintInterface &printObject) const override {
            return printObject.print(c_str());
        }

        /**
         * @brief Retrieves the first character of the string.
         *
         * This method returns the first character of the FixedString instance. If the string
         * is empty, it returns a static character `zeroChar` as a default value.
         *
         * @return The first character of the string, or `zeroChar` if the string is empty.
         */
        char first() const { return _data[0]; }

        /**
         * @brief Retrieves the last character of the string.
         *
         * This method returns the last character of the FixedString instance. It assumes
         * that the string is not empty and accesses the last character based on its size.
         *
         * @return The last character of the string.
         */
        char last() const { return _data[size() - 1]; }

        /**
         * @brief Removes a character at the specified index and shifts the remaining characters leftward.
         *
         * This method removes the character at the given index `idx` from the FixedString instance,
         * shifts all subsequent characters leftward to fill the gap, and maintains null termination
         * of the string. If the index is out of bounds (greater than or equal to the string's size),
         * the method returns a static character `zeroChar` as a default value without modifying
         * the string.
         *
         * @param idx The index of the character to remove.
         * @return The character that was removed, or `zeroChar` if the index is out of bounds.
         */
        char remove(const size_t idx) {
            if (idx >= size()) return zeroChar;
            const char c = _data[idx];
            std::memmove(&_data[idx], &_data[idx + 1], (size() - idx) + 1);
            return c;
        }

        /**
         * @brief Removes a specified number of characters starting from a given index.
         *
         * This method removes `n` characters starting from the index `idx` in the FixedString instance.
         * It shifts the subsequent characters to the left to fill the gap and ensures null termination.
         * If `n` is zero or if the starting index is out of bounds, the method performs no action and
         * returns `ZERO_SIZE`. If `n` exceeds the range of removable characters from the index, only
         * the maximum possible characters are removed.
         *
         * @param idx The starting index from which characters should be removed.
         * @param n The number of characters to remove.
         * @return The actual number of characters removed, or `ZERO_SIZE` if no characters were removed.
         */
        size_t remove(const size_t idx, const size_t n) {
            if (n == 0) return ZERO_SIZE;
            const size_t len = size();
            if (idx >= len) return ZERO_SIZE;
            const size_t max_rem = len - idx;
            const size_t rem = (n < max_rem) ? n : max_rem;
            const size_t tail = (len - (idx + rem)) + 1;
            std::memmove(&_data[idx], &_data[idx + rem], tail);
            std::fill(_data.begin() + (len - rem) + 1, _data.end(), '\0');
            return rem;
        }

        /**
         * @brief Compares the object with a given `std::string_view` for equality.
         *
         * Determines if the content of the object matches the content of the provided `std::string_view`.
         * The comparison returns true if both have the same size and their content is identical.
         *
         * @param rhs The `std::string_view` to compare against.
         * @return `true` if the contents are equal, `false` otherwise.
         */
        virtual bool operator==(const std::string_view rhs) const {
            return (size() == rhs.size()) && std::char_traits<char>::compare(_data.data(), rhs.data(), rhs.size()) == 0;
        }

        /**
         * @brief Checks if the FixedString instance starts with the specified string view.
         *
         * This method determines whether the FixedString contains the provided string view
         * as a prefix. The comparison is case-sensitive. If the size of the string view
         * is greater than the size of the FixedString, the method will return false.
         *
         * @param sv A string view representing the prefix to be checked for.
         * @return True if the FixedString starts with the specified prefix, otherwise false.
         */
        bool startsWith(const std::string_view sv) const {
            if (sv.size() > size()) return false;
            return std::char_traits<char>::compare(_data.data(), sv.data(), sv.size()) == 0;
        }

        /**
         * @brief Checks if the FixedString instance ends with the specified string view.
         *
         * This method determines whether the FixedString contains the provided string view
         * as a suffix. The comparison is case-sensitive. If the size of the string view
         * is greater than the size of the FixedString, the method will return false.
         *
         * @param sv A string view representing the suffix to be checked for.
         * @return True if the FixedString ends with the specified suffix, otherwise false.
         */
        bool endsWith(const std::string_view sv) const {
            if (sv.size() > size()) return false;
            return std::char_traits<char>::compare(&_data[size() - sv.size()], sv.data(), sv.size()) == 0;
        }

        /**
         * @brief Finds the first occurrence of a substring within the FixedString instance.
         *
         * Searches for the first occurrence of the given substring `needle` in the FixedString.
         * If the `needle` is an empty string, the method returns 0. If the size of the `needle`
         * exceeds the size of the FixedString, it returns `std::string_view::npos`. Otherwise,
         * it attempts to locate the `needle` within the FixedString.
         *
         * @param needle The substring to search for as a `std::string_view`.
         * @return The starting index of the first occurrence of `needle`, or `std::string_view::npos`
         *         if the substring is not found.
         */
        std::size_t find(const std::string_view needle) const {
            if (needle.empty()) return 0;
            if (needle.size() > size()) return std::string_view::npos;
            return string_view().find(needle);
        }

        /**
         * @brief Checks whether the FixedString contains the specified substring.
         *
         * Determines if the given substring `needle` occurs within this FixedString.
         * The search is case-sensitive and follows the semantics of std::string_view::find.
         *
         * Note: An empty `needle` is considered found (returns true).
         *
         * @param needle The substring to search for.
         * @return true if `needle` is found (or empty), otherwise false.
         */
        bool contains(const std::string_view needle) const {
            return find(needle) != std::string_view::npos;
        }

        /**
         * @brief Removes leading characters from the string that match any character in the specified string view.
         *
         * This function trims leading characters from the FixedString instance, where the character matches
         * any character found in the provided string view. If the string view is empty, no trimming occurs.
         *
         * @param sv A string view containing the characters to be trimmed from the beginning of the string.
         */
        virtual FixedString &ltrim(const std::string_view sv) {
            if (sv.empty()) return *this;

            const std::size_t len = size();
            std::size_t first_keep = 0;

            while (first_keep < len && sv.find(_data[first_keep]) != std::string_view::npos) {
                ++first_keep;
            }

            if (first_keep > 0) {
                std::memmove(&_data[0], &_data[first_keep], (len - first_keep) + 1);
            }
            return *this;
        }

        /**
         * @brief Trims specified characters from the end of the string.
         *
         * This method removes any trailing characters from the FixedString instance
         * that are present in the given string view `sv`. If the string view is empty,
         * the method does nothing. The operation preserves null termination of the string
         * and ensures that the trailing characters are replaced with null characters.
         *
         * @param sv A string view containing the set of characters to be trimmed
         *           from the end of the string.
         * @return A reference to the modified FixedString instance.
         */
        virtual FixedString &rtrim(const std::string_view sv) {
            if (sv.empty()) return *this;

            const std::size_t sz = size();
            std::size_t len = sz;
            while (len > 0 && sv.find(_data[len - 1]) != std::string_view::npos) {
                --len;
            }

            if (len < sz) {
                _data[len] = '\0';
                std::fill(_data.begin() + len + 1, _data.end(), '\0');
            }
            return *this;
        }

        /**
         * @brief Trims the specified characters from both ends of the FixedString.
         *
         * This method removes all leading and trailing characters from the FixedString
         * that are present in the provided character set `sv`. It first performs a left trim
         * using `ltrim` and subsequently a right trim using `rtrim`, returning the updated
         * FixedString instance.
         *
         * @param sv The string view containing the set of characters to be trimmed.
         * @return A reference to the updated FixedString instance.
         */
        virtual FixedString &trim(const std::string_view sv) {
            ltrim(sv);
            return rtrim(sv);
        }

        /**
         * @brief Trims leading whitespace characters from the FixedString instance.
         *
         * This method removes all leading whitespace characters using a predefined
         * set of whitespace characters, leaving the remaining part of the string intact.
         *
         * @return A reference to the modified FixedString instance.
         */
        virtual FixedString &ltrim() { return ltrim(trim_default); }

        /**
         * @brief Removes trailing whitespace or specified characters from the FixedString instance.
         *
         * This method trims trailing spaces or characters defined by a default trimming set
         * from the FixedString instance. It modifies the string in place and returns a reference
         * to the updated FixedString object.
         *
         * @return A reference to the FixedString instance after trimming.
         */
        virtual FixedString &rtrim() { return rtrim(trim_default); }

        /**
         * @brief Removes leading and trailing characters from the string that match the default set.
         *
         * This method trims both leading and trailing characters from the FixedString instance
         * using the default set of characters defined by `trim_default`. The default set typically
         * includes whitespace characters such as spaces, tabs, carriage returns, and newlines.
         *
         * @return A reference to the current FixedString instance after trimming.
         */
        virtual FixedString &trim() { return trim(trim_default); }

    private:
        static constexpr std::string_view trim_default = " \t\r\n";
    };
}
