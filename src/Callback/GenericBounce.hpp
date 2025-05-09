/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <libsmart_config.hpp>
#include "GenericCallback.hpp"

namespace Stm32Common {
    /**
     * The GenericBounce class extends the functionality of the GenericCallback
     * class, enabling the management of multiple instances and supporting a
     * dynamic invocation mechanism for callback functions with configurable
     * nesting depth.
     *
     * This class provides mechanisms to track active instances in a static array,
     * execute callback functions within the limits of allowable nesting depth,
     * and retrieve references to static bounce functions for external use.
     *
     * @tparam Ret The return type of the callback function.
     * @tparam Args The parameter pack representing argument types for the callback function.
     */
    template<typename Ret, typename... Args>
    class GenericBounce : public GenericCallback<Ret, Args...> {
    public:
        using BounceFn = typename GenericCallback<Ret, Args...>::BounceFn;

        /**
         * Constructor of the GenericBounce class. Registers the current instance
         * in a static array of instances if there is available space.
         * This mechanism ensures that the maximum allowed number of instances
         * is not exceeded.
         *
         * @return A new instance of the GenericBounce class. If the internal
         * instance limit is reached, the instance will not be added to the
         * static tracking array.
         */
        GenericBounce() {
            if (s_currentIndex < MAX_INSTANCES) {
                s_instances[s_currentIndex++] = this;
            }
        }

        /**
         * Destructor of the GenericBounce class. Removes the current instance
         * from the static array of instances, ensuring proper cleanup and
         * preventing dangling references. Reorganizes the array by shifting
         * subsequent entries forward to fill the gap left by the removed instance.
         *
         * This mechanism maintains the integrity of the static tracking array
         * and updates the tracking index to reflect the removal of the instance.
         */
        ~GenericBounce() override {
            for (uint8_t i = 0; i < s_currentIndex; ++i) {
                if (s_instances[i] == this) {
                    // Move all subsequent entries forward
                    --s_currentIndex;
                    for (uint8_t j = i; j < s_currentIndex; ++j) {
                        s_instances[j] = s_instances[j + 1];
                    }
                    break;
                }
            }
        }

        /**
         * Executes a callback function stored in one of the active GenericBounce
         * instances. Ensures proper nesting depth handling to prevent exceeding the
         * maximum allowed depth. If the maximum depth is reached, execution is aborted
         * and a default return value is provided. The function searches for the first
         * available instance in the static tracking array and invokes its associated
         * function with the given arguments.
         *
         * @param args Variable arguments passed to the callback function.
         * @return The result of the executed callback function. If no valid instance
         * or function is found, returns a default-constructed object of type Ret.
         */
        static Ret bounce(Args... args) {
            if (s_nestingDepth >= MAX_NESTED_DEPTH) {
                if constexpr (std::is_void_v<Ret>) {
                    return;
                } else {
                    return Ret{};
                }
            }

            ++s_nestingDepth;

            GenericBounce *currentInstance = nullptr;
            for (uint8_t i = 0; i < s_currentIndex; ++i) {
                if (s_instances[i] != nullptr) {
                    currentInstance = s_instances[i];
                    break;
                }
            }

            Ret result{};
            if (currentInstance && currentInstance->function_) {
                result = currentInstance->function_(currentInstance->context_, args...);
            }

            --s_nestingDepth;

            return result;
        }

        /**
         * Retrieves a pointer to the static bounce function. This allows external
         * callers or components to obtain a reference to the function that can be
         * executed later with the appropriate arguments.
         *
         * @return A pointer to the static bounce function of type BounceFn. This
         * function encapsulates the callback logic defined in the GenericBounce
         * class.
         */
        virtual BounceFn getBounce() const {
            return &bounce;
        }

    private:
        /// Maximum number of instances
        static constexpr uint8_t MAX_INSTANCES = LIBSMART_GENERIC_BOUNCE_MAX_INSTANCES;

        /// Maximum callback nesting depth
        static constexpr uint8_t MAX_NESTED_DEPTH = LIBSMART_GENERIC_BOUNCE_MAX_NESTED_DEPTH;

        /// Static array of instances
        static inline GenericBounce *s_instances[MAX_INSTANCES] = {};
        static inline uint8_t s_currentIndex = 0;

        /// Counter for nested callbacks
        static inline uint8_t s_nestingDepth = 0;
    };
}
