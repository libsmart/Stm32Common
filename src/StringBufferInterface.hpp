/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAM_STRINGBUFFERINTERFACE_HPP
#define LIBSMART_STM32COMMON_STREAM_STRINGBUFFERINTERFACE_HPP

#include <libsmart_config.hpp>
#include <cstddef>
#include "Stream.hpp"

#ifdef LIBSMART_ENABLE_STD_FUNCTION
#include <functional>
#include <utility>
#define onInitFunction onInitFn
#define onEmptyFunction onEmptyFn
#define onNonEmptyFunction onNonEmptyFn
#define onWriteFunction onWriteFn
#define onReadFunction onReadFn
#else
#define onInitFunction LIBSMART_NOF
#define onEmptyFunction LIBSMART_NOF
#define onNonEmptyFunction LIBSMART_NOF
#define onWriteFunction LIBSMART_NOF
#define onReadFunction LIBSMART_NOF
#endif

namespace Stm32Common {
    typedef size_t buf_size_t;
    typedef int64_t buf_size_signed_t;

    class StringBufferInterface : public Stream {
    public:
        /**
         * Check if the StringBuffer is empty.
         *
         * This method checks if the StringBuffer is empty by comparing the head index with the tail index.
         *
         * \return True if the StringBuffer is empty, false otherwise.
         */
        virtual bool isEmpty() = 0;

        /**
         * Check if the StringBuffer is full.
         *
         * This method checks if the StringBuffer is full by comparing the head index with the maximum buffer size (Size).
         *
         * \return True if the StringBuffer is full, false otherwise.
         */
        virtual bool isFull() = 0;

        /**
         * Get the remaining space of the StringBuffer.
         *
         * This method returns the number of bytes that are available for writing to the StringBuffer. It is calculated by subtracting the head index from the maximum buffer size (Size).
         *
         * \return The number of bytes available for writing.
         */
        virtual buf_size_t getRemainingSpace() = 0;

        /**
         * Get the length of the StringBuffer.
         *
         * This method returns the number of bytes currently stored in the StringBuffer. It is calculated by subtracting the tail index from the head index.
         *
         * \return The length of the StringBuffer.
         */
        virtual buf_size_t getLength() = 0;

        /**
         * Read data from the StringBuffer.
         *
         * This method reads up to `size` bytes of data from the StringBuffer into the provided output buffer `out`.
         *
         * @param out Pointer to the buffer where the read data will be stored.
         * @param size The maximum number of bytes to read from the StringBuffer.
         * @return The actual number of bytes read from the StringBuffer.
         */
        virtual buf_size_t read(void *out, buf_size_t size) = 0;

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        /**
         * Read data from the StringBuffer and write to the given StringBuffer.
         *
         * @param stringBuffer Pointer to the StringBufferInterface object to which to write data.
         * @return The number of bytes read from the StringBuffer.
         */
        virtual buf_size_t read(StringBufferInterface *stringBuffer) = 0;
#endif

        using Stream::read;

        /**
         * Peek at the byte at the specified position in the buffer.
         *
         * This method allows inspecting the byte stored at a specific position `pos` in the StringBuffer without removing it.
         *
         * @param pos The position in the buffer from which to peek the byte.
         * @return The byte at the specified position.
         */
        virtual int peek(buf_size_t pos) = 0;

        using Stream::peek;

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        /**
         * Get a pointer to the write buffer.
         *
         * This method provides direct access to the write buffer for writing operations.
         *
         * @return A pointer to the write buffer.
         */
        virtual uint8_t *getWritePointer() = 0;
#endif

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_READ
        /**
         * Get a pointer to read from the StringBuffer.
         *
         * Provides access to a constant pointer pointing to the current read position in the StringBuffer.
         *
         * \return A constant pointer to the current read position in the StringBuffer.
         */
        virtual const uint8_t *getReadPointer() = 0;
#endif

        /**
         * Increase the pointer of the buffer after writing directly to the buffer.
         *
         * @param add The buffer size to add.
         * @return The real size added to the buffer.
         */
        virtual buf_size_t add(buf_size_t add) = 0;

        /**
         * Remove a specified number of bytes from the StringBuffer.
         *
         * @param remove The number of bytes to remove.
         * @return The actual number of bytes removed from the StringBuffer.
         */
        virtual buf_size_t remove(buf_size_t remove) = 0;

        /**
         * Clear the StringBuffer by resetting head and tail indices to 0 and
         * clearing the buffer with zero values.
         */
        virtual void clear() = 0;

        /**
         * Find the position of a specified character in the buffer.
         *
         * This pure virtual function searches for the first occurrence of the specified character
         * in the buffer and returns the position as a signed integer.
         *
         * \param c The character to search for in the buffer.
         * \return The position of the character in the buffer, or -1 if the character is not found.
         */
        virtual buf_size_signed_t findPos(uint8_t c) = 0;

        using onInitCb_t = void();
        using onEmptyCb_t = void();
        using onNonEmptyCb_t = void();
        using onWriteCb_t = void();
        using onReadCb_t = void();

#ifdef LIBSMART_ENABLE_STD_FUNCTION
        using onInitFn_t = std::function<onInitCb_t>;
        using onEmptyFn_t = std::function<onEmptyCb_t>;
        using onNonEmptyFn_t = std::function<onNonEmptyCb_t>;
        using onWriteFn_t = std::function<onWriteCb_t>;
        using onReadFn_t = std::function<onReadCb_t>;

        virtual void setOnInitFn(const onInitFn_t &on_init_fn) = 0;

        virtual void setOnEmptyFn(const onEmptyFn_t &on_empty_fn) = 0;

        virtual void setOnNonEmptyFn(const onNonEmptyFn_t &on_non_empty_fn) = 0;

        /**
         * Set the callback function for write operations.
         *
         * This function sets the callback function for write operations. The provided on_write_fn will be called
         * whenever a write operation is performed on the StringBuffer.
         *
         * Note: Take care when using StrinBuffer with interrupts. Some callbacks are called by the isr.
         *
         * \param on_write_fn The callback function for write operations.
         */
        virtual void setOnWriteFn(const onWriteFn_t &on_write_fn) = 0;


        virtual void setOnReadFn(const onReadFn_t &on_read_fn) = 0;
#endif
    };
}

#endif
