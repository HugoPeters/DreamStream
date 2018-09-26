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

#ifndef _Timer_H_
#define _Timer_H_

#include <cstdint>

#if _WIN32
#include <Winsock2.h>
#include <time.h>
#endif

#ifdef __linux__
#include <stdio.h>
#include <sys/time.h>
#endif

struct Timer
{
    Timer()
    {
        m_start_time = GetTotalMSec();
    }

    uint64_t Elapsed()
    {
        return GetTotalMSec() - m_start_time;
    }

    void Reset()
    {
        m_start_time = GetTotalMSec();
    }

private:
#ifdef __linux__
    static uint64_t GetTotalMSec()
    {
        timeval time;
        gettimeofday(&time, NULL);
        return time.tv_sec + (time.tv_usec / 1000000.0);
    }
#elif defined(_WIN32)
    // https://gamedev.stackexchange.com/questions/26759/best-way-to-get-elapsed-time-in-miliseconds-in-windows
    static uint64_t GetTotalMSec()
    {
        static LARGE_INTEGER freq;
        static bool usePrecisionTimer = QueryPerformanceFrequency(&freq);
        if (usePrecisionTimer)
        {
            LARGE_INTEGER timeNow;
            QueryPerformanceCounter(&timeNow);
            return (1000LL * timeNow.QuadPart) / freq.QuadPart;
        }
        else
        {
            return GetTickCount();
        }
    }
#endif

    uint64_t m_start_time;
};

#endif // _Timer_H_