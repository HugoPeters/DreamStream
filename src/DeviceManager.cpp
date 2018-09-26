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

#include "DeviceManager.h"
#include "Utils.h"
#include "Packet.h"
#include "DeviceDreamScreen.h"
#include "DeviceSideKick.h"
#include <string>
#include "DeviceEmulator.h"
#include "Enums.h"

bool DeviceManager::Init(const char* broadcastAddr)
{
    m_broadcast_addr = broadcastAddr;

    memset(m_packet_cache, 0, sizeof(PACKET_BUFF_SIZE));

    if (!m_client->Init(m_broadcast_addr, DS_PORT))
    {
        LOG_ERROR("Failed to initialize socket");
        return false;
    }

    return true;
}

void DeviceManager::Shutdown()
{
    if (m_client)
    {
        delete m_client;
        m_client = NULL;
    }

#ifdef ENABLE_CLIENT_DEVICES
    for (int i = 0; i < int(m_devices.size()); ++i)
    {
        delete m_devices[i];
    }

    m_devices.clear();
#endif // ENABLE_CLIENT_DEVICES

    if (m_emu_device)
    {
        delete m_emu_device;
        m_emu_device = nullptr;
    }
}

void DeviceManager::Update()
{
    while (true)
    {
        assert(m_client && "No UDP client exists");
        m_client->Select(1, 1000);

        UDPMessage msg;
        UDPMessageInfo msgInfo;

        int32_t numBytesReceived = m_client->Receive(&msg, &msgInfo);

        if (numBytesReceived == 0)
            break;

        // hack to stop messages sent to ourselves...
        // yikes... remove this asap...
        if (msgInfo.m_ipv4addr == "192.168.1.100")
            continue;

        HandleDataReceived(msg, msgInfo);
    }

    if (m_emu_device)
        m_emu_device->Update();

#ifdef ENABLE_CLIENT_DEVICES
    for (int i = 0; i < int(m_devices.size()); ++i)
    {
        m_devices[i]->Update();
    }
#endif // ENABLE_CLIENT_DEVICES
}

bool DeviceManager::BroadcastDiscovery()
{
#ifdef ENABLE_CLIENT_DEVICES
    if (m_broadcast_addr.Length())
    {
        LOG_DEBUG("Broadcast IP not set, use SetBroadcastAddress first");
        return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        SendPacket(m_broadcast_addr, 0xFF, PKT_BROADCAST | PKT_RESPONSE_REQUEST, Commands::CurrentState, NULL, 0);
        Utils::Sleep(500);
    }
#endif // ENABLE_CLIENT_DEVICES
    return true;
}

bool DeviceManager::Connect(const char* ip)
{
    //TODO
    return true;
}

bool DeviceManager::TryBindEmulatedDeviceToHost()
{
#ifdef ENABLE_CLIENT_DEVICES
    if (!m_emu_device)
        return false;

    if (m_emu_device->HasHostAddress())
    {
        return true;
    }
    else
    {
        for (int i = 0; i < int(m_devices.size()); ++i)
        {
            Device* dev = m_devices[i];

            if (dev->GetType() < DeviceType::SideKick
                && dev->GetGroupNumber() == m_emu_device->GetGroupNumber())
            {
                m_emu_device->SetHostAddress(dev->GetDeviceAddress());
                m_emu_device->InitGroupName(dev->GetGroupName());

                LOG_INFO("Emulated device bound to [%s] on group [%s] (%s)"
                    , dev->GetName(), dev->GetGroupName(), dev->GetDeviceAddress());

                return true;
            }
        }
    }
#endif // ENABLE_CLIENT_DEVICES
    return false;
}

