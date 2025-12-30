/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "PrintInterface.hpp"

namespace Stm32Common {
    class PrintableResultBase : public Printable {
    public:
        const char *COMPONENT_NAME{};
        const char *CLASS_NAME{};
        const char *INSTANCE_NAME{};
        const char *FUNCTION_NAME{};
    };

    /**
     * @brief Represents the result of an operation, which can either be a success or an error.
     *
     * @tparam T The type of the value that represents a successful outcome.
     * @tparam E The type of the error that represents a failure.
     *
     * Inherits from ResultBase. The Result<T, E> class is designed to encapsulate the concept
     * of success and error as first-class constructs. It provides methods to easily distinguish
     * between success and error states and retrieve the associated data.
     */
    template<typename T, typename E>
    class PrintableResult final : public PrintableResultBase {
    public:
        constexpr static PrintableResult ok(const T &value) noexcept { return PrintableResult(value); }
        constexpr static PrintableResult err(const E &error) noexcept { return PrintableResult(error); }

        constexpr bool isOk() const noexcept { return m_is_ok; }
        constexpr bool isError() const noexcept { return !m_is_ok; }

        constexpr const T &value() const noexcept { return m_value; }
        constexpr const E &error() const noexcept { return m_error; }

        size_t printTo(PrintInterface &printObject) const override {
            const auto sz = printObject.printf("%s::%s[%s]::%s", COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME);
            if (isOk()) {
                return sz + printObject.print(value());
            }
            return sz + printObject.print(error());
            // return printObject.printf("%s::%s[%s]::%s = 0x%02x %s %s\r\n",
            //                           COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME,
            //                           isError() ? error().apiReturnValue() : 0,
            //                           isError() ? error().apiReturnString() : "",
            //                           isError() ? error().message() : ""
            // );
        }

    private:
        explicit PrintableResult(const T &value) noexcept : m_value(value), m_error(), m_is_ok(true) { ; }
        constexpr PrintableResult(const E &error) noexcept : m_value(), m_error(error), m_is_ok(false) { ; }

        T m_value{};
        E m_error{};
        bool m_is_ok = false;
    };


    /**
     * @brief Specialization of the Result class for cases where T is `void`.
     *
     * This class represents the result of an operation that does not produce a value
     * on success but instead encapsulates either a success or an error.
     * It provides utility functions to check the state of the result and
     * access the associated error.
     *
     * @tparam E The type of the error that represents a failure.
     */
    template<typename E>
    class PrintableResult<void, E>final : public PrintableResultBase {
    public:
        constexpr static PrintableResult ok() noexcept { return PrintableResult(true); }
        constexpr static PrintableResult err(const E &error) noexcept { return PrintableResult(error); }

        constexpr bool isOk() const noexcept { return m_is_ok; }
        constexpr bool isError() const noexcept { return !m_is_ok; }

        constexpr const E &error() const noexcept { return m_error; }

        size_t printTo(PrintInterface &printObject) const override {
            auto sz = printObject.printf("%s::%s[%s]::%s", COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME);
            return sz + printObject.print(error());
            // return printObject.printf("%s::%s[%s]::%s = 0x%02x %s %s\r\n",
            //                           COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, FUNCTION_NAME,
            //                           isError() ? error().apiReturnValue() : 0,
            //                           isError() ? error().apiReturnString() : "",
            //                           isError() ? error().message() : ""
            // );
        }

    private:
        constexpr PrintableResult(bool ok) noexcept : m_error(), m_is_ok(ok) { ; }
        constexpr PrintableResult(const E &error) noexcept : m_error(error), m_is_ok(false) { ; }

        E m_error{};
        bool m_is_ok = false;
    };
}
