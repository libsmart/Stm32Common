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
 * Portions of the code are derived from Austin Appleby's work,
 * which is licensed under the Creative Commons Attribution-ShareAlike License 4.0.
 * You can find the original work at:
 * <https://en.wikipedia.org/wiki/MurmurHash>
 * ----------------------------------------------------------------------------
 */

#ifndef LIBSMART_STM32COMMON_HASH_MURMURHASH3_HPP
#define LIBSMART_STM32COMMON_HASH_MURMURHASH3_HPP

#include <cstdint>
#include <cstring>

namespace Stm32Common::Hash {
    class MurmurHash3 {
    public:
        static uint32_t murmur3_32(const uint8_t *key, size_t len, uint32_t seed) {
            uint32_t h = seed;
            uint32_t k;
            /* Read in groups of 4. */
            for (size_t i = len >> 2; i; i--) {
                // Here is a source of differing results across endiannesses.
                // A swap here has no effects on hash properties though.
                memcpy(&k, key, sizeof(uint32_t));
                key += sizeof(uint32_t);
                h ^= murmur_32_scramble(k);
                h = (h << 13) | (h >> 19);
                h = h * 5 + 0xe6546b64;
            }
            /* Read the rest. */
            k = 0;
            for (size_t i = len & 3; i; i--) {
                k <<= 8;
                k |= key[i - 1];
            }
            // A swap is *not* necessary here because the preceding loop already
            // places the low bytes in the low places according to whatever endianness
            // we use. Swaps only apply when the memory is copied in a chunk.
            h ^= murmur_32_scramble(k);
            /* Finalize. */
            h ^= len;
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            h ^= h >> 16;
            return h;
        }

    private:
        static uint32_t murmur_32_scramble(uint32_t k) {
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            return k;
        }
    };
}

#endif
