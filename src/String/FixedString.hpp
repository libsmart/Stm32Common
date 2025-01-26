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
    class FixedString : public BasePrint {
        // static constexpr auto N_CORR = N + N % 4 - 1;
        std::array<char, N + 1> _data{}; // +1 for null termination

    public:
        FixedString() = default;

        explicit FixedString(const char *str) { std::strncpy(_data.data(), str, N); }

        [[nodiscard]] const char *c_str() const { return _data.data(); }

        virtual void set(const char *str) {
            clear();
            std::strncpy(_data.data(), str, N);
            _data[N] = '\0';
        }

        virtual const char &operator[](std::size_t idx) const { return _data[idx]; }

        virtual char &operator[](std::size_t idx) { return _data[idx]; }

        virtual bool operator==(FixedString &rhs) const {
            return (size() == rhs.size()) && (strcmp(c_str(), rhs.c_str()) == 0);
        }

        virtual bool operator==(const char *rhs) const {
            return (size() == strlen(rhs)) && (strcmp(c_str(), rhs) == 0);
        }

        virtual explicit operator const char *() const { return c_str(); }

        static constexpr std::size_t capacity() { return sizeof(_data); }

        [[nodiscard]] virtual std::size_t size() const { return strlen(_data.data()); }

        virtual void fill(const char &c) { _data.fill(c); }

        virtual void clear() { fill('\0'); }

        [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }

        size_t write(uint8_t data) override {
            if (availableForWrite() == 0) return 0;
            _data[strlen(c_str())] = data;
            return 1;
        };

        int availableForWrite() override { return N - size(); }
    };
}
