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

#ifndef _DeviceEmulator_H_
#define _DeviceEmulator_H_

#include "DeviceSideKick.h"
#include "Timer.h"

class DeviceManager;

/* Base class for all devices, both hardware (hdw) or emulated (emu) */
class DeviceSideKickEmu : public DeviceSideKick
{
public:
    typedef DeviceSideKick Super;

    DeviceSideKickEmu(DeviceManager* manager);
    ~DeviceSideKickEmu();

    void Update() override;

    // State setters
    //bool SetAmbientColor(uint8_t r, uint8_t g, uint8_t b) override { return false; }

    virtual void HandleChangeBrightness();

    void HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt) override;

private:
    void HandlePacket_Read_CurrentState(const UDPMessageInfo& msg, const PacketInfo& pkt);
    void HandlePacket_Read_SubscribeToSectorData(const UDPMessageInfo& msg, const PacketInfo& pkt);
    void HandlePacket_Response_SectorData(const UDPMessageInfo& msg, const PacketInfo& pkt);

    bool m_should_subscribe;
    bool m_has_active_sector_subscription;

    Timer m_sector_subscription_timeout;
};

#endif // _DeviceEmulator_H_