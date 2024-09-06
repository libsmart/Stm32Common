/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_STREAMSESSIONINTERFACE_HPP

namespace Stm32Common::StreamSession {
    class StreamSessionInterface {
    public:
        virtual ~StreamSessionInterface() = default;

        virtual void setup() = 0;

        virtual void loop() = 0;

        virtual void end() = 0;

        virtual bool isInUse() { return inUse; }

        virtual uint32_t getId() { return id; }

        void init(uint32_t id) {
            inUse = true;
            this->id = id;
        }

        void kill() {
            inUse = false;
            id = UINT32_MAX;
        }

    private:
        bool inUse = false;
        uint32_t id = UINT32_MAX;
    };
}
#endif
