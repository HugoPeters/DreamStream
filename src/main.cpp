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

#include <cstdio>
#include "Utils.h"
#include "DeviceManager.h"

int main(int argc, char* argv[])
{
    Logger::SetMinSeverity(LogSeverity::DEBUG);

    LOG_INFO("DreamStream : Boot");

    DeviceManager& dvmgr = DeviceManager::GetInstance();

    if (!dvmgr.Init("255.255.255.255"))
        return -1;

    dvmgr.CreateEmulatedDevice();
    dvmgr.BroadcastDiscovery();

    LOG_INFO("DreamStream : Start listening for incoming packets...");

    bool isRunning = true;

    while (isRunning)
    {
        Utils::Sleep(2);
        dvmgr.Update();
    }

    return 0;
}