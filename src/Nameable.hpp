/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_NAMEABLE_HPP
#define LIBSMART_STM32COMMON_NAMEABLE_HPP

namespace Stm32Common {
    class Nameable {
    public:
        virtual ~Nameable() = default;

        Nameable() = default;

        explicit Nameable(const char *name)
            : name(name) {
        }

        [[nodiscard]] virtual const char *getName() const {
            return name == nullptr ? getDefaultName() : name;
        }

        virtual char *getNameNonConst() {
            return const_cast<char *>(getName());
        }

        virtual void setName(const char *name) {
            this->name = name;
        }

    protected:
        [[nodiscard]] virtual const char *getDefaultName() const {
            return "";
        }

    private:
        const char *name = {};
    };
}

#endif //LIBSMART_STM32COMMON_NAMEABLE_HPP
