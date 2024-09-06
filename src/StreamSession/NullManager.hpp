/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_NULLMANAGER_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_NULLMANAGER_HPP

#include "ManagerInterface.hpp"

namespace Stm32Common::StreamSession {
    class NullManager : public ManagerInterface {
    public:
        void setup() override { ; }

        void loop() override { ; }

        void end() override { ; }

        StreamSessionInterface *getNewSession(uint32_t id) override { return nullptr; }

        void removeSession(StreamSessionInterface *session) override { ; }

        StreamSessionInterface *getSessionById(uint32_t id) override { return nullptr; }

        StreamSessionInterface *getFirstSession() override { return nullptr; }

        StreamSessionInterface *getNextSession(StreamSessionInterface *session) override { return nullptr; }

        void removeAll() override { ; }

        size_t getFreeSessions() override { return 0; }
    };

    inline NullManager nullManager;
}
#endif
