/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_NULLSTRINGBUFFER_HPP
#define LIBSMART_STM32COMMON_NULLSTRINGBUFFER_HPP

#include "StringBufferInterface.hpp"

namespace Stm32Common {
    class NullStringBuffer : public StringBufferInterface {
    public:
        size_t getWriteBuffer(uint8_t *&buffer) override {
            buffer = nullptr;
            return 0;
        }

        size_t setWrittenBytes(size_t size) override { return 0; }

        size_t write(uint8_t data) override { return 0; }

        int availableForWrite() override { return 0; }

        void flush() override { ; }

        int available() override { return 0; }

        int read() override { return -1; }

        int peek() override { return -1; }

        bool isEmpty() override { return true; }

        bool isFull() override { return true; }

        buf_size_t getRemainingSpace() override { return 0; }

        buf_size_t getLength() override { return 0; }

        buf_size_t read(void *out, buf_size_t size) override { return 0; }

        buf_size_t read(StringBufferInterface *stringBuffer) override { return 0; }

        int peek(buf_size_t pos) override { return -1; }

        uint8_t *getWritePointer() override { return nullptr; }

        const uint8_t *getReadPointer() override { return nullptr; }

        buf_size_t add(buf_size_t add) override { return 0; }

        buf_size_t remove(buf_size_t remove) override { return 0; }

        void clear() override { ; }

        buf_size_signed_t findPos(uint8_t c) override { return -1; }

        void setOnInitFn(const onInitFn_t &on_init_fn) override { ; }

        void setOnEmptyFn(const onEmptyFn_t &on_empty_fn) override { ; }

        void setOnNonEmptyFn(const onNonEmptyFn_t &on_non_empty_fn) override { ; }

        void setOnWriteFn(const onWriteFn_t &on_write_fn) override { ; }

        void setOnReadFn(const onReadFn_t &on_read_fn) override { ; }
    };

    inline NullStringBuffer nullStringBuffer;
}
#endif
