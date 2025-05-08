/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "PrintInterface.hpp"

namespace Stm32Common {
    class ResultBase : public Printable {
    public:
        const char *COMPONENT_NAME{};
        const char *CLASS_NAME{};
        const char *INSTANCE_NAME{};
        const char *FUNCTION_NAME{};
    };


    // Result<T, E> für reguläre Typen
    template<typename T, typename E>
    class Result : public ResultBase {
    public:
        constexpr static Result ok(const T &value) noexcept {
            return Result(value);
        }

        constexpr static Result err(const E &error) noexcept {
            return Result(error);
        }

        constexpr bool is_ok() const noexcept { return m_is_ok; }
        constexpr bool is_err() const noexcept { return !m_is_ok; }

        constexpr const T &value() const noexcept { return m_value; }
        constexpr const E &error() const noexcept { return m_error; }

        size_t printTo(PrintInterface &printObject) const override {
            return printObject.printf("%s::%s[%s]::%s = 0x%02x %s\r\n",
                                      COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME,
                                      is_err() ? error().apiReturnValue() : 0,
                                      is_err() ? error().message() : ""
            );
        }

    private:
        explicit Result(const T &value) noexcept : m_value(value), m_error(), m_is_ok(true) { ; }

        constexpr Result(const E &error) noexcept : m_value(), m_error(error), m_is_ok(false) { ; }

        T m_value{};
        E m_error{};
        bool m_is_ok = false;
    };


    // Spezialisierung für Result<void, E>
    template<typename E>
    class Result<void, E> : public ResultBase {
    public:
        constexpr static Result ok() noexcept {
            return Result(true);
        }

        constexpr static Result err(const E &error) noexcept {
            return Result(error);
        }

        constexpr bool is_ok() const noexcept { return m_is_ok; }
        constexpr bool is_err() const noexcept { return !m_is_ok; }

        constexpr const E &error() const noexcept { return m_error; }

        size_t printTo(PrintInterface &printObject) const override {
            return printObject.printf("%s::%s[%s]::%s = 0x%02x %s\r\n",
                                      COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME,
                                      is_err() ? error().apiReturnValue() : 0,
                                      is_err() ? error().message() : ""
            );
        }

    private:
        constexpr Result(bool ok) noexcept : m_error(), m_is_ok(ok) { ; }

        constexpr Result(const E &error) noexcept : m_error(error), m_is_ok(false) { ; }

        E m_error{};
        bool m_is_ok = false;
    };
}
