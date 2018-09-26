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

#include "Device.h"
#include <cassert>
#include "Commands.h"
#include "DeviceManager.h"
#include "Packet.h"

Device::Device(DeviceManager* manager, DeviceType::Type type, const char* ip)
    : m_type(type)
    , m_manager(manager)
{
    assert(manager);
    memset(m_device_address, 0, sizeof(m_device_address));
    memset(m_broadcast_address, 0, sizeof(m_broadcast_address));
    memset(m_host_address, 0, sizeof(m_host_address));
    strcpy(m_device_address, ip);

    memset(m_name, 0, sizeof(m_name));
}

Device::~Device()
{

}

bool Device::SendCommandWrite(Commands::Type type, bool broadcast, uint8_t* payload, int32_t payloadLength)
{
    if (!m_is_emulated)
        return false;

    if (!broadcast && !HasHostAddress())
    {
        LOG_DEBUG("SendCommandWrite failed: no host address set.");
        return false;
    }

    uint8_t flag = broadcast ? (PKT_BROADCAST | PKT_WRITE_CONSTANT) : (PKT_RESPONSE_REQUEST | PKT_WRITE_CONSTANT);
    const char* addr = broadcast ? m_broadcast_address : m_host_address;
    return GetManager()->SendPacket(addr, m_group_number, flag, type, payload, payloadLength);
}

bool Device::SendCommandConstantUnicastWrite(Commands::Type type, uint8_t* payload, int32_t payloadLength)
{
    if (!HasHostAddress())
    {
        LOG_DEBUG("SendCommandConstantUnicastWrite failed: no host address set.");
        return false;
    }

    return GetManager()->SendPacket(m_host_address, m_group_number, PKT_WRITE_CONSTANT, type, payload, payloadLength);
}

bool Device::SendCommandUnicastRead(Commands::Type type)
{
    if (!m_is_emulated)
        return false;

    if (!HasHostAddress())
    {
        LOG_DEBUG("SendCommandUnicastRead failed: no host address set.");
        return false;
    }

    return GetManager()->SendPacket(m_host_address, m_group_number, PKT_RESPONSE_REQUEST, type, NULL, 0);

}

bool Device::SendCommandCustom(Commands::Type type, uint8_t flag, uint8_t* payload, int32_t payloadLength)
{
    if (!m_is_emulated)
        return false;

    if (!HasHostAddress())
    {
        LOG_DEBUG("SendCommandCustom failed: no host address set.");
        return false;
    }

    return GetManager()->SendPacket(m_host_address, m_group_number, flag, type, payload, payloadLength);
}

bool Device::SendPacket(const char* ip, Commands::Type type, uint8_t flag, uint8_t* payload /*= nullptr*/, int32_t payloadLength /*= 0*/)
{
    return GetManager()->SendPacket(ip, m_group_number, flag, type, payload, payloadLength);
}

void Device::HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt)
{
    if (PKT_IS(PKT_BROADCAST, pkt.m_hdr->m_flags))
    {
        switch (pkt.m_type)
        {
            case Commands::Mode: PacketUtils::TrySetFromPayload(&m_mode, pkt); hs.SetHandled(); break;
            default: hs.SetUnhandled(); break;
        }
    }
}
