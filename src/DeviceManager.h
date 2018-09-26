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

#ifndef _DeviceManager_H_
#define _DeviceManager_H_

#include "Defines.h"
#include "UDPClient.h"
#include "Device.h"
#include "Commands.h"
#include <vector>

class DeviceSideKickEmu;

struct PacketInfo;
using namespace std;

class DeviceManager
{
public:
    static DeviceManager& GetInstance()
    {
        static DeviceManager inst;
        return inst;
    }

    bool Init(const char* broadcastAddr);
    void Shutdown();
    void Update();

    bool BroadcastDiscovery();
    bool Connect(const char* ip);
    bool TryBindEmulatedDeviceToHost();

    int PreparePacketData(uint8_t group, uint8_t flags, uint8_t cmdUpper, uint8_t cmdLower, uint8_t* payloadData, int32_t payloadSize);

    Device* FindDevice(const char* ipaddr);
    DeviceSideKickEmu* CreateEmulatedDevice();

    bool SendPacket(const char* addr, uint8_t group, uint8_t flag, Commands::Type cmd, uint8_t* payloadData, int32_t payloadSize);

private:
    void HandleDataReceived(const UDPMessage& msg, const UDPMessageInfo& msgInfo);
    void HandlePacket(const PacketInfo& pkt, const UDPMessageInfo& msgInfo);

#ifdef ENABLE_CLIENT_DEVICES
    void HandlePacket_CurrentState_NewDevice(const PacketInfo& pkt, const UDPMessageInfo& msgInfo);
#endif // ENABLE_CLIENT_DEVICES

    DeviceManager();
    ~DeviceManager();

    UDPClient* m_client;
    uint8_t m_packet_cache[PACKET_BUFF_SIZE];

#ifdef ENABLE_CLIENT_DEVICES
    vector< Device* > m_devices;
#endif // ENABLE_CLIENT_DEVICES

    Device* m_emu_device;

    char m_broadcast_addr[IPV4_ADDRSTRLEN];
};

#endif // _DeviceManager_H_
