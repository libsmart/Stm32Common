/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMRXTX_HPP
#define LIBSMART_STM32COMMON_STREAMRXTX_HPP

#include <climits>
#include "Stream.hpp"
#include "StreamRxTxInterface.hpp"
#include "StringBuffer.hpp"

namespace Stm32Common {
    template<buf_size_t bufferSizeRx, buf_size_t bufferSizeTx>
    class StreamRxTx : virtual public StreamRxTxInterface {
    public:
        StreamRxTx() = default;

#ifdef LIBSMART_ENABLE_DIRECT_BUFFER_WRITE
        /**
         * @brief Retrieves a write buffer for transmitting data.
         *
         * This method returns a pointer to the write buffer of the txBuffer object and the remaining space in the buffer.
         *
         * @param buffer A reference to a pointer that will be set to the write buffer.
         * @return The remaining space in the write buffer.
         */
        size_t getWriteBuffer(uint8_t *&buffer) override {
            buffer = txBuffer.getWritePointer();
            return txBuffer.getRemainingSpace();
        }

        /**
         * @brief Sets the number of bytes that have been written to the transmit buffer.
         *
         * This method updates the number of bytes that have been written to the transmit buffer with the given size.
         *
         * @param size The number of bytes that have been written.
         * @return The new total number of written bytes in the transmit buffer.
         */
        size_t setWrittenBytes(size_t size) override {
            return txBuffer.add(size);
        }
#endif

        /**
         * @brief Writes a single byte of data to the transmit buffer.
         *
         * This method writes a single byte of data to the transmit buffer and returns the number of bytes written.
         *
         * @param data The byte of data to be written.
         * @return The number of bytes written. Returns 1 if data is successfully written to the transmit buffer, otherwise returns 0.
         */
        size_t write(uint8_t data) override {
            if (txBuffer.write(data) == 1) {
                return 1;
            }
            return 0;
        }

        using Stream::write;

        /**
         * @brief Returns the number of bytes available for writing to the transmit buffer.
         *
         * This method calculates and returns the remaining space in the transmit buffer of the StreamRxTx object.
         *
         * @return The number of bytes available for writing to the transmit buffer. Returns INT_MAX if the remaining space exceeds INT_MAX, otherwise returns the remaining space as int.
         */
        int availableForWrite() override {
            return std::min(txBuffer.getRemainingSpace(), static_cast<buf_size_t>(INT_MAX));
        }

        /**
         * @brief Flushes the transmit buffer.
         *
         * This method flushes the transmit buffer, ensuring all data in the buffer is written to the output stream.
         * It is an override of the flush() method from the Stream base class.
         */
        void flush() override {
        }

        /**
         * @brief Returns the number of bytes available for reading from the receive buffer.
         *
         * This method calculates and returns the remaining bytes in the receive buffer of the StreamRxTx object.
         *
         * @return The number of bytes available for reading from the receive buffer. Returns INT_MAX if the remaining bytes exceeds INT_MAX, otherwise returns the remaining bytes as int.
         */
        int available() override {
            return std::min(rxBuffer.getLength(), static_cast<buf_size_t>(INT_MAX));
        }

        /**
         * @brief Reads a single byte of data from the receive buffer.
         *
         * This method reads a single byte of data from the receive buffer and returns it as an integer.
         *
         * @return The byte of data read from the receive buffer.
         *         Returns -1 if there is no data available in the receive buffer.
         */
        int read() override {
            return rxBuffer.read();
        }

        /**
         * @brief Returns the next byte of data in the receive buffer without removing it.
         *
         * This method returns the next byte of data in the receive buffer without removing it.
         *
         * @return The next byte of data in the receive buffer as an integer.
         *         Returns -1 if there is no data available in the receive buffer.
         */
        int peek() override {
            return rxBuffer.peek();
        }

        typedef StringBuffer<bufferSizeRx> rxBuffer_t;
        typedef StringBuffer<bufferSizeTx> txBuffer_t;
        StringBufferInterface *getRxBuffer() override { return &rxBuffer; }
        StringBufferInterface *getTxBuffer() override { return &txBuffer; }

    private:
        /**
         * @class rxBufferClass
         * @brief A class that represents a transmit buffer.
         *
         * This class is a final subclass of rxBuffer_t and is used to manage the receive buffer.
         */
        class rxBufferClass final : public rxBuffer_t {
        public:
            rxBufferClass() = delete;

            explicit rxBufferClass(StreamRxTx &streamRxTxInstance)
                : streamRxTxInstance(streamRxTxInstance) { ; }

        protected:
            void onWrite() override {
                StringBuffer<bufferSizeRx>::onWrite();
            }

            StreamRxTx &streamRxTxInstance;
        } rxBuffer{*this};


        /**
         * @class txBufferClass
         * @brief A class that represents a transmit buffer.
         *
         * This class is a final subclass of txBuffer_t and is used to manage the transmit buffer for data transmission.
         */
        class txBufferClass final : public txBuffer_t {
        public:
            txBufferClass() = delete;

            explicit txBufferClass(StreamRxTx &streamRxTxInstance)
                : streamRxTxInstance(streamRxTxInstance) { ; }

        protected:
            void onWrite() override {
                StringBuffer<bufferSizeTx>::onWrite();
            }

            StreamRxTx &streamRxTxInstance;
        } txBuffer{*this};
    };
}
#endif
