/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STRINGBUFFER_HPP
#define LIBSMART_STM32COMMON_STRINGBUFFER_HPP

#include <algorithm>
#include <libsmart_config.hpp>
#include <cstddef>
#include "Stream.hpp"

namespace Stm32Common {
    typedef size_t buf_size_t;
    typedef int64_t buf_size_signed_t;

    template<buf_size_t Size>
    class StringBuffer : public Stream {
    public:
        StringBuffer() { init(); };

        /**
         * Check if the StringBuffer is empty.
         *
         * This method checks if the StringBuffer is empty by comparing the head index with the tail index.
         *
         * \return True if the StringBuffer is empty, false otherwise.
         */
        [[nodiscard]] bool isEmpty() const {
            return head == tail;
        }

        /**
         * Check if the StringBuffer is full.
         *
         * This method checks if the StringBuffer is full by comparing the head index with the maximum buffer size (Size).
         *
         * \return True if the StringBuffer is full, false otherwise.
         */
        [[nodiscard]] bool isFull() const {
            return head == Size;
        }

        /**
         * Get the remaining space of the StringBuffer.
         *
         * This method returns the number of bytes that are available for writing to the StringBuffer. It is calculated by subtracting the head index from the maximum buffer size (Size).
         *
         * \return The number of bytes available for writing.
         */
        [[nodiscard]] buf_size_t getRemainingSpace() const {
            return Size - head;
        }

        /**
         * Get the length of the StringBuffer.
         *
         * This method returns the number of bytes currently stored in the StringBuffer. It is calculated by subtracting the tail index from the head index.
         *
         * \return The length of the StringBuffer.
         */
        [[nodiscard]] buf_size_t getLength() const {
            return head - tail;
        }

        /**
         * Write a single byte to the StringBuffer.
         *
         * This method writes a single byte to the StringBuffer by calling the write method with the byte buffer and size of 1.
         *
         * @param c The byte to be written to the StringBuffer.
         * @return The number of bytes written to the StringBuffer.
         */
        buf_size_t write(const uint8_t c) override {
            return write(&c, 1);
        }

        /**
         * Write a null-terminated string to the StringBuffer.
         *
         * This method writes a null-terminated string to the StringBuffer by calling the write method with the byte buffer and the length of the string calculated using strlen() function.
         *
         * @param str The null-terminated string to be written to the StringBuffer.
         * @return The number of bytes written to the StringBuffer.
         */
        buf_size_t write(const char *str) override {
            return write(str, strlen(str));
        }

        buf_size_t write(const void *in, buf_size_t strlen) {
            if (in == nullptr) return 0;
            if (strlen == 0) return 0;
            if (getRemainingSpace() < strlen) return 0;
            memcpy(_getWritePointer(), in, strlen);
            return add(strlen);
        }

#ifdef LIBSMART_ENABLE_PRINTF
        buf_size_t printf(const char *format, ...) PRINTF_OVERRIDE {
            va_list args;
            va_start(args, format);
            const auto ret = vprintf(format, args);
            va_end(args);
            return ret;
        }


        buf_size_t vprintf(const char *format, va_list args) PRINTF_OVERRIDE {
            const int len = vsnprintf(reinterpret_cast<char *>(_getWritePointer()), getRemainingSpace(), format, args);
            if (len <= 0) return 0;
            return add(std::min(static_cast<buf_size_t>(len), getRemainingSpace()));
        }
#endif

        int read() override {
            if (getLength() < 1) return -1;
            const int ret = buffer[tail];
            remove(1);
            return ret;
        }

        buf_size_t read(void *out, buf_size_t size) {
            memset(out, 0, size);
            const buf_size_t sz = std::min(getLength(), size);
            memcpy(out, _getReadPointer(), sz);
            return remove(sz);
        }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        buf_size_t read(StringBuffer *stringBuffer) {
            const size_t sz = std::min(getLength(), stringBuffer->getRemainingSpace());
            memcpy(stringBuffer->getWritePointer(), getReadPointer(), sz);
            stringBuffer->add(sz);
            return remove(sz);
        }
#endif


        int peek() override {
            if (isEmpty()) return -1;
            return buffer[tail];
        }

        int peek(buf_size_t pos) {
            if (pos + tail > head || isEmpty()) return -1;
            return buffer[tail + pos];
        }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        uint8_t *getWritePointer() {
            return _getWritePointer();
        }
#endif

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_READ
        const uint8_t *getReadPointer() {
            return _getReadPointer();
        }
#endif

    private:
        uint8_t *_getWritePointer() {
            return buffer + head;
        }

        const uint8_t *_getReadPointer() {
            return buffer + tail;
        }

    public:
        buf_size_t add(const buf_size_t add) {
            const size_t sz = std::min(getRemainingSpace(), add);
            if (sz == 0) return 0;
            head += sz;
            if (head == sz) onNonEmpty();
            onWrite();
            return sz;
        }

        /**
         * Remove a specified number of bytes from the StringBuffer.
         *
         * @param remove The number of bytes to remove.
         * @return The actual number of bytes removed from the StringBuffer.
         */
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

        /**
         * Clear the StringBuffer by resetting head and tail indices to 0 and
         * clearing the buffer with zero values.
         */
        void clear() {
            head = tail = 0;
            std::memset(buffer, 0, Size);
        }

        /**
         * Get the number of bytes available for reading from the StringBuffer.
         *
         * This method returns the number of bytes that are available for reading from the StringBuffer by calling the
         * getLength() method.
         *
         * @return The number of bytes available for reading.
         */
        int available() override {
            return getLength();
        }

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        size_t getWriteBuffer(uint8_t *&buffer) DIRECT_BUFFER_WRITE_OVERRIDE {
            buffer = _getWritePointer();
            return getRemainingSpace();
        }

        size_t setWrittenBytes(size_t size) DIRECT_BUFFER_WRITE_OVERRIDE {
            return add(size);
        }
#endif

        /**
         * Get the number of bytes available for writing to the StringBuffer.
         *
         * This method returns the number of bytes that are available for writing to the StringBuffer by calling the
         * getRemainingSpace() method.
         *
         * @return The number of bytes available for writing.
         */
        int availableForWrite() override {
            return getRemainingSpace();
        }

        void flush() override {
            //DOES NOTHING
        }

        buf_size_signed_t findPos(const uint8_t c) {
            for (buf_size_t i = 0; i < getLength(); i++) {
                const auto ch = peek(i);
                if (ch < 0) return -1;
                if (ch == c) return i;
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
            clear();
            onInit();
            onEmpty();
        }

        uint8_t buffer[Size] = {};
        volatile size_t head = 0; // Index of the next free byte for write
        volatile size_t tail = 0; // Index of the next byte to read
    };
}

#endif //LIBSMART_STM32COMMON_STRINGBUFFER_HPP
