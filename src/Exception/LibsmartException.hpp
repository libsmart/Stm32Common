/*
 * SPDX-FileCopyrightText: 2025 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

namespace Stm32Common {
#if __EXCEPTIONS
#include <stdexcept>
#define LIBSMART_EXCEPTION(fn, res, exp)                                 \
do {                                                                    \
res.COMPONENT_NAME = COMPONENT_NAME;                                    \
res.CLASS_NAME = CLASS_NAME;                                            \
res.INSTANCE_NAME = getName();                                          \
res.FUNCTION_NAME = fn;                                                 \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(res);    \
throw exp(res);                                                         \
} while (0);


    class LibsmartException : public std::runtime_error, public Stm32Common::Printable {
    public:
        LibsmartException(const char *message)
            : LibsmartException("", message) { ; }

        LibsmartException(const char *COMPONENT_NAME, const char *message)
            : LibsmartException(COMPONENT_NAME, "", message) { ; }

        LibsmartException(const char *COMPONENT_NAME, const char *CLASS_NAME, const char *message)
            : LibsmartException(COMPONENT_NAME, CLASS_NAME, "", message) { ; }

        LibsmartException(const char *COMPONENT_NAME, const char *CLASS_NAME, const char *INSTANCE_NAME,
                          const char *message)
            : LibsmartException(COMPONENT_NAME, CLASS_NAME, INSTANCE_NAME, "", message) { ; }

        LibsmartException(const char *COMPONENT_NAME, const char *CLASS_NAME, const char *INSTANCE_NAME,
                          const char *FUNCTION_NAME, const char *message)
            : std::runtime_error(message), COMPONENT_NAME(COMPONENT_NAME), CLASS_NAME(CLASS_NAME),
              INSTANCE_NAME(INSTANCE_NAME), FUNCTION_NAME(FUNCTION_NAME) { ; }

        size_t printTo(Stm32Common::PrintInterface &printObject) const override {
            return printObject.print(what());
        }

    protected:
        const char *COMPONENT_NAME;
        const char *CLASS_NAME;
        const char *INSTANCE_NAME;
        const char *FUNCTION_NAME;
    };

#else
#define LIBSMART_EXCEPTION(fn, res, exp)                                 \
do {                                                                    \
res.COMPONENT_NAME = COMPONENT_NAME;                                    \
res.CLASS_NAME = CLASS_NAME;                                            \
res.INSTANCE_NAME = getName();                                          \
res.FUNCTION_NAME = fn;                                                 \
log(Stm32ItmLogger::LoggerInterface::Severity::ERROR)->println(res);    \
return res;                                                             \
} while (0);
#endif
}
