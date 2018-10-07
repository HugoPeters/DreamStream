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

#ifndef _StaticString_H_
#define _StaticString_H_

#include <cstdint>
#include <string.h>

template<uint32_t TBuffSize>
struct StaticString
{
public:
    StaticString()
    {
        Clear();
    }
    StaticString(const char* other)
    {
        Set(other);
    }

    const char* Get() const { return GetBuffer(); }

    bool operator==(const char* other)
    {
        return strcmp(m_buffer, other) == 0;
    }

    int Strcmp(const char* other)
    {
        return strcmp(m_buffer, other);
    }

    void operator=(const char* other)
    {
        Set(other);
    }

    void Set(const char* str)
    {
        Clear();

        if (!str)
            return;

        size_t otherStrLen = strlen(str);
        size_t minSize = otherStrLen < TBuffSize ? otherStrLen : TBuffSize;
        strncpy(m_buffer, str, minSize);
    }

    void Clear()
    {
        memset(m_buffer, 0, sizeof(m_buffer));
    }

    operator const char*() const
    {
        return GetBuffer();
    }

    const char* GetBuffer() const
    {
        return static_cast<const char*>(m_buffer);
    }

    size_t Length() const
    {
        return strlen(m_buffer);
    }

    uint32_t BufferLength() const { return TBuffSize; }

    void CopyFrom(const uint8_t* ptr, size_t count)
    {
        Clear();
        size_t minSize = count < TBuffSize ? count : TBuffSize;
        memcpy(m_buffer, ptr, minSize);
    }

private:
    char m_buffer[TBuffSize + 1];
};

#endif // _StaticString_H_