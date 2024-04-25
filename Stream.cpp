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
 * Portions of the code are derived from David A. Mellis's work,
 * which is licensed under the GNU Lesser General Public License. You can find the original work at:
 * <https://github.com/arduino/ArduinoCore-avr/>
 * ----------------------------------------------------------------------------
 */

#include "Stream.hpp"
#include "Helper.hpp"

using namespace Stm32Common;

int Stream::timedRead() {
    int c;
    _startMillis = millis();
    do {
        c = read();
        if (c >= 0) return c;
    } while (millis() - _startMillis < _timeout);
    return -1;     // -1 indicates timeout
}


int Stream::timedPeek() {
    int c;
    _startMillis = millis();
    do {
        c = peek();
        if (c >= 0) return c;
    } while (millis() - _startMillis < _timeout);
    return -1;     // -1 indicates timeout
}


int Stream::peekNextDigit(LookaheadMode lookahead, bool detectDecimal) {
    int c;
    while (true) {
        c = timedPeek();

        if (c < 0 ||
            c == '-' ||
            (c >= '0' && c <= '9') ||
            (detectDecimal && c == '.'))
            return c;

        switch (lookahead) {
            case SKIP_NONE:
                return -1; // Fail code.
            case SKIP_WHITESPACE:
                switch (c) {
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n':
                        break;
                    default:
                        return -1; // Fail code.
                }
            case SKIP_ALL:
                break;
        }
        read();  // discard non-numeric
    }
}


void Stream::setTimeout(unsigned long timeout) {
    _timeout = timeout;
}


unsigned long Stream::getTimeout() {
    return _timeout;
}


bool Stream::find(char *target) {
    return findUntil(target, strlen(target), nullptr, 0);
}


bool Stream::find(uint8_t *target) {
    return find((char *) target);
}


bool Stream::find(char *target, size_t length) {
    return findUntil(target, length, nullptr, 0);
}


bool Stream::find(uint8_t *target, size_t length) {
    return find((char *) target, length);
}


bool Stream::find(char target) {
    return find(&target, 1);
}


bool Stream::findUntil(char *target, char *terminator) {
    return findUntil(target, strlen(target), terminator, strlen(terminator));
}


bool Stream::findUntil(uint8_t *target, char *terminator) {
    return findUntil((char *) target, terminator);
}


bool Stream::findUntil(char *target, size_t targetLen, char *terminator, size_t termLen) {
    if (terminator == nullptr) {
        MultiTarget t[1] = {{target, targetLen, 0}};
        return findMulti(t, 1) == 0 ? true : false;
    } else {
        MultiTarget t[2] = {{target,     targetLen, 0},
                            {terminator, termLen,   0}};
        return findMulti(t, 2) == 0 ? true : false;
    }
}


bool Stream::findUntil(uint8_t *target, size_t targetLen, char *terminator, size_t termLen) {
    return findUntil((char *) target, targetLen, terminator, termLen);
}


long Stream::parseInt(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    long value = 0;
    int c;

    c = peekNextDigit(lookahead, false);
    // ignore non numeric leading characters
    if (c < 0)
        return 0; // zero returned if timeout

    do {
        if (c == ignore); // ignore this character
        else if (c == '-')
            isNegative = true;
        else if (c >= '0' && c <= '9')        // is c a digit?
            value = value * 10 + c - '0';
        read();  // consume the character we got with peek
        c = timedPeek();
    } while ((c >= '0' && c <= '9') || c == ignore);

    if (isNegative)
        value = -value;
    return value;
}


float Stream::parseFloat(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    bool isFraction = false;
    long value = 0;
    int c;
    float fraction = 1.0;

    c = peekNextDigit(lookahead, true);
    // ignore non numeric leading characters
    if (c < 0)
        return 0; // zero returned if timeout

    do {
        if (c == ignore); // ignore
        else if (c == '-')
            isNegative = true;
        else if (c == '.')
            isFraction = true;
        else if (c >= '0' && c <= '9') {      // is c a digit?
            value = value * 10 + c - '0';
            if (isFraction)
                fraction *= 0.1;
        }
        read();  // consume the character we got with peek
        c = timedPeek();
    } while ((c >= '0' && c <= '9') || (c == '.' && !isFraction) || c == ignore);

    if (isNegative)
        value = -value;
    if (isFraction)
        return value * fraction;
    else
        return value;
}


size_t Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0) break;
        *buffer++ = (char) c;
        count++;
    }
    return count;
}


size_t Stream::readBytes(uint8_t *buffer, size_t length) {
    return readBytes((char *) buffer, length);
}


size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length) {
    size_t index = 0;
    while (index < length) {
        int c = timedRead();
        if (c < 0 || c == terminator) break;
        *buffer++ = (char) c;
        index++;
    }
    return index; // return number of characters, not including null terminator
}


size_t Stream::readBytesUntil(char terminator, uint8_t *buffer, size_t length) {
    return readBytesUntil(terminator, (char *) buffer, length);
}


/*
String Stream::readString()
{
    String ret;
    int c = timedRead();
    while (c >= 0)
    {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}


String Stream::readStringUntil(char terminator)
{
    String ret;
    int c = timedRead();
    while (c >= 0 && c != terminator)
    {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}
*/


long Stream::parseInt(char ignore) {
    return parseInt(SKIP_ALL, ignore);
}


float Stream::parseFloat(char ignore) {
    return parseFloat(SKIP_ALL, ignore);
}


int Stream::findMulti(struct Stream::MultiTarget *targets, int tCount) {
    // any zero length target string automatically matches and would make
    // a mess of the rest of the algorithm.
    for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
        if (t->len <= 0)
            return t - targets;
    }

    while (1) {
        int c = timedRead();
        if (c < 0)
            return -1;

        for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
            // the simple case is if we match, deal with that first.
            if (c == t->str[t->index]) {
                if (++t->index == t->len)
                    return t - targets;
                else
                    continue;
            }

            // if not we need to walk back and see if we could have matched further
            // down the stream (ie '1112' doesn't match the first position in '11112'
            // but it will match the second position so we can't just reset the current
            // index to 0 when we find a mismatch.
            if (t->index == 0)
                continue;

            int origIndex = t->index;
            do {
                --t->index;
                // first check if current char works against the new current index
                if (c != t->str[t->index])
                    continue;

                // if it's the only char then we're good, nothing more to check
                if (t->index == 0) {
                    t->index++;
                    break;
                }

                // otherwise we need to check the rest of the found string
                int diff = origIndex - t->index;
                size_t i;
                for (i = 0; i < t->index; ++i) {
                    if (t->str[i] != t->str[i + diff])
                        break;
                }

                // if we successfully got through the previous loop then our current
                // index is good.
                if (i == t->index) {
                    t->index++;
                    break;
                }

                // otherwise we just try the next index
            } while (t->index);
        }
    }
    // unreachable
    return -1;
}