int DeviceManager::PreparePacketData(uint8_t group, uint8_t flags, uint8_t cmdUpper, uint8_t cmdLower, const uint8_t* payloadData, int32_t payloadSize)
{
    int32_t packetSize = 7 + payloadSize;

    if (packetSize >= PACKET_BUFF_SIZE)
    {
        LOG_ERROR("PACKET PAYLOAD OVERFLOWS BUFFER (size=%i, max=%i)", packetSize, PACKET_BUFF_SIZE);
        return -1;
    }

    memset(m_packet_cache, 0, PACKET_BUFF_SIZE);
    m_packet_cache[0] = 0xFC;
    m_packet_cache[1] = 5 + payloadSize;
    m_packet_cache[2] = group;
    m_packet_cache[3] = flags;
    m_packet_cache[4] = cmdUpper;
    m_packet_cache[5] = cmdLower;

    if (payloadSize > 0)
    {
        assert(payloadData);
        memcpy(&m_packet_cache[6], payloadData, payloadSize);
    }

    uint8_t crc = Utils::CalculateCRC8(m_packet_cache, 6 + payloadSize);
    m_packet_cache[6 + payloadSize] = crc;
    return packetSize;
}

Device* DeviceManager::FindDevice(const char* ipaddr)
{
#ifdef ENABLE_CLIENT_DEVICES
    for (int i = 0; i < int(m_devices.size()); ++i)
    {
        if (strcmp(m_devices[i]->GetDeviceAddress(), ipaddr) == 0)
            return m_devices[i];
    }
#endif // ENABLE_CLIENT_DEVICES

    return nullptr;
}

DeviceSideKickEmu* DeviceManager::CreateEmulatedDevice()
{
    DeviceSideKickEmu* device = new DeviceSideKickEmu(this);
    device->SetBroadcastAddress(m_broadcast_addr);

    m_emu_device = device;

    TryBindEmulatedDeviceToHost();

    LOG_DEBUG("Created emulation device");

    return device;
}

bool DeviceManager::SendPacket(const char* addr, uint8_t group, uint8_t flag, Commands::Type cmd, const uint8_t* payloadData, int32_t payloadSize)
{
    bool isBroadcast = PKT_IS(PKT_BROADCAST, flag);

    if (isBroadcast)
        if (!m_client->EnableBroadcast(true))
            return false;

    const CommandDescriptor& desc = Commands::Descriptors[cmd];

    int packageSize = PreparePacketData(group, flag, desc.m_cmd.regions[0], desc.m_cmd.regions[1], payloadData, payloadSize);

    if (packageSize == -1)
    {
        if (isBroadcast)
            m_client->EnableBroadcast(false);

        return false;
    }

    bool ret = m_client->Send(addr, DS_PORT, m_packet_cache, packageSize);

    Strfmt<16> flagStr("(%i=%s%s%s%s%s)"
        , flag
        , PKT_IS(PKT_RESPONSE, flag) ? "r" : ""
        , PKT_IS(PKT_BROADCAST, flag) ? "b" : ""
        , PKT_IS(PKT_RESPONSE_REQUEST, flag) ? "q" : ""
        , PKT_IS(PKT_WRITE, flag) ? "w" : ""
        , PKT_IS(PKT_WRITE_CONSTANT, flag) ? "u" : "");

    if (ret)
        LOG_DEBUG("[%s]: Sent command [%s] %s", addr, desc.m_name, flagStr.GetBuffer());
    else
        LOG_ERROR("[%s]: Failed to send command [%s] %s", addr, desc.m_name, flagStr.GetBuffer());

#ifdef DEBUG_DUMP_CMD_DATA
    Utils::DumpBytes(m_packet_cache, packageSize);
#endif

    if (isBroadcast)
        m_client->EnableBroadcast(false);

    return ret;
}

