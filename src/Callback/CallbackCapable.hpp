/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "GenericBounce.hpp"

namespace Stm32Common {
    /**
     * This class allows the user to bind a callback function that can be overridden
     * by derived classes, offering flexibility in implementing specific callback logic.
     *
     * @tparam Ret The return type of the callback function.
     * @tparam Args The parameter pack representing argument types for the callback function.
     */
    template<typename Ret, typename... Args>
    class CallbackCapable : public GenericBounce<Ret, Args...> {
    public:
        CallbackCapable() {
            this->template bind<CallbackCapable>(this, &CallbackCapable::callback);
        }

        /**
         * A pure virtual method representing a callback function. This method must
         * be implemented by any derived class to define specific callback behavior.
         *
         * @param args Variadic arguments of type Args... to be passed to the callback function.
         * @return An object of type Ret, representing the result of the callback execution.
         */
        virtual Ret callback(Args... args) = 0;
    };
}
