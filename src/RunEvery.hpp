/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_RUNEVERY_HPP
#define LIBSMART_STM32COMMON_RUNEVERY_HPP

#include "libsmart_config.hpp"

#ifdef LIBSMART_ENABLE_STD_FUNCTION

#include <cstdint>
#include <functional>
#include <iomanip>
#include <utility>

#endif

#include "Helper.hpp"

namespace Stm32Common {
    class RunEvery {
    public:
        RunEvery() = default;

        ~RunEvery() = default;

#ifdef LIBSMART_ENABLE_STD_FUNCTION
        using fn_t = std::function<void()>;

        explicit RunEvery(const uint32_t interval_ms) : _interval_ms(interval_ms) {}

        explicit RunEvery(const fn_t &fn) : RunEvery(0, fn) {}

        RunEvery(const uint32_t interval_ms, fn_t fn)
                : _interval_ms(interval_ms),
                  _fn(std::move(fn)) {}

        /**
         * @brief Set the function to be executed by the RunEvery object.
         *
         * This method sets the function to be executed by the RunEvery object. The function will be called
         * at the specified interval defined in the RunEvery object.
         *
         * @param fn The function to be executed.
         *
         * @return void
         */
        void setFunction(fn_t fn) { this->_fn = std::move(fn); };

        /**
         * @brief Executes the given loop function at the defined interval.
         *
         * This method checks if the specified interval defined in the RunEvery object has elapsed
         * since the last execution. If the interval has elapsed, the stored function will be called.
         *
         * @param loop_fn The function to be executed.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        bool loop(const fn_t &loop_fn) {
            if (isSet()) {
                loop_fn();
                reset();
                return true;
            }
            return false;
        };

        /**
         * @brief Execute the given loop function at the given interval.
         *
         * This method checks if the given interval has elapsed since the last execution.
         * If the interval has elapsed, the given function will be called.
         *
         * @param delay_ms The interval in milliseconds between function executions.
         * @param loop_fn The function to be executed.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        bool loop(const uint32_t delay_ms, const fn_t &loop_fn) {
            setInterval(delay_ms);
            return loop(loop_fn);
        };


    private:
        fn_t _fn = []() {};
#endif

    public:
        /**
         * @brief Set the interval for the RunEvery object.
         *
         * This method sets the interval for the RunEvery object. The interval determines how often the stored function will be executed.
         *
         * @param[in] interval_ms The interval in milliseconds.
         *
         * @return void
         */
        void setInterval(const uint32_t interval_ms) { _interval_ms = interval_ms; }

        /**
         * @brief Conversion operator to bool.
         *
         * This method is a conversion operator that returns a boolean value. It checks if the specified interval defined in the RunEvery object has elapsed since the last execution. If the interval has elapsed, it returns true; otherwise, it returns false.
         *
         * @return true if the interval has elapsed, false otherwise.
         */
        explicit operator bool() const { return isSet(); }

        /**
         * @brief Reset the last run time of the RunEvery object.
         *
         * This method resets the last run time of the RunEvery object to the current millis value.
         * By resetting the last run time, the interval will start counting from the moment this method is called.
         *
         * @return void
         */
        void reset() { _last_last_run_ms = millis(); }

        /**
         * @brief Get the elapsed time since the last run of the RunEvery object.
         *
         * This method returns the elapsed time in milliseconds since the last execution of the RunEvery object.
         * The elapsed time is calculated by subtracting the last run time from the current millis value.
         *
         * @return The elapsed time [ms].
         */
        [[nodiscard]] uint32_t elapsed() const { return millis() - _last_last_run_ms; }

        /**
         * @brief Check if the specified interval has elapsed since the last execution.
         *
         * This method checks if the specified interval defined in the RunEvery object has elapsed since the last execution.
         * If the interval has elapsed, it returns true; otherwise, it returns false.
         *
         * @return true if the specified interval has elapsed, false otherwise.
         */
        [[nodiscard]] bool isSet() const { return elapsed() >= _interval_ms; }

        /**
         * @brief Executes the stored loop function at the defined interval.
         *
         * This function checks if the specified interval defined in the RunEvery object has elapsed
         * since the last execution. If the interval has elapsed, the stored function will be called.
         * Otherwise, it returns false.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        bool loop() {
            if (isSet()) {
#ifdef LIBSMART_ENABLE_STD_FUNCTION
                _fn();
#endif
                reset();
                return true;
            }
            return false;
        };

        /**
         * @brief Executes the stored loop function at the defined interval.
         *
         * This function checks if the specified interval defined in the RunEvery object has elapsed
         * since the last execution. If the interval has elapsed, the stored function will be called.
         *
         * @param delay_ms The interval in milliseconds between function executions.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        bool loop(const uint32_t delay_ms) {
            setInterval(delay_ms);
            return loop();
        };


    private:
        uint32_t _last_last_run_ms = {};
        uint32_t _interval_ms = {};

    };
}
#endif //LIBSMART_STM32COMMON_RUNEVERY_HPP
