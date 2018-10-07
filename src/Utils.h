/********************************************************************************
* Copyright (c) 2018 by Hugo Peters. http://hugo.fyi/                           *
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
* THE SOFTWARE.                                                                 *
*********************************************************************************/

#ifndef _Utils_H_
#define _Utils_H_

#include "Defines.h"
#include <stdarg.h>
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Small helper for inline snprintf using stdarg
template<int TBuffSize>
struct Strfmt
{
    Strfmt()
    {
        memset(m_buffer, 0, sizeof(char) * TBuffSize);
    }

    Strfmt(const char* fmt, ...)
        : Strfmt()
    {
        va_list vlist;
        va_start(vlist, fmt);
        vsnprintf(m_buffer, TBuffSize, fmt, vlist);
        va_end(vlist);
    }

    operator const char*() const
    {
        return GetBuffer();
    }

    const char* GetBuffer() const
    {
        return static_cast<const char*>(m_buffer);
    }

private:
    char m_buffer[TBuffSize + 1];
};

// Logging macros
#if (!defined(LOG_BUFF_SIZE) || LOG_BUFF_SIZE == 0)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_DEBUG(...)
#define LOG_ERROR(...)
#define LOG(...)
#else
#define ENABLE_LOGGING
#define LOG_INFO(msg, ...) Logger::Log(LogSeverity::INFO, Strfmt<LOG_BUFF_SIZE>(msg, ## __VA_ARGS__))
#define LOG_WARN(msg, ...) Logger::Log(LogSeverity::WARN, Strfmt<LOG_BUFF_SIZE>(msg, ## __VA_ARGS__))
#define LOG_DEBUG(msg, ...) Logger::Log(LogSeverity::DEBUG, Strfmt<LOG_BUFF_SIZE>(msg, ## __VA_ARGS__))
#define LOG_ERROR(msg, ...) Logger::Log(LogSeverity::ERROR, Strfmt<LOG_BUFF_SIZE>(msg, ## __VA_ARGS__))
#define LOG(severity, msg, ...) Logger::Log(severity, Strfmt<LOG_BUFF_SIZE>(msg, ## __VA_ARGS__))
#endif

namespace LogSeverity
{
    enum Type
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        NUM
    };

#ifdef ENABLE_LOGGING
    static const char* Names[NUM] =
    {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
    };
#endif
}

class Logger
{
public:
    static void Log(LogSeverity::Type logSeverity, const char* msg);
    static void SetMinSeverity(LogSeverity::Type newMinSeverity) { m_severity = newMinSeverity; }

private:
    Logger() {}

    static int m_severity;
};

struct HandleState
{
    HandleState()
        : m_num_handled(0)
        , m_num_unhandled(0)
    {}

    void SetHandled() { ++m_num_handled; }
    void SetUnhandled() { ++m_num_unhandled; }

    bool IsFullyHandled() { return m_num_handled > 0 && m_num_unhandled == 0; }
    bool IsFullyUnhandled() { return m_num_handled == 0 && m_num_unhandled >= 0; }
    bool IsPartiallyUnhandled() { return (m_num_handled > 0 && m_num_unhandled > 0) || IsFullyUnhandled(); }

    int GetNumHandled() const { return m_num_handled; }
    int GetNumUnhandled() const { return m_num_unhandled; }

private:
    int m_num_handled;
    int m_num_unhandled;
};

namespace Utils
{
    uint8_t CalculateCRC8(const uint8_t* data, int dataSize);
    void Sleep(int sleepMs);
    void DumpBytes(const uint8_t* data, int32_t dataSize, bool asWarn = false);
}

#endif // _Utils_H_