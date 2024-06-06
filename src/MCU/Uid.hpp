/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_MCU_UID_HPP
#define LIBSMART_STM32COMMON_MCU_UID_HPP

#include <main.h>
#include "../Hash/MurmurHash3.hpp"

namespace Stm32Common::MCU {
    /**
     * @class Uid
     * @brief Class for retrieving device identifiers.
     */
    class Uid {
    public:
        using uidArray = uint32_t[3];


        /**
         * @brief Returns a 32-bit unique identifier (UID) of the device.
         *
         * This function retrieves the UID from hardware registers and calculates a hash value using the MurmurHash3 algorithm.
         * The hash value is stored in a static variable to ensure that it is calculated only once and cached for subsequent calls.
         *
         * As an example, this could be used to create a somewhat unique mac address:
         * @code
         * // Core/Src/eth.c
         * // USER CODE BEGIN MACADDRESS
         * MACAddr[2] = (Stm32Common_MCU_Uid_getUid32() >> 24) & 0xFF;
         * MACAddr[3] = (Stm32Common_MCU_Uid_getUid32() >> 16) & 0xFF;
         * MACAddr[4] = (Stm32Common_MCU_Uid_getUid32() >> 8) & 0xFF;
         * MACAddr[5] = (Stm32Common_MCU_Uid_getUid32() >> 0) & 0xFF;
         * // USER CODE END MACADDRESS
         * @endcode
         *
         * @return The 32-bit UID of the device.
         */
        static uint32_t getUid32() {
            static uint32_t uid32 = 0;
            if (uid32 == 0) {
                uid32 = Hash::MurmurHash3::murmur3_32(
                    reinterpret_cast<uint8_t *>(getUid()),
                    sizeof(uidArray),
                    'E' + 'Z');
            }
            return uid32;
        }


        /**
         * @brief Returns the unique identifier (UID) of the device.
         *
         * The UID is a 96-bit value that is retrieved from hardware registers.
         *
         * @return Reference to the array of three uint32_t values representing the UID.
         */
        static uidArray &getUid() {
            static uidArray uid = {};

            if (uid[0] == 0) {
                uid[0] = HAL_GetUIDw0();
                uid[1] = HAL_GetUIDw1();
                uid[2] = HAL_GetUIDw2();
            }
            return uid;
        }


        /**
         * @brief Returns the revision ID (RevId) of the device.
         *
         * The RevId is a 32-bit value that is retrieved from the HAL_GetREVID() function.
         *
         * @return The revision ID of the device.
         */
        static uint32_t getRevId() {
            return HAL_GetREVID();
        }


        /**
         * @brief Returns the device identifier (DevId) of the device.
         *
         * The DevId is a 32-bit value that is retrieved from the HAL_GetDEVID() function.
         *
         * @return The device identifier of the device.
         */
        static uint32_t getDevId() {
            return HAL_GetDEVID();
        }


        /**
         * @brief Returns the flash size of the device.
         *
         * The flash size is retrieved from the hardware registers and then multiplied by 1024 to convert it from kilobytes to bytes.
         *
         * @return Size of the flash in bytes.
         */
        static size_t getFlashSize() {
            return (*reinterpret_cast<const uint16_t *>(FLASHSIZE_BASE)) * 1024;
        }
    };
}

#endif
