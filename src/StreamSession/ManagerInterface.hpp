/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_MANAGERINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_MANAGERINTERFACE_HPP
#include <main.h>
#include "StreamSessionInterface.hpp"
#include "Process/ProcessInterface.hpp"

namespace Stm32Common::StreamSession {
    class ManagerInterface : public virtual Process::ProcessInterface, public Stm32ItmLogger::Loggable {
    public:
        ~ManagerInterface() override = default;

        /**
         * @brief Creates a new session with the specified identifier.
         *
         * This pure virtual method must be implemented by any class inheriting from StreamSessionInterface.
         * It is responsible for creating a new StreamSessionInterface instance and assigning it the provided identifier.
         *
         * @param id The unique identifier for the new session.
         * @return A pointer to the newly created StreamSessionInterface instance.
         */
        virtual StreamSessionInterface *getNewSession(uint32_t id) = 0;

        /**
         * @brief Removes the specified session.
         *
         * This pure virtual method must be implemented by any class inheriting from ManagerInterface.
         * It is responsible for handling the removal of the provided StreamSessionInterface instance.
         *
         * @param session A pointer to the StreamSessionInterface instance to be removed.
         */
        virtual void removeSession(StreamSessionInterface *session) = 0;

        /**
         * @brief Retrieves a session by its identifier.
         *
         * This pure virtual method must be implemented by any class inheriting from StreamSessionInterface.
         * It is responsible for returning a pointer to the StreamSessionInterface instance associated with the provided identifier.
         *
         * @param id The unique identifier for the session to retrieve.
         * @return A pointer to the StreamSessionInterface instance associated with the given identifier, or nullptr if no such session exists.
         */
        virtual StreamSessionInterface *getSessionById(uint32_t id) = 0;

        /**
         * @brief Retrieves the first available session.
         *
         * This pure virtual method must be implemented by any class inheriting from ManagerInterface.
         * It is responsible for returning a pointer to the first available StreamSessionInterface instance.
         *
         * @return A pointer to the first available StreamSessionInterface instance, or nullptr if no sessions are available.
         */
        virtual StreamSessionInterface *getFirstSession() = 0;

        /**
         * @brief Retrieves the next session in the sequence after the provided session.
         *
         * This pure virtual method must be implemented by any class inheriting from StreamSessionInterface.
         * It is responsible for returning a pointer to the StreamSessionInterface instance that comes after the provided session.
         *
         * @param session A pointer to the current StreamSessionInterface instance.
         * @return A pointer to the next StreamSessionInterface instance, or nullptr if there are no more sessions in sequence.
         */
        virtual StreamSessionInterface *getNextSession(StreamSessionInterface *session) = 0;

        /**
         * @brief Removes all current sessions.
         *
         * This pure virtual method must be implemented by any class inheriting from ManagerInterface.
         * It is responsible for handling the removal of all existing StreamSessionInterface instances.
         */
        virtual void removeAll() = 0;

        /**
         * @brief Retrieves the number of free sessions available.
         *
         * This pure virtual method must be implemented by any class inheriting from ManagerInterface.
         * It is responsible for returning the count of currently available (free) sessions.
         *
         * @return The number of free sessions as a size_t value.
         */
        virtual size_t getFreeSessions() = 0;

    };
}

#endif
