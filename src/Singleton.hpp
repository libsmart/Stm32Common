/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_SINGLETON_HPP
#define LIBSMART_STM32COMMON_SINGLETON_HPP

namespace Stm32Common {
    template<typename T>
    class Singleton {
    public:
        static auto &getInstance() {
            static T instance;
            return instance;
        }

        explicit Singleton(const T &) = delete;

        T &operator=(const T &) = delete;

        explicit Singleton(const T &&) = delete;

        T &operator=(const T &&) = delete;

    protected:
        Singleton() {
        }
    };
}
#endif //LIBSMART_STM32COMMON_SINGLETON_HPP
