/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_MANAGER_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_MANAGER_HPP

#include <main.h>
#include <array>
#include "Loggable.hpp"
#include "ManagerInterface.hpp"
#include "StreamSessionInterface.hpp"

namespace Stm32Common::StreamSession {
    template<class StreamSessionT, size_t MaxSessionCount>
    class Manager : public ManagerInterface {
        static_assert(std::is_base_of<StreamSessionInterface, StreamSessionT>::value,
                      "StreamSession must be of type StreamSessionInterface");

    public:
        ~Manager() override = default;

        StreamSessionInterface *getNewSession(uint32_t id) override {
            if (getSessionById(id) != nullptr) return nullptr;
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (!sessions[i].isInUse()) {
                    sessions[i].init(id);
                    // sessions[i].setup();
                    return &sessions[i];
                }
            }
            return nullptr;
        }


        void removeSession(StreamSessionInterface *session) override {
            if (session == nullptr) return;
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (&sessions[i] == session && sessions[i].isInUse()) {
                    sessions[i].end();
                    sessions[i].kill();
                }
            }
        }

        StreamSessionInterface *getSessionById(uint32_t id) override {
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (sessions[i].isInUse() && sessions[i].getId() == id) {
                    return &sessions[i];
                }
            }
            return nullptr;
        }

        StreamSessionInterface *getFirstSession() override {
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (sessions[i].isInUse()) {
                    return &sessions[i];
                }
            }
            return nullptr;
        }

        StreamSessionInterface *getNextSession(StreamSessionInterface *session) override {
            if (session == nullptr) return nullptr;
            bool found = false;
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (found && sessions[i].isInUse()) {
                    return &sessions[i];
                }
                if (!found && &sessions[i] == session) { found = true; }
            }
            return nullptr;
        }

        void removeAll() override {
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (sessions[i].isInUse()) {
                    removeSession(&sessions[i]);
                }
            }
        }

        size_t getFreeSessions() override {
            size_t freeSessions = std::size(sessions);
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (sessions[i].isInUse()) freeSessions--;
            }
            return freeSessions;
        }

        void setup() override { ; }

        void loop() override {
            for (size_t i = 0; i < MaxSessionCount; i++) {
                if (sessions[i].isInUse()) {
                    sessions[i].loop();
                }
            }
        }

        virtual void end() { removeAll(); }

    private:
        std::array<StreamSessionT, MaxSessionCount> sessions = {};
    };
}

#endif
