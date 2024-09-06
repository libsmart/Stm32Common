/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONINTERFACE_HPP

#include "Loggable.hpp"
#include "Nameable.hpp"
#include "StreamRxTxInterface.hpp"
#include "Process/ProcessInterface.hpp"

namespace Stm32Common::StreamSession {
    template<class StreamSessionT, size_t MaxSessionCount>
    class Manager;

    class StreamSessionInterface : public virtual StreamRxTxInterface, public Process::ProcessInterface,
                                   public Nameable,
                                   public Stm32ItmLogger::Loggable {
    public:
        template<class StreamSessionT, size_t MaxSessionCount>
        friend class Manager;

        ~StreamSessionInterface() override = default;

        /**
         * @brief Checks if the current session is in use.
         *
         * This method returns whether the session is marked as in use.
         *
         * @return True if the session is in use, false otherwise.
         */
        virtual bool isInUse() { return inUse; }

        /**
         * @brief Retrieves the unique identifier for the stream session.
         *
         * This method returns the ID associated with the current stream session.
         *
         * @return The unique identifier of the stream session as a 32-bit unsigned integer.
         */
        virtual uint32_t getId() { return id; }

    private:
        /**
         * @brief Initializes the stream session with the specified ID.
         *
         * This method marks the session as in use and assigns the provided
         * unique identifier to the session.
         *
         * @param id The unique identifier to be assigned to the session.
         * @note Used by the manager
         */
        void init(uint32_t id) {
            inUse = true;
            this->id = id;
        }

        /**
         * @brief Terminates the current stream session.
         *
         * This method marks the session as no longer in use and resets its
         * unique identifier to the maximum possible value for a 32-bit unsigned integer.
         *
         * @note Used by the manager
         */
        void kill() {
            inUse = false;
            id = UINT32_MAX;
        }

        bool inUse = false;
        uint32_t id = UINT32_MAX;
    };
}
#endif
