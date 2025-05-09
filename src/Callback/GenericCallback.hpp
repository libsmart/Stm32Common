/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <type_traits>
#include <functional>

namespace Stm32Common {
    /**
     * @brief A generic callback mechanism supporting member, const-member, and free functions.
     *
     * The GenericCallback class allows the binding and invocation of different types
     * of callable entities, including member functions, const-member functions, and free functions.
     * It provides a flexible utility for creating callbacks with user-defined arguments and return types.
     *
     * @tparam Ret The return type of the callback functions.
     * @tparam Args The parameter types of the callback functions.
     */
    template<typename Ret, typename... Args>
    class GenericCallback {
    public:
        using CallbackFn = std::function<Ret(void *, Args...)>;

        virtual ~GenericCallback() = default;

        GenericCallback() = default;

        /**
         * @brief Binds a member function of a specific object to the callback.
         *
         * This method enables the callback to invoke a member function of an object,
         * with the context being the object instance provided.
         *
         * @tparam T The type of the object whose member function is being bound.
         * @param obj Pointer to the object instance whose member function is to be invoked.
         * @param method Pointer to the member function of the object to be bound.
         *               The function must match the signature specified by the callback.
         */
        template<typename T>
        void bind(T *obj, Ret (T::*method)(Args...)) {
            context_ = obj;
            function_ = [method](void *ctx, Args... args) -> Ret {
                return (static_cast<T *>(ctx)->*method)(args...);
            };
        }

        /**
         * @brief Binds a const member function of a specific object to the callback.
         *
         * This method enables the callback to invoke a const member function of an object,
         * with the context being the object instance provided.
         *
         * @tparam T The type of the object whose const member function is being bound.
         * @param obj Pointer to the object instance whose const member function is to be invoked.
         * @param method Pointer to the const member function of the object to be bound.
         *               The function must match the signature specified by the callback.
         */
        template<typename T>
        void bind(T *obj, Ret (T::*method)(Args...) const) {
            context_ = obj;
            function_ = [method](void *ctx, Args... args) -> Ret {
                return (static_cast<const T *>(ctx)->*method)(args...);
            };
        }

        /**
         * @brief Binds a free function to the callback.
         *
         * This method allows the callback to invoke a static or free function
         * that matches the specified signature.
         *
         * @param func Pointer to the free function to be bound. The function must
         *             match the signature defined by the callback.
         */
        void bind(Ret (*func)(Args...)) {
            context_ = nullptr;
            function_ = [func](void *, Args... args) -> Ret {
                return func(args...);
            };
        }

        /**
         * @brief Invokes the bound callback function with the provided arguments.
         *
         * Executes the previously bound callback function using the supplied arguments.
         * If no function is bound, this operator either returns a default-constructed
         * value of the return type or, if the return type is void, exits gracefully.
         *
         * @param args The arguments to pass to the callback function. These must match
         *             the parameters expected by the function's signature.
         * @return The result of the callback function execution. For a void return type,
         *         nothing is returned. If no function is bound and the return type is
         *         non-void, a default-constructed value is returned.
         */
        Ret operator()(Args... args) const {
            if (function_) {
                return function_(context_, args...);
            }
            if constexpr (std::is_void_v<Ret>) {
                return;
            } else {
                return Ret{};
            }
        }

    protected:
        /**
         * @brief Stores the object instance for member function callbacks
         *
         * This pointer holds the object instance when a member function is bound.
         * It provides the proper 'this' context when the callback is executed.
         * For free functions, this pointer is set to nullptr.
         */
        void *context_ = nullptr;

        /** Stores a callback function */
        CallbackFn function_ = nullptr;
    };
}
