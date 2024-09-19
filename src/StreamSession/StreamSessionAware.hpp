/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONAWAREINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONAWAREINTERFACE_HPP

#include "ManagerInterface.hpp"
#include "NullManager.hpp"

namespace Stm32Common::StreamSession {
    /**
     * StreamSessionAware class provides an interface for objects that are aware of a stream session.
     * It holds a reference to a session manager and provides a method to access it.
     */
    class StreamSessionAware {
    public:
        virtual ~StreamSessionAware() = default;

        explicit StreamSessionAware(ManagerInterface *session_mgr)
            : sessionMgr(session_mgr) {
        }

        /**
         * Notifies that data is ready to be transmitted for the given session.
         *
         * @param session A pointer to the StreamSessionInterface indicating the session for which the data is ready.
         */
        virtual void dataReadyTx(StreamSessionInterface *session) { ; }

    protected:
        virtual bool hasSessionManager() {
            return sessionMgr != nullptr;
        }

        /**
         * Provides access to the session manager. If the session manager is not available, a null manager is returned.
         *
         * @return A pointer to the session manager, or a null manager if the session manager is not initialized.
         */
        virtual ManagerInterface *getSessionManager() {
            if (sessionMgr == nullptr) {
                return &nullManager;
            }
            return sessionMgr;
        }

    private:
        ManagerInterface *sessionMgr = {};
    };
}
#endif
