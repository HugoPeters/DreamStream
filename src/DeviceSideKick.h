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

#ifndef _DeviceSideKick_H_
#define _DeviceSideKick_H_

#include "Device.h"

class DeviceSideKick : public Device
{
public:
    typedef Device Super;

    DeviceSideKick(DeviceManager* manager, const char* ip);
    virtual ~DeviceSideKick();

    void SetStateFromPayload(const uint8_t* payload, int32_t payloadLength) override;
    int32_t SetStateToPayload(uint8_t* payload) override;

    virtual void HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt) override;

protected:

    uint8_t m_sector_assignment[15];
    uint8_t m_esp_firmware_version[2];
};

#endif // _DeviceSideKick_H_