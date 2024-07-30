/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_RUNONCE_HPP
#define LIBSMART_STM32COMMON_RUNONCE_HPP

#include <libsmart_config.hpp>

#include "Helper.hpp"

#ifdef LIBSMART_ENABLE_STD_FUNCTION

#include <cstdint>
#include <functional>
#include <iomanip>
#include <utility>

#endif

namespace Stm32Common {
    class RunOnce : public RunEvery {
    public:
        RunOnce() : RunOnce(0) { ; }

        explicit RunOnce(const uint32_t delay_ms) : RunEvery(delay_ms, delay_ms, 1) { ; }

#ifdef LIBSMART_ENABLE_STD_FUNCTION

        explicit RunOnce(fn_t fn) : RunOnce(0, std::move(fn)) { ; }

        RunOnce(const uint32_t delay_ms, fn_t fn)
            : RunEvery(delay_ms, delay_ms, 1, std::move(fn)) { ; }


        /**
         * @brief Executes the loop function.
         *
         * This method executes the loop function of the RunEvery base class with the given delay interval.
         *
         * @param delay_ms The delay in milliseconds.
         * @param loop_fn The function to be executed in the loop.
         * @return True if the loop function was executed successfully, false otherwise.
         */
        bool loop(const uint32_t delay_ms, const fn_t &loop_fn) override {
            setDelay(delay_ms);
            return RunEvery::loop(loop_fn);
        }
#endif

        /**
         * @brief Sets the maximum number of times the method can be run.
         *
         * This method sets the maximum number of times the method can be run
         * before it stops executing.
         *
         * @param run_count_max The maximum number of times the method can be run.
         * @note The given value is ignored for the class RunOnce
         */
        void setRunCountMax(const uint32_t run_count_max) override { UNUSED(run_count_max); }


        /**
         * @brief Executes the loop function.
         *
         * This method executes the loop function of the RunEvery base class with the given delay interval.
         *
         * @param delay_ms The delay in milliseconds.
         * @return True if the loop function was executed successfully, false otherwise.
         */
        bool loop(const uint32_t delay_ms) override {
            setDelay(delay_ms);
            return RunEvery::loop();
        }

        using RunEvery::loop;
    };
}
#endif
