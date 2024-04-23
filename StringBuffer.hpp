/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STRINGBUFFER_HPP
#define LIBSMART_STM32COMMON_STRINGBUFFER_HPP

#include <cstddef>

namespace Stm32Common {
    template<size_t Size>
    class StringBuffer {

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


        bool write(const char *str) {
            return write(str, strlen(str));
        }

        bool write(const void *in, size_t strlen) {
            if (getRemainingSpace() < strlen) return false;
            memcpy(buffer + head, in, strlen);
            head += strlen;
            return true;
        }

        bool printf(const char *format, ...) {
            va_list args;
            va_start(args, format);
            auto ret = vprintf(format, args);
            va_end(args);
            return ret;
        }

        bool vprintf(const char *format, va_list args) {
            int len = vsnprintf(reinterpret_cast<char *>(buffer + head), getRemainingSpace(), format, args);
            if (len < 0 || len >= getRemainingSpace()) return false;
            head += len;
            return true;
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

        uint8_t *getStart() {
            return buffer + tail;
        }

        void clear() {
            memset(buffer, 0, Size);
            head = tail = 0;
        }

    protected:
        void checkClear() {
            if(head == tail) clear();
        }

    private:
        uint8_t buffer[Size]{};
        size_t head; // Index des nächsten freien Elements
        size_t tail; // Index des nächsten zu lesenden Elements

    };
}

#endif //LIBSMART_STM32COMMON_STRINGBUFFER_HPP
