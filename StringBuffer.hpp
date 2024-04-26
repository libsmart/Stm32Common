/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STRINGBUFFER_HPP
#define LIBSMART_STM32COMMON_STRINGBUFFER_HPP

#include <cstddef>
#include "Stream.hpp"

namespace Stm32Common {

    typedef size_t buf_size_t;
    typedef int64_t buf_size_signed_t;

    template<buf_size_t Size>
    class StringBuffer : public Stream {
    public:
        StringBuffer() { init(); };

        [[nodiscard]] bool isEmpty() const {
            return head == tail;
        }

        [[nodiscard]] bool isFull() const {
            return head == Size;
        }

        [[nodiscard]] buf_size_t getRemainingSpace() const {
            return Size - head;
        }

        [[nodiscard]] buf_size_t getLength() const {
            return head - tail;
        }

        buf_size_t write(const uint8_t c) override {
            return write(&c, 1);
        }

        buf_size_t write(const char *str) override {
            return write(str, strlen(str));
        }

        buf_size_t write(const void *in, buf_size_t strlen) {
            if (in == nullptr) return 0;
            if (strlen == 0) return 0;
            if (getRemainingSpace() < strlen) return 0;
            memcpy(getWritePointer(), in, strlen);
            return add(strlen);
        }

        buf_size_t printf(const char *format, ...) PRINTF_OVERRIDE {
            va_list args;
            va_start(args, format);
            const auto ret = vprintf(format, args);
            va_end(args);
            return ret;
        }

        buf_size_t vprintf(const char *format, va_list args) PRINTF_OVERRIDE {
            const int len = vsnprintf(reinterpret_cast<char *>(getWritePointer()), getRemainingSpace(), format, args);
            if (len <= 0) return 0;
            return add(std::min(static_cast<buf_size_t>(len), getRemainingSpace()));
        }

        int read() override {
            if (getLength() < 1) return -1;
            const int ret = buffer[tail];
            remove(1);
            return ret;
        }

        buf_size_t read(void *out, buf_size_t size) {
            memset(out, 0, size);
            const buf_size_t sz = std::min(getLength(), size);
            memcpy(out, getReadPointer(), sz);
            return remove(sz);
        }

        buf_size_t read(StringBuffer *stringBuffer) {
            const size_t sz = std::min(getLength(), stringBuffer->getRemainingSpace());
            memcpy(stringBuffer->getWritePointer(), getReadPointer(), sz);
            stringBuffer->add(sz);
            return remove(sz);
        }

        int peek() override {
            if (isEmpty()) return -1;
            return buffer[tail];
        }

        int peek(buf_size_t pos) {
            if (pos + tail > head || isEmpty()) return -1;
            return buffer[tail + pos];
        }

        uint8_t *getWritePointer() {
            return buffer + head;
        }

        const uint8_t *getReadPointer() {
            return buffer + tail;
        }

        buf_size_t add(const buf_size_t add) {
            const size_t sz = std::min(getRemainingSpace(), add);
            if (sz == 0) return 0;
            head += sz;
            if (head == sz) onNonEmpty();
            onWrite();
            return sz;
        }

        buf_size_t remove(const buf_size_t remove) {
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

        buf_size_signed_t findPos(const uint8_t c) {
            for (buf_size_t i = 0; i < getLength(); i++) {
                if(const auto ch = peek(i); ch == c) return i;
            }
            return -1;
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
        volatile size_t head = 0; // Index of the next free byte for write
        volatile size_t tail = 0; // Index of the next byte to read
    };
}

#endif //LIBSMART_STM32COMMON_STRINGBUFFER_HPP
