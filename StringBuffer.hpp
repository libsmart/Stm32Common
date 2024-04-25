/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STRINGBUFFER_HPP
#define LIBSMART_STM32COMMON_STRINGBUFFER_HPP

#include <cstddef>
#include "Stream.hpp"

namespace Stm32Common {
    template<size_t Size>
    class StringBuffer : public Stream {

    public:
        StringBuffer() : head(0), tail(0) {}

        [[nodiscard]] bool isEmpty() const {
            return head == tail;
        }

        [[nodiscard]] bool isFull() const {
            return head == Size;
        }

        size_t getRemainingSpace() {
            return Size - head;
        }

        size_t getLength() {
            return head - tail;
        }

        size_t write(const uint8_t c) override {
            return write(&c, 1);
        }

        size_t write(const char *str) override {
            return write(str, strlen(str));
        }

        size_t write(const void *in, size_t strlen) {
            if (getRemainingSpace() < strlen) return 0;
            memcpy(buffer + head, in, strlen);
            head += strlen;
            return strlen;
        }


        size_t printf(const char *format, ...) PRINTF_OVERRIDE
        {
            va_list args;
            va_start(args, format);
            auto ret = vprintf(format, args);
            va_end(args);
            return ret;
        }

        size_t vprintf(const char *format, va_list args) PRINTF_OVERRIDE
        {
            int len = vsnprintf(reinterpret_cast<char *>(buffer + head), getRemainingSpace(), format, args);
            if(len < 0) return 0;
            len = std::min((size_t)len, getRemainingSpace());
            head += len;
            return len;
        }

        int read() override {
            if(tail == head) return -1;
            return buffer[tail++];
        }

        size_t read(void *out, size_t size) {
            memset(out, 0, size);
            size_t sz = getLength() < size ? getLength() : size;
            memcpy(out, buffer + tail, sz);
            tail += sz;
            checkClear();
            return sz;
        }

        size_t read(StringBuffer *stringBuffer) {
            size_t sz = getLength() < stringBuffer->getRemainingSpace() ? getLength() : stringBuffer->getRemainingSpace();
            memcpy(stringBuffer->buffer + stringBuffer->head, buffer + tail, sz);
            tail += sz;
            stringBuffer->head += sz;
            checkClear();
            return sz;
        }

        int peek() override {
            if(tail == head) return -1;
            return buffer[tail];
        }

        uint8_t *getWritePointer() {
            return buffer + head;
        }

        const uint8_t *getReadPointer() {
            return buffer + tail;
        }

        /**
         * @deprecated
         */
        const uint8_t *getStart() {
            return getReadPointer();
        }

        size_t add(size_t add) {
            size_t sz = getRemainingSpace() < add ? getRemainingSpace() : add;
            head += sz;
            return sz;
        }

        size_t remove(size_t remove) {
            size_t sz = getLength() < remove ? getLength() : remove;
            tail += sz;
            return sz;
        }

        void clear() {
            memset(buffer, 0, Size);
            head = tail = 0;
        }


        int available() override {
            return getRemainingSpace();
        }

        size_t getWriteBuffer(uint8_t *&buffer) DIRECT_BUFFER_WRITE_OVERRIDE
        {
            buffer = getWritePointer();
            return getRemainingSpace();
        }

        size_t setWrittenBytes(size_t size) DIRECT_BUFFER_WRITE_OVERRIDE
        {
            return add(size);
        }

        int availableForWrite() override {
            return getRemainingSpace();
        }

        void flush() override {
            //DOES NOTHING
        }

    protected:
        void checkClear() {
            if(head == tail) clear();
        }

    private:
        uint8_t buffer[Size]{};
        volatile size_t head; // Index of the next free byte for write
        volatile size_t tail; // Index of the next byte to read

    };
}

#endif //LIBSMART_STM32COMMON_STRINGBUFFER_HPP