void DeviceManager::HandleDataReceived(const UDPMessage& msg, const UDPMessageInfo& msgInfo)
{
    // stupid early safety check
    if (msg.m_length < 5)
        return;

    uint8_t flag = msg.m_data[3];

    Strfmt<16> flagStr("(%i=%s%s%s%s%s)"
        , flag
        , PKT_IS(PKT_RESPONSE, flag) ? "r" : ""
        , PKT_IS(PKT_BROADCAST, flag) ? "b" : ""
        , PKT_IS(PKT_RESPONSE_REQUEST, flag) ? "q" : ""
        , PKT_IS(PKT_WRITE, flag) ? "w" : ""
        , PKT_IS(PKT_WRITE_CONSTANT, flag) ? "u" : "");

    PacketInfo pkt;
    if (PacketUtils::ReadPacketHeader(msg.m_data, msg.m_length, pkt))
    {
        if (pkt.m_type != Commands::SectorData)
        {
            LOG_DEBUG("[%s]: Received packet [%s]! %s", msgInfo.m_ipv4addr.Get(), Commands::Descriptors[pkt.m_type].m_name, flagStr.GetBuffer());

        #ifdef DEBUG_DUMP_CMD_DATA
            Utils::DumpBytes(msg.m_data, msg.m_length);
        #endif
        }

    #ifdef ENABLE_CLIENT_DEVICES
        Device* dev = FindDevice(msgInfo.m_ipv4addr);

        if (dev == nullptr)
        {
            bool isResponse = PKT_IS(PKT_RESPONSE, msg.m_data[3]);
            bool isGroupBroadcast = PKT_IS(PKT_BROADCAST, msg.m_data[3]);
            bool isResponseRequested = PKT_IS(PKT_RESPONSE_REQUEST, msg.m_data[3]);
            bool isWrite = PKT_IS(PKT_WRITE, msg.m_data[3]);

            if (pkt.m_type == Commands::CurrentState && !isResponseRequested && isResponse)
            {
                HandlePacket_CurrentState_NewDevice(pkt, msgInfo);
            }
            else if (pkt.m_type == Commands::Ping && !isResponseRequested && isResponse)
            {
                LOG_INFO("Received ping from invalid device! Sending broadcast discovery...");
                BroadcastDiscovery();
            }
            else if (pkt.m_type == Commands::ESPConnectedToWiFi)
            {
                LOG_INFO("ESPConnectedToWiFi: %i", pkt.m_payload[0]);
            }
            else
            {
                // probably from app?
                HandlePacket(pkt, msgInfo);
            }

            return;
        }
    #endif // ENABLE_CLIENT_DEVICES

        HandlePacket(pkt, msgInfo);
    }
    else
    {
        LOG_WARN("[%s]: Received a bad packet %s", msgInfo.m_ipv4addr.Get(), flagStr.GetBuffer());
        Utils::DumpBytes(msg.m_data, msg.m_length, true);
    }
}

void DeviceManager::HandlePacket(const PacketInfo& pkt, const UDPMessageInfo& msgInfo)
{
    bool handled = false;

    HandleState hs;

#ifdef ENABLE_CLIENT_DEVICES
    Device* owner = FindDevice(msgInfo.m_ipv4addr);

    if (owner && PKT_IS(PKT_RESPONSE, pkt.m_flag) && pkt.m_type == Commands::CurrentState)
        owner->HandlePacket(hs, msgInfo, pkt);
    else
    {
    #endif // ENABLE_CLIENT_DEVICES
        if (m_emu_device && (pkt.m_hdr->m_groupaddr == m_emu_device->GetGroupNumber() || pkt.m_hdr->m_groupaddr == 0xFF))
        {
            m_emu_device->HandlePacket(hs, msgInfo, pkt);
        }
    #ifdef ENABLE_CLIENT_DEVICES
        // only in very rare cases do we need to handle something on managed devices
        // need to check if it's actually ONLY broadcasts and state updates
        // but any reads should not be handled by default...
        //if (PKT_IS(PKT_RESPONSE_REQUEST, pkt.m_flag) == false)
        //{
        //	if ((hs.IsFullyUnhandled() || PKT_IS(PKT_BROADCAST, pkt.m_flag)) && owner && (pkt.m_hdr->m_groupaddr == owner->GetGroupNumber() || pkt.m_hdr->m_groupaddr == 0xFF))
        //	{
        //		owner->HandlePacket(hs, nullptr, msgInfo, pkt);
        //	}
        //}
    }
#endif // ENABLE_CLIENT_DEVICES

    if (hs.IsFullyUnhandled())
    {
        uint8_t flag = pkt.m_flag;
        Strfmt<16> flagStr("(%i=%s%s%s%s%s)"
            , flag
            , PKT_IS(PKT_RESPONSE, flag) ? "r" : ""
            , PKT_IS(PKT_BROADCAST, flag) ? "b" : ""
            , PKT_IS(PKT_RESPONSE_REQUEST, flag) ? "q" : ""
            , PKT_IS(PKT_WRITE, flag) ? "w" : ""
            , PKT_IS(PKT_WRITE_CONSTANT, flag) ? "u" : "");

        LOG_DEBUG("[%s]: Unhandled packet (handled=%i, unhandled=%i) [%s] %s", msgInfo.m_ipv4addr.Get(), hs.GetNumHandled(), hs.GetNumUnhandled(), Commands::Descriptors[pkt.m_type].m_name, flagStr.GetBuffer());
    }
}

