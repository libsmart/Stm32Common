/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_RINGBUFFER_HPP
#define LIBSMART_STM32COMMON_RINGBUFFER_HPP

#include <libsmart_config.hpp>
#include <array>
#include <cstddef>

namespace Stm32Common {
    template<typename T, size_t Size>
    class RingBuffer {
    private:
        std::array<T, Size> buffer{};
        size_t head; // Index des nächsten freien Elements
        size_t tail; // Index des nächsten zu lesenden Elements
        size_t count; // Anzahl der Elemente im Puffer

    public:
        RingBuffer() : head(0), tail(0), count(0) {}

        [[nodiscard]] bool isEmpty() const {
            return count == 0;
        }

        [[nodiscard]] bool isFull() const {
            return count == Size;
        }

        bool enqueue(T data) {
            if (isFull()) {
                return false;
            }
            buffer[head] = data;
            head = (head + 1) % Size;
            count++;
            return true;
        }

        bool dequeue(T &data) {
            if (isEmpty()) {
                return false;
            }
            data = buffer[tail];
            tail = (tail + 1) % Size;
            count--;
            return true;
        }
    };
}

#endif //LIBSMART_STM32COMMON_RINGBUFFER_HPP
