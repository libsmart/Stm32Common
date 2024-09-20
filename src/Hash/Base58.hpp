/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 *
 * This file is part of libsmart/Stm32Common, which is distributed under the terms
 * of the BSD 3-Clause License. You should have received a copy of the BSD 3-Clause
 * License along with libsmart/Stm32Common. If not, see <https://spdx.org/licenses/BSD-3-Clause.html>.
 *
 * ----------------------------------------------------------------------------
 * Portions of the code are derived from Luke Dashjr's and Huang Le's work,
 * which is licensed under the MIT License.
 * You can find the original work at:
 * <https://github.com/luke-jr/libbase58>
 * ----------------------------------------------------------------------------
 */

#ifndef LIBSMART_STM32COMMON_HASH_BASE58_HPP
#define LIBSMART_STM32COMMON_HASH_BASE58_HPP

#include <cstdint>
#include <cstring>

namespace Stm32Common::Hash {
    class Base58 {
    public:
        /**
         * Encodes binary data into a Base58-encoded string.
         *
         * @param b58 The buffer where the Base58-encoded string will be written.
         * @param b58sz The size of the buffer `b58` on input and the size of the written string on output.
         * @param data The binary data to encode.
         * @param binsz The size of the binary data.
         * @return True if encoding was successful, false otherwise (e.g., if the buffer size is insufficient).
         */
        static bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz) {
            const auto bin = static_cast<const uint8_t *>(data);
            int carry;
            size_t i, j, high, zcount = 0;
            size_t size;

            while (zcount < binsz && !bin[zcount])
                ++zcount;

            size = (binsz - zcount) * 138 / 100 + 1;
            uint8_t buf[size];
            memset(buf, 0, size);

            for (i = zcount, high = size - 1; i < binsz; ++i, high = j) {
                for (carry = bin[i], j = size - 1; (j > high) || carry; --j) {
                    carry += 256 * buf[j];
                    buf[j] = carry % 58;
                    carry /= 58;
                    if (!j) {
                        // Otherwise j wraps to maxint which is > high
                        break;
                    }
                }
            }

            for (j = 0; j < size && !buf[j]; ++j);

            if (*b58sz <= zcount + size - j) {
                *b58sz = zcount + size - j + 1;
                return false;
            }

            if (zcount)
                memset(b58, '1', zcount);
            for (i = zcount; j < size; ++i, ++j)
                b58[i] = b58digits_ordered[buf[j]];
            b58[i] = '\0';
            *b58sz = i + 1;

            return true;
        }

        /**
         * Decodes a Base58-encoded string into binary data.
         *
         * @param bin The buffer where the binary data will be written.
         * @param binszp On input, points to the size of the `bin` buffer. On output, it points to the size of the decoded binary data.
         * @param b58 The Base58-encoded string to decode.
         * @param b58sz The size of the Base58-encoded string. If zero, the function will calculate the length using `strlen`.
         * @return True if decoding was successful, false otherwise (e.g., if the input string contains invalid characters or the `bin` buffer is too small).
         */
        static bool b58tobin(void *bin, size_t *binszp, const char *b58, size_t b58sz) {
            size_t binsz = *binszp;
            const auto *b58u = reinterpret_cast<const unsigned char *>(b58);
            auto binu = static_cast<unsigned char *>(bin);
            size_t outisz = (binsz + sizeof(b58_almostmaxint_t) - 1) / sizeof(b58_almostmaxint_t);
            b58_almostmaxint_t outi[outisz];
            b58_maxint_t t;
            b58_almostmaxint_t c;
            size_t i, j;
            uint8_t bytesleft = binsz % sizeof(b58_almostmaxint_t);
            b58_almostmaxint_t zeromask = bytesleft ? (b58_almostmaxint_mask << (bytesleft * 8)) : 0;
            unsigned zerocount = 0;

            if (!b58sz)
                b58sz = strlen(b58);

            for (i = 0; i < outisz; ++i) {
                outi[i] = 0;
            }

            // Leading zeros, just count
            for (i = 0; i < b58sz && b58u[i] == '1'; ++i)
                ++zerocount;

            for (; i < b58sz; ++i) {
                if (b58u[i] & 0x80)
                    // High-bit set on invalid digit
                    return false;
                if (b58digits_map[b58u[i]] == -1)
                    // Invalid base58 digit
                    return false;
                c = static_cast<unsigned>(b58digits_map[b58u[i]]);
                for (j = outisz; j--;) {
                    t = static_cast<b58_maxint_t>(outi[j]) * 58 + c;
                    c = t >> b58_almostmaxint_bits;
                    outi[j] = t & b58_almostmaxint_mask;
                }
                if (c)
                    // Output number too big (carry to the next int32)
                    return false;
                if (outi[0] & zeromask)
                    // Output number too big (last int32 filled too far)
                    return false;
            }

            j = 0;
            if (bytesleft) {
                for (i = bytesleft; i > 0; --i) {
                    *(binu++) = (outi[0] >> (8 * (i - 1))) & 0xff;
                }
                ++j;
            }

            for (; j < outisz; ++j) {
                for (i = sizeof(*outi); i > 0; --i) {
                    *(binu++) = (outi[j] >> (8 * (i - 1))) & 0xff;
                }
            }

            // Count canonical base58 byte count
            // binu = bin;
            binu = static_cast<unsigned char *>(bin);
            for (i = 0; i < binsz; ++i) {
                if (binu[i])
                    break;
                --*binszp;
            }
            *binszp += zerocount;

            return true;
        }

    private:
        typedef uint32_t b58_almostmaxint_t;
        typedef uint64_t b58_maxint_t;
        static constexpr size_t b58_almostmaxint_bits = (sizeof(b58_almostmaxint_t) * 8);
        static constexpr b58_almostmaxint_t b58_almostmaxint_mask = (
            (static_cast<b58_maxint_t>(1) << b58_almostmaxint_bits) - 1);

        static constexpr int8_t b58digits_map[] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1, -1, -1, -1,
            -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
            22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
            -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
            47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
        };

        static constexpr char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    };
}

#endif