#ifdef ENABLE_CLIENT_DEVICES
void DeviceManager::HandlePacket_CurrentState_NewDevice(const PacketInfo& pkt, const UDPMessageInfo& msgInfo)
{
    Device* dev = nullptr;

    DeviceType::Type devType = static_cast<DeviceType::Type>(pkt.m_payload[pkt.m_payloadSize - 1]);

    if (devType != DeviceType::SideKick)
    {
        DeviceDreamScreen* newDev = nullptr;

        switch (devType)
        {
            case DeviceType::DreamScreen4K: newDev = new DeviceDreamScreen(this, msgInfo.m_ipv4addr, true); break;
            case DeviceType::DreamScreenHD: newDev = new DeviceDreamScreen(this, msgInfo.m_ipv4addr, false); break;
        }

        newDev->SetStateFromPayload(pkt.m_payload, pkt.m_payloadSize);

        uint8_t bootState = newDev->GetBootState();

        bool bootOk = false;

        switch (bootState)
        {
            case 0:
            case 1:
            {
                bootOk = true;
                m_devices.push_back(newDev);
            }; break;
            case 2:
            {
                bootOk = true;
                m_devices.push_back(newDev);
                LOG_WARN("Received a device in bootloader mode! (%s)", msgInfo.m_ipv4addr.Get());
            }; break;
            case 3:
            case 4:
            {
                LOG_WARN("Received a device with a bad boot state (%i): %s", bootState, msgInfo.m_ipv4addr.Get());
                bootOk = false;
            }; break;
            default:
            {
                LOG_WARN("Received a device with an invalid boot state (%i): %s", bootState, msgInfo.m_ipv4addr.Get());
                bootOk = false;
            }; break;
        }

        if (bootOk)
        {
            dev = newDev;
            TryBindEmulatedDeviceToHost();
        }
        else
        {
            delete newDev;
            dev = nullptr;
        }
    }
    else
    {
        DeviceSideKick* newDev = new DeviceSideKick(this, msgInfo.m_ipv4addr);
        newDev->SetStateFromPayload(pkt.m_payload, pkt.m_payloadSize);
        m_devices.push_back(newDev);

        dev = newDev;
    }

    if (dev)
    {
        dev->SetBroadcastAddress(m_broadcast_addr);
        dev->SetHostAddress(msgInfo.m_ipv4addr);

        LOG_INFO("[%s]: Found a new device! (name=%s [group=%s] : type=%s)"
            , msgInfo.m_ipv4addr.Get(), dev->GetName(), dev->GetGroupName(), DeviceType::Names[dev->GetType()]);
    }
}
#endif // ENABLE_CLIENT_DEVICES

DeviceManager::DeviceManager()
    : m_emu_device(nullptr)
{
    m_client = UDPClientFactory::CreateClient();
    LOG_DEBUG("Created: DeviceManager");
}

DeviceManager::~DeviceManager()
{
    Shutdown();

    LOG_DEBUG("Destroyed: DeviceManager");
}

