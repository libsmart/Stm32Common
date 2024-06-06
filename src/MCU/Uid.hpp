/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_MCU_UID_HPP
#define LIBSMART_STM32COMMON_MCU_UID_HPP

#include <main.h>
#include "../Hash/MurmurHash3.hpp"

namespace Stm32Common::MCU {
    class Uid {
    public:
        using uidArray = uint32_t[3];
        using dataStruct = struct {
            uidArray uid;
            // uint32_t RevId;
            // uint32_t DevId;
        };


        static uint32_t getUid32() {
            static dataStruct data = {};
            static uint32_t uid32 = 0;
            if(uid32 == 0) {
                data.uid[0] = getUid()[0];
                data.uid[1] = getUid()[1];
                data.uid[2] = getUid()[2];
                // data.RevId = getRevId();
                // data.DevId = getDevId();
                uid32 = Hash::MurmurHash3::murmur3_32(reinterpret_cast<uint8_t *>(&data), sizeof data, 'E' + 'Z');
            }
            return uid32;
        }

        static uidArray &getUid() {
            static uidArray uid = {};

            if (uid[0] == 0) {
                uid[0] = HAL_GetUIDw0();
                uid[1] = HAL_GetUIDw1();
                uid[2] = HAL_GetUIDw2();
            }
            return uid;
        }

        static uint32_t getRevId() {
            return HAL_GetREVID();
        }

        static uint32_t getDevId() {
            return HAL_GetDEVID();
        }

        static size_t getFlashSize() {
            return (*reinterpret_cast<const uint16_t *>(FLASHSIZE_BASE)) * 1024;
        }
    };
}

#endif
