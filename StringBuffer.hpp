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
        StringBuffer() { init(); };

        [[nodiscard]] bool isEmpty() const {
            return head == tail;
        }

        [[nodiscard]] bool isFull() const {
            return head == Size;
        }

        [[nodiscard]] size_t getRemainingSpace() const {
            return Size - head;
        }

        [[nodiscard]] size_t getLength() const {
            return head - tail;
        }

        size_t write(const uint8_t c) override {
            return write(&c, 1);
        }

        size_t write(const char *str) override {
            return write(str, strlen(str));
        }

        size_t write(const void *in, size_t strlen) {
            if (in == nullptr) return 0;
            if (strlen == 0) return 0;
            if (getRemainingSpace() < strlen) return 0;

            memcpy(getWritePointer(), in, strlen);
            add(strlen);
            return strlen;
        }

        size_t printf(const char *format, ...) PRINTF_OVERRIDE {
            va_list args;
            va_start(args, format);
            const auto ret = vprintf(format, args);
            va_end(args);
            return ret;
        }

        size_t vprintf(const char *format, va_list args) PRINTF_OVERRIDE {
            const int len = vsnprintf(reinterpret_cast<char *>(getWritePointer()), getRemainingSpace(), format, args);
            if (len <= 0) return 0;
            return add(std::min(static_cast<size_t>(len), getRemainingSpace()));
        }

        int read() override {
            if (getLength() < 1) return -1;
            const int ret = buffer[tail];
            remove(1);
            return ret;
        }

        size_t read(void *out, size_t size) {
            memset(out, 0, size);
            const size_t sz = std::min(getLength(), size);
            memcpy(out, getReadPointer(), sz);
            return remove(sz);
        }

        size_t read(StringBuffer *stringBuffer) {
            const size_t sz = std::min(getLength(), stringBuffer->getRemainingSpace());
            memcpy(stringBuffer->getWritePointer(), getReadPointer(), sz);
            stringBuffer->add(sz);
            return remove(sz);
        }

        int peek() override {
            if (isEmpty()) return -1;
            return buffer[tail];
        }

        uint8_t *getWritePointer() {
            return buffer + head;
        }

        const uint8_t *getReadPointer() {
            return buffer + tail;
        }

        size_t add(const size_t add) {
            const size_t sz = std::min(getRemainingSpace(), add);
            if (sz == 0) return 0;
            head += sz;
            if (head == sz) onNonEmpty();
            onWrite();
            return sz;
        }

        size_t remove(const size_t remove) {
            const size_t sz = std::min(getLength(), remove);
            if (sz == 0) return 0;
            tail += sz;
            if (head == tail) {
                clear();
                onEmpty();
            }
            onRead();
            return sz;
        }

        void clear() {
            head = tail = 0;
            memset(buffer, 0, Size);
        }

        int available() override {
            return getRemainingSpace();
        }

        size_t getWriteBuffer(uint8_t *&buffer) DIRECT_BUFFER_WRITE_OVERRIDE {
            buffer = getWritePointer();
            return getRemainingSpace();
        }

        size_t setWrittenBytes(size_t size) DIRECT_BUFFER_WRITE_OVERRIDE {
            return add(size);
        }

        int availableForWrite() override {
            return getRemainingSpace();
        }

        void flush() override {
            //DOES NOTHING
        }

    protected:
        virtual void onInit() {
        }

        virtual void onEmpty() {
        }

        virtual void onNonEmpty() {
        }

        virtual void onWrite() {
        }

        virtual void onRead() {
        }

    private:
        void init() {
            onInit();
            onEmpty();
        }

        uint8_t buffer[Size] = {};
        size_t head = 0; // Index of the next free byte for write
        size_t tail = 0; // Index of the next byte to read
    };
}

#endif //LIBSMART_STM32COMMON_STRINGBUFFER_HPP
