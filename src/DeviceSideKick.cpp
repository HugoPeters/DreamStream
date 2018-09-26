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

#include "DeviceSideKick.h"
#include "Packet.h"

DeviceSideKick::DeviceSideKick(DeviceManager* manager, const char* ip)
    : Device(manager, DeviceType::SideKick, ip)
{
    memset(m_sector_assignment, 0, sizeof(m_sector_assignment));
    memset(m_esp_firmware_version, 0, sizeof(m_esp_firmware_version));
    m_name = "SideKick";
}

DeviceSideKick::~DeviceSideKick()
{

}

void DeviceSideKick::SetStateFromPayload(const uint8_t* payload, int32_t payloadLength)
{
    m_name.CopyFrom(&payload[0], 16);
    if (m_name.Length() == 0) m_name = "SideKick";
    m_group_name.CopyFrom(&payload[16], 16);
    if (m_group_name.Length() == 0) m_group_name = "unassigned";

    m_group_number = payload[32];
    m_mode = payload[33];
    m_brightness = payload[34];
    memcpy(m_ambient_color, &payload[35], 3);
    memcpy(m_saturation, &payload[38], 3);
    m_fade_rate = payload[41];
    memcpy(m_sector_assignment, &payload[42], 15);
    memcpy(m_esp_firmware_version, &payload[57], 2);

    if (payloadLength == 62)
    {
        m_ambient_mode_type = payload[59];
        m_ambient_show_type = payload[60];
    }
}

int32_t DeviceSideKick::SetStateToPayload(uint8_t* payload)
{
    memcpy(&payload[0], m_name, 16);
    memcpy(&payload[16], m_group_name, 16);
    payload[32] = m_group_number;
    payload[33] = m_mode;
    payload[34] = m_brightness;
    memcpy(&payload[35], m_ambient_color, 3);
    memcpy(&payload[38], m_saturation, 3);
    payload[41] = m_fade_rate;
    memcpy(&payload[42], m_sector_assignment, 15);
    memcpy(&payload[57], m_esp_firmware_version, 2);
    payload[59] = m_ambient_mode_type;
    payload[60] = m_ambient_show_type;
    payload[61] = uint8_t(GetType());
    return 62;
}

void DeviceSideKick::HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt)
{
    Super::HandlePacket(hs, msg, pkt);

    if (!IsEmulated() && PKT_IS(PKT_BROADCAST | PKT_RESPONSE, pkt.m_flag))
    {
        switch (pkt.m_type)
        {
            case Commands::CurrentState:
                SetStateFromPayload(pkt.m_payload, pkt.m_payloadSize);
                hs.SetHandled();
                break;
            default: hs.SetUnhandled(); break;
        }
    }
}
