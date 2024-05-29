/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_RUNEVERY_HPP
#define LIBSMART_STM32COMMON_RUNEVERY_HPP

#include <libsmart_config.hpp>

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

        virtual ~RunEvery() = default;

        explicit RunEvery(const uint32_t interval_and_delay_ms)
            : _delay_ms(interval_and_delay_ms),
              _interval_ms(interval_and_delay_ms) { ; }

        RunEvery(const uint32_t interval_ms, const uint32_t delay_ms)
            : _delay_ms(delay_ms),
              _interval_ms(interval_ms) { ; }

        RunEvery(const uint32_t interval_ms, const uint32_t delay_ms, const uint32_t run_count_max)
            : _run_count_max(run_count_max),
              _delay_ms(delay_ms),
              _interval_ms(interval_ms) { ; }

#ifdef LIBSMART_ENABLE_STD_FUNCTION
        using fn_t = std::function<void()>;

        explicit RunEvery(const fn_t &fn) : RunEvery(0, fn) { ; }

        RunEvery(const uint32_t interval_and_delay_ms, fn_t fn)
            : _fn(std::move(fn)),
              _delay_ms(interval_and_delay_ms),
              _interval_ms(interval_and_delay_ms) { ; }

        RunEvery(const uint32_t interval_ms, const uint32_t delay_ms, fn_t fn)
            : _fn(std::move(fn)),
              _delay_ms(delay_ms),
              _interval_ms(interval_ms) { ; }

        RunEvery(const uint32_t interval_ms, const uint32_t delay_ms, const uint32_t run_count_max, fn_t fn)
            : _fn(std::move(fn)),
              _run_count_max(run_count_max),
              _delay_ms(delay_ms),
              _interval_ms(interval_ms) { ; }

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
        virtual void setFunction(fn_t fn) { this->_fn = std::move(fn); }

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
        virtual bool loop(const fn_t &loop_fn) {
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
         * @param interval_ms The interval in milliseconds between function executions.
         * @param loop_fn The function to be executed.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        virtual bool loop(const uint32_t interval_ms, const fn_t &loop_fn) {
            setInterval(interval_ms);
            return loop(loop_fn);
        };

    private:
        fn_t _fn = []() { ; };
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
        virtual void setInterval(const uint32_t interval_ms) { _interval_ms = interval_ms; }


        /**
         * @brief Set the delay in milliseconds for the RunEvery object.
         *
         * This method sets the delay in milliseconds for the RunEvery object. The delay determines the interval at
         * which the function attached to the RunEvery object will be executed.
         *
         * @param delay_ms The delay in milliseconds.
         *
         * @return void
         */
        virtual void setDelay(const uint32_t delay_ms) { _delay_ms = delay_ms; }


        /**
         * @brief Set the maximum number of runs allowed for the RunEvery object.
         *
         * This method sets the maximum number of runs allowed for the RunEvery object. Once the number of runs
         * reaches this maximum value, the RunEvery object will stop executing.
         *
         * @param run_count_max The maximum number of runs allowed.
         *
         * @return void
         */
        virtual void setRunCountMax(const uint32_t run_count_max) { _run_count_max = run_count_max; }


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
        virtual void reset() {
            _run_count++;
            _last_last_run_ms = millis();
        }

        /**
         * @brief Get the elapsed time since the last run of the RunEvery object.
         *
         * This method returns the elapsed time in milliseconds since the last execution of the RunEvery object.
         * The elapsed time is calculated by subtracting the last run time from the current millis value.
         *
         * @return The elapsed time [ms].
         */
        [[nodiscard]] virtual uint32_t elapsed() const { return millis() - _last_last_run_ms; }

        /**
         * @brief Check if the RunEvery object is set to execute the function.
         *
         * This method checks if the RunEvery object is set by evaluating the conditions for execution.
         *
         * @return The result of the evaluation as a boolean. Returns true if the object is set, false otherwise.
         */
        [[nodiscard]] virtual bool isSet() const {
            return (_run_count_max == 0 || _run_count < _run_count_max) &&
                   (_run_count == 0 ? elapsed() >= _delay_ms : elapsed() >= _interval_ms);
        }

        /**
         * @brief Executes the stored loop function at the defined interval.
         *
         * This function checks if the specified interval defined in the RunEvery object has elapsed
         * since the last execution. If the interval has elapsed, the stored function will be called.
         * Otherwise, it returns false.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        virtual bool loop() {
            if (isSet()) {
#ifdef LIBSMART_ENABLE_STD_FUNCTION
                _fn();
#endif
                reset();
                return true;
            }
            return false;
        }

        /**
         * @brief Executes the stored loop function at the defined interval.
         *
         * This function checks if the specified interval defined in the RunEvery object has elapsed
         * since the last execution. If the interval has elapsed, the stored function will be called.
         *
         * @param interval_ms The interval in milliseconds between function executions.
         *
         * @return true if the specified interval has elapsed and the function is executed, false otherwise.
         */
        virtual bool loop(const uint32_t interval_ms) {
            setInterval(interval_ms);
            return loop();
        }

    protected:
        /**
         * @brief Represents the timestamp of the last execution of a particular function or task.
         */
        uint32_t _last_last_run_ms = millis();

        /**
         * @brief Number of times the function has been executed.
         */
        uint32_t _run_count = {};

        /**
         * @brief Maximum number of times the function can be run.
         */
        uint32_t _run_count_max = {};

        /**
         * @brief Represents the delay in milliseconds for the RunOnce utility class.
         */
        uint32_t _delay_ms = {};

        /**
         * @brief The interval in milliseconds.
         */
        uint32_t _interval_ms = {};
    };
}
#endif
