/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <array>
#include <cstdint>
#include <cstddef>

namespace Stm32Common::Hash {
    namespace Crc32Table {
        constexpr std::array<uint32_t, 256> generate_table() {
            std::array<uint32_t, 256> tbl{};
            for (uint32_t i = 0; i < 256; ++i) {
                uint32_t c = i;
                for (int j = 0; j < 8; ++j)
                    c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
                tbl[i] = c;
            }
            return tbl;
        }

        constexpr std::array<uint32_t, 256> table = generate_table();
    }

    class Crc32 {
    public:
        constexpr Crc32() = default;

        static constexpr uint32_t compute(const uint8_t* data, size_t const length) {
            uint32_t crc = 0xFFFFFFFFu;
            for (size_t i = 0; i < length; ++i) {
                crc = (crc >> 8) ^ Crc32Table::table[(crc ^ data[i]) & 0xFF];
            }
            return crc ^ 0xFFFFFFFFu;
        }
    };
}
