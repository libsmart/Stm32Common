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

        explicit FixedString(const char *str) { std::strncpy(_data.data(), str, N); }

        explicit FixedString(const char *str, const size_t sz) { std::strncpy(_data.data(), str, std::min(sz, N)); }

        explicit FixedString(const std::string &str) { std::strncpy(_data.data(), str.c_str(), N); }

        [[nodiscard]] const char *c_str() const { return _data.data(); }

        virtual void set(const char *str) {
            if (str == nullptr) return;
            clear();
            std::strncpy(_data.data(), str, N);
            _data[N] = '\0';
        }

        template<std::size_t Nrhs>
        void set(const FixedString<Nrhs> &str) {
            clear();
            std::strncpy(_data.data(), str.c_str(), Nrhs);
            _data[N] = '\0';
        }

        FixedString &operator=(const char *rhs) {
            set(rhs);
            return *this;
        }

        template<std::size_t Nrhs>
        FixedString &operator=(const FixedString<Nrhs> &rhs) {
            set(rhs);
            return *this;
        }

        virtual const char &operator[](std::size_t idx) const { return (idx < N) ? _data[idx] : (zeroChar='\0'); }

        virtual char &operator[](std::size_t idx) { return (idx < N) ? _data[idx] : (zeroChar='\0'); }

        template<std::size_t Nrhs>
        bool operator==(const FixedString<Nrhs> &rhs) const {
            return (size() == rhs.size()) && (strcmp(c_str(), rhs.c_str()) == 0);
        }

        virtual bool operator==(const char *rhs) const {
            if (rhs == nullptr) return false;
            return (size() == strlen(rhs)) && (strcmp(c_str(), rhs) == 0);
        }

        template<std::size_t Nrhs>
        bool operator==(const char(&rhs)[Nrhs]) const {
            return (size() == strlen(rhs)) && (strcmp(c_str(), rhs) == 0);
        }

        virtual explicit operator const char *() const { return c_str(); }

        static constexpr std::size_t capacity() { return sizeof(_data) - 1; }

        [[nodiscard]] virtual std::size_t size() const { return strlen(_data.data()); }

        virtual void fill(const char &c) { _data.fill(c); _data[N] = '\0'; }

        virtual void clear() { fill('\0'); }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == ZERO_SIZE; }

        size_t write(uint8_t data) override {
            if (availableForWrite() == 0) return ZERO_SIZE;
            _data[strlen(c_str())] = data;
            return 1;
        };

        int availableForWrite() override { return std::max(N - size(), ZERO_SIZE); }


        void copyTo(char *buffer, const size_t size) const {
            std::strncpy(buffer, c_str(), std::min(size, N));
        }

        size_t printTo(PrintInterface &printObject) const override {
            return printObject.print(c_str());
        }

        char first() const { return _data[0]; }
        char last() const { return _data[size() - 1]; }

        char remove(const size_t idx) {
            if (idx >= size()) return zeroChar;
            const char c = _data[idx];
            std::memmove(&_data[idx], &_data[idx + 1], (size() - idx) + 1);
            return c;
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

        std::size_t find(const std::string_view sv) const {
            if (sv.empty()) return 0;
            const std::size_t selfSize = size();
            if (sv.size() > selfSize) return std::string_view::npos;
            const std::string_view haystack{_data.data(), selfSize};
            return haystack.find(sv);
        }

        bool contains(const std::string_view sv) const {
            return find(sv) != std::string_view::npos;
        }

        /**
         * @brief Removes leading characters from the string that match any character in the specified string view.
         *
         * This function trims leading characters from the FixedString instance, where the character matches
         * any character found in the provided string view. If the string view is empty, no trimming occurs.
         *
         * @param sv A string view containing the characters to be trimmed from the beginning of the string.
         */
        void ltrim(const std::string_view sv) {
            if (sv.empty()) return;

            const std::size_t len = size();
            std::size_t first_keep = 0;

            while (first_keep < len && sv.find(_data[first_keep]) != std::string_view::npos) {
                ++first_keep;
            }

            if (first_keep > 0) {
                std::memmove(&_data[0], &_data[first_keep], (len - first_keep) + 1);
            }
        }

        /**
         * @brief Removes trailing characters from the string that match any character in the specified string view.
         *
         * This function trims trailing characters from the FixedString instance, where the character matches
         * any character found in the provided string view. If the string view is empty, no trimming occurs.
         *
         * @param sv A string view containing the characters to be trimmed from the end of the string.
         */
        void rtrim(const std::string_view sv) {
            if (sv.empty()) return;

            const std::size_t sz = size();
            std::size_t len = sz;
            while (len > 0 && sv.find(_data[len - 1]) != std::string_view::npos) {
                --len;
            }

            if (len < sz) {
                _data[len] = '\0';
                std::fill(_data.begin() + len + 1, _data.end(), '\0');
            }
        }

        void trim(const std::string_view sv) { ltrim(sv); rtrim(sv); }

        void ltrim() { ltrim(trim_default); }

        void rtrim() { rtrim(trim_default); }

        void trim() { trim(trim_default); }

    private:
        static constexpr std::string_view trim_default = " \t\r\n";
    };
}
