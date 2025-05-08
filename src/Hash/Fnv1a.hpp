/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 *
 * This file is part of libsmart/Stm32Common, which is distributed under the terms
 * of the BSD 3-Clause License. You should have received a copy of the BSD 3-Clause
 * License along with libsmart/Stm32Common. If not, see <https://spdx.org/licenses/BSD-3-Clause.html>.
 *
* ----------------------------------------------------------------------------
 * The FNV1a hash algorithm is based on work by Glenn Fowler, Landon Curt Noll,
 * and Phong Vo. For more information, see:
 * <http://www.isthe.com/chongo/tech/comp/fnv/>
 *
 * The FNV hash algorithms and source code have been released into the public domain.
 * The authors of the FNV algorithm took deliberate steps to disclose the algorithm
 * in a public forum long before the first known patent on the FNV algorithm was
 * filed. For more details, see: <http://www.isthe.com/chongo/tech/comp/fnv/index.html#public_domain>
 * ----------------------------------------------------------------------------
 */

#pragma once

#include <cstdint>

namespace Stm32Common::Hash::FNV1a {

    // 32-bit FNV-1a constants
    constexpr uint32_t FNV_OFFSET_BASIS = 2166136261u;
    constexpr uint32_t FNV_PRIME = 16777619u;

    using fnv1a32_t = uint32_t;

    /**
     * Calculates a 32-bit FNV-1a hash for a given null-terminated string.
     *
     * The FNV-1a hashing algorithm is a simple, non-cryptographic hash function widely used for
     * fast, lightweight hashing. The function processes each character of the input string, applying
     * the FNV-1a algorithm using the FNV offset basis and prime constant.
     *
     * The resulting hash value is deterministic, meaning that the same input string will always produce
     * the same hash value.
     *
     * @param str A pointer to a null-terminated string to be hashed.
     * @return The 32-bit hash value corresponding to the input string.
     */
    constexpr fnv1a32_t hash(const char* str) {
        uint32_t hash = FNV_OFFSET_BASIS;
        while (*str) {
            hash ^= static_cast<uint8_t>(*str++);
            hash *= FNV_PRIME;
        }
        return hash;
    }

    /**
     * Calculates a 32-bit FNV-1a hash for a given string with a specified length.
     *
     * This method applies the FNV-1a hashing algorithm, which is lightweight and commonly used
     * for non-cryptographic hashing purposes. The algorithm processes each character of the input,
     * combining it with the previously accumulated hash using the FNV offset basis and prime constant.
     *
     * @param str A pointer to the input string to be hashed.
     * @param length The length of the input string.
     * @return The 32-bit hash value corresponding to the input string and its length.
     */
    constexpr fnv1a32_t hash(const char* str, size_t length) {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; i < length; ++i) {
            hash ^= static_cast<uint8_t>(str[i]);
            hash *= FNV_PRIME;
        }
        return hash;
    }

}
