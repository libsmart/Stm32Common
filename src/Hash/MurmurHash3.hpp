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

#pragma once

#include <cstdint>
#include <cstring>

#include "String/FixedString.hpp"

namespace Stm32Common::Hash {
    class MurmurHash3 {
        /**
         * Performs a left rotation on a 32-bit unsigned integer by a specified number of bits.
         *
         * @param x The 32-bit unsigned integer to be rotated.
         * @param r The number of bits to rotate the integer to the left. Should be non-negative
         *          and less than or equal to 32.
         * @return The result of left rotating the input integer by the specified number of bits.
         */
        static constexpr uint32_t rotl32(uint32_t x, int r) {
            return (x << r) | (x >> (32 - r));
        }

        /**
         * Finalizes the 32-bit hash value by applying a series of bitwise operations and multiplications.
         * Ensures the generated hash satisfies the avalanche effect for uniform distribution.
         *
         * @param h The 32-bit hash value to be finalized.
         * @return The finalized 32-bit hash value after applying the transformations.
         */
        static constexpr uint32_t fmix32(uint32_t h) {
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            h ^= h >> 16;
            return h;
        }

        /**
         * Extracts a 32-bit block from a character array, starting at a specified index.
         * Combines 4 consecutive bytes into a single 32-bit unsigned integer, with the
         * least significant byte being taken from the input starting index.
         *
         * @param key A pointer to the character array from which to extract the block.
         * @param i The starting index in the character array from which the 32-bit block is extracted.
         * @return A 32-bit unsigned integer representing the combined value of 4 bytes starting
         *         from the specified index in the array.
         */
        static constexpr uint32_t get_block(const char *key, size_t i) {
            return static_cast<uint8_t>(key[i])
                   | (static_cast<uint8_t>(key[i + 1]) << 8)
                   | (static_cast<uint8_t>(key[i + 2]) << 16)
                   | (static_cast<uint8_t>(key[i + 3]) << 24);
        }

        /**
         * Processes the main body of the MurmurHash3 algorithm, applying iterative transformations
         * to compute a 32-bit hash value for chunks of the input key.
         *
         * @param key A pointer to the character array to be hashed, representing the input key.
         * @param len The length of the input key in bytes.
         * @param h1 The initial hash value (seed), which is updated during the processing.
         * @param i The index at which to start processing the key within the character array.
         *          Defaults to 0 and increments as chunks are processed iteratively.
         * @return The 32-bit hash value after processing the key up to the specified length.
         */
        static constexpr uint32_t murmur3_body(const char *key, size_t len, uint32_t h1, size_t i = 0) {
            const uint32_t c1 = 0xcc9e2d51;
            const uint32_t c2 = 0x1b873593;

            return i + 4 <= len
                       ? murmur3_body(key, len,
                                      rotl32(h1 ^ ([&] {
                                          uint32_t k1 = get_block(key, i);
                                          k1 *= c1;
                                          k1 = rotl32(k1, 15);
                                          k1 *= c2;
                                          return k1;
                                      })(), 13) * 5 + 0xe6546b64,
                                      i + 4)
                       : h1;
        }

        /**
         * Processes the remaining bytes of the input key that do not fit into a full 4-byte block.
         * Combines the remaining bytes into a partial 32-bit hash value and applies the necessary
         * transformations to ensure consistency with the MurmurHash3 algorithm.
         *
         * @param key A pointer to the character array representing the input key.
         * @param len The length of the input key in bytes.
         * @param tail_index The index in the character array where the tail section begins.
         *                   Calculated as (length of the key & ~3) to align to the nearest 4-byte boundary.
         * @return A 32-bit unsigned integer derived from the remaining bytes of the input key.
         */
        static constexpr uint32_t murmur3_tail(const char *key, size_t len, size_t tail_index) {
            uint32_t k1 = 0;
            switch (len & 3) {
                case 3: k1 ^= static_cast<uint8_t>(key[tail_index + 2]) << 16;
                case 2: k1 ^= static_cast<uint8_t>(key[tail_index + 1]) << 8;
                case 1: k1 ^= static_cast<uint8_t>(key[tail_index]);
            }
            if (k1) {
                k1 *= 0xcc9e2d51;
                k1 = rotl32(k1, 15);
                k1 *= 0x1b873593;
            }
            return k1;
        }

