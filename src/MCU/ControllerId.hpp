/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include "Stm32ItmLogger.hpp"
#include "Hash/Base58.hpp"
#include "MCU/Uid.hpp"

namespace Stm32Common {
    class ControllerId {
    public:
        static const char *getControllerId() {
            static ctrlId_t ctrlId{};
            if(std::strlen(ctrlId) == 0) {
                updateControllerId(ctrlId);
            }
            return ctrlId;
        }

    private:
        using ctrlId_t = char[20];

        static void updateControllerId(ctrlId_t &ctrlId) {
            auto &uid = Stm32Common::MCU::Uid::getUid();
            auto ctrlIdLen = sizeof(ctrlId) - 1;
            Hash::Base58::b58enc(ctrlId, &ctrlIdLen, &uid, sizeof(uid));
            if (ctrlIdLen > sizeof(ctrlId) - 1) {
                std::memset(ctrlId, 0, sizeof(ctrlId));
            }
        }
    };

}
