/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef LIBSMART_STM32COMMON_PROCESS_PROCESSINTERFACE_HPP
#define LIBSMART_STM32COMMON_PROCESS_PROCESSINTERFACE_HPP

namespace Stm32Common::Process {
    /**
     * @brief Abstract interface for handling processing logic in the system.
     *
     * The `ProcessInterface` class defines a set of pure virtual functions that must be implemented by any derived class.
     * These functions are designed to manage the lifecycle of a process, including setup, operation, and teardown.
     */
    class ProcessInterface {
    public:
        virtual ~ProcessInterface() = default;

        /**
         * @brief Pure virtual function to set up the initial state of the process.
         *
         * This method must be implemented by any class that inherits from ProcessInterface.
         * It should be used to initialize all necessary components, allocate resources,
         * or perform any other setup operations required before the process can start running.
         */
        virtual void setup() = 0;

        /**
         * @brief Pure virtual function to run the main processing loop.
         *
         * This method must be implemented by any class that inherits from ProcessInterface.
         * It should encompass the primary logic that needs to be repeatedly executed
         * during the lifecycle of the process.
         */
        virtual void loop() = 0;

        /**
         * @brief Pure virtual function to terminate the process.
         *
         * This method must be implemented by any class that inherits from ProcessInterface.
         * It should be used to clean up all necessary components, deallocate resources,
         * or perform any other teardown operations required after the process has completed.
         */
        virtual void end() = 0;

        /**
         * @brief Pure virtual function to handle error conditions in the process.
         *
         * This method must be implemented by any class that inherits from ProcessInterface.
         * It should define the logic for managing errors, recovery steps, or logging error information.
         */
        virtual void errorHandler() = 0;
    };
}
#endif
