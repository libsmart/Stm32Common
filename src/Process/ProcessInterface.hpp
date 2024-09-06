/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef LIBSMART_STM32COMMON_PROCESS_PROCESSINTERFACE_HPP
#define LIBSMART_STM32COMMON_PROCESS_PROCESSINTERFACE_HPP

namespace Stm32Common::Process {
    class ProcessInterface {
    public:
        virtual ~ProcessInterface() = default;

        /**
       * @brief This pure virtual function is part of the MachineComponentInterface and needs to be overridden by derived classes.
       *
       * This function is responsible for setting up the machine component. It should be used to initialize any necessary resources, configurations, or variables.
       * The derived classes should implement their own setup logic based on their specific requirements.
       */
        virtual void setup() = 0;

        /**
         * @brief This pure virtual function is part of the MachineComponentInterface and needs to be overridden by derived classes.
         *
         * This function is called repeatedly in the main program loop and is responsible for performing the main tasks or computations of the machine component.
         */
        virtual void loop() = 0;
    };
}
#endif
