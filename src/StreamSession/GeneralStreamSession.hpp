/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32COMMON_STREAMSESSION_GENERALSTREAMSESSION_HPP
#define LIBSMART_STM32COMMON_STREAMSESSION_GENERALSTREAMSESSION_HPP

#include "Helper.hpp"
#include "StreamRxTx.hpp"
#include "StreamSessionInterface.hpp"

#define LIBSMART_STM32SHELL_GENERALSTREAMSESSION_BUFFER_SIZE_RX 128
#define LIBSMART_STM32SHELL_GENERALSTREAMSESSION_BUFFER_SIZE_TX 128

namespace Stm32Common::StreamSession {
    class GeneralStreamSession : public StreamSessionInterface,
                              public StreamRxTx<
                                  LIBSMART_STM32SHELL_GENERALSTREAMSESSION_BUFFER_SIZE_RX,
                                  LIBSMART_STM32SHELL_GENERALSTREAMSESSION_BUFFER_SIZE_TX> {
    public:
        GeneralStreamSession() {
            Nameable::setName("GeneralStreamSession");
        }

        void setup() override { ; }

        void loop() override {
//            while (available() > 0) {
//                getTxBuffer()->write(getRxBuffer()->read());
//            }

            if (getTxBuffer()->available() > 0) {
                sessionOwner->dataReadyTx(this);
            }
        }

        void end() override {
            getRxBuffer()->clear();
            getTxBuffer()->clear();
        }

        void errorHandler() override { ; }

        void flush() override {
            StreamRxTx::flush();
        }

    protected:
        void onWriteTx() override {
            StreamRxTx::onWriteTx();
            loop();
        }
    };
}
#endif