    public:
        /**
         * Computes a 32-bit MurmurHash3 hash value for a given string literal and seed value.
         * This implementation operates on a fixed-size array, using compile-time constants.
         *
         * @param key A reference to a constant character array representing the string to be hashed.
         *            The key is null-terminated, and its size is determined at compile time.
         * @param seed A 32-bit unsigned integer seed value used as the initial hash.
         * @return The 32-bit hash value computed using the MurmurHash3 algorithm for the input string.
         */
        template<size_t N>
        static constexpr uint32_t murmur3_32(const char (&key)[N], const uint32_t seed) {
            constexpr size_t len = N - 1;
            uint32_t h1 = murmur3_body(key, len, seed);
            h1 ^= murmur3_tail(key, len, len & ~3);
            h1 ^= len;
            return fmix32(h1);
        }

        /**
         * Computes the MurmurHash3 32-bit hash for the given input key and seed.
         *
         * @param key The input key to be hashed, represented as a pointer to a character array.
         *            If key is nullptr, the function returns 0.
         * @param len The length of the key in bytes.
         * @param seed The initial seed value for the hash computation, providing variability in hash results.
         * @return The computed 32-bit hash value of the input key.
         */
        static uint32_t murmur3_32(const char *key, const size_t len, const uint32_t seed) {
            if (key == nullptr) return 0;
            uint32_t h1 = murmur3_body(key, len, seed);
            h1 ^= murmur3_tail(key, len, len & ~3);
            h1 ^= len;
            return fmix32(h1);
        }

        /**
         * Computes the MurmurHash3 32-bit hash for the given input key and seed.
         *
         * @param key A pointer to the key (byte array) to be hashed.
         * @param len The length of the key in bytes.
         * @param seed An initialization seed value to randomize the hash output.
         * @return The 32-bit hash value computed for the given input key.
         */
        static uint32_t murmur3_32(const uint8_t *key, const size_t len, const uint32_t seed) {
            return murmur3_32(reinterpret_cast<const char *>(key), len, seed);
        }

        /**
         * Computes a 32-bit hash value using the MurmurHash3 algorithm.
         *
         * @param key The input key as an array of 8-bit unsigned integers to be hashed.
         * @param len The length of the input key in bytes.
         * @param seed A 32-bit unsigned integer seed value used for initializing the hash.
         * @return A 32-bit hash value calculated from the input key and seed.
         */
        static uint32_t murmur3_32(uint8_t *key, const size_t len, const uint32_t seed) {
            return murmur3_32(reinterpret_cast<const char *>(key), len, seed);
        }

        /**
         * Computes a 32-bit hash value using the MurmurHash3 algorithm with the given key, length, and seed.
         *
         * @param key A pointer to the input key data to be hashed.
         * @param len The length of the input key data in bytes.
         * @param seed A 32-bit seed value to initialize the hash computation.
         * @return The 32-bit hash value computed from the input key and seed.
         */
        static uint32_t murmur3_32(char *key, const size_t len, const uint32_t seed) {
            return murmur3_32(reinterpret_cast<const char *>(key), len, seed);
        }

        /**
         * Computes a 32-bit MurmurHash3 hash for a given fixed-length string key and seed.
         *
         * @param key The fixed-length immutable string to hash.
         * @param seed The seed value to initialize the hash computation, providing randomization.
         * @return The computed 32-bit hash value.
         */
        template<size_t N>
        static uint32_t murmur3_32(String::FixedString<N> &key, const uint32_t seed) {
            return murmur3_32(key.c_str(), key.size(), seed);
        }
    };
}
