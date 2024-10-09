/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_NULLSTREAMSESSION_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_NULLSTREAMSESSION_HPP
#include "Helper.hpp"
#include "NullStringBuffer.hpp"
#include "StreamSessionInterface.hpp"

namespace Stm32Common::StreamSession {
    class NullStreamSession : public StreamSessionInterface {
    public:
#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        size_t getWriteBuffer(uint8_t *&buffer) override {
            buffer = nullptr;
            return 0;
        }
#endif

        NullStreamSession() {
            Nameable::setName("NullStreamSession");
        }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        size_t setWrittenBytes(size_t size) override { return 0; }
#endif

        size_t write(uint8_t data) override { return 0; }

        int availableForWrite() override { return 0; }

        void flush() override { ; }

        int available() override { return 0; }

        int read() override { return -1; }

        int peek() override { return -1; }

        StringBufferInterface *getRxBuffer() override { return &nullStringBuffer; }

        StringBufferInterface *getTxBuffer() override { return &nullStringBuffer; }

        void setup() override { ; }

        void loop() override { ; }

        void end() override { ; }

        void errorHandler() override { ; }
    };

    inline NullStreamSession nullStreamSession;
}
#endif
