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

#ifndef _Device_H_
#define _Device_H_

#include "UDPClient.h"
#include "StaticString.h"
#include "DeviceType.h"
#include "Commands.h"
#include <cassert>
#include "Packet.h"

// Sorry for the mess, but not going to write all this boilerplate code
#define DECLARE_STATEFUNC_UINT8(cmd, member)\
virtual bool Set##cmd(uint8_t newVal) { member = newVal; return SendCommandCustom(Commands::cmd, PKT_WRITE, (const uint8_t*)&newVal, sizeof(newVal)); }\
virtual uint8_t Get##cmd() const { return member; }

#define DECLARE_STATEFUNC_UINT8_ARRAY(cmd, member, arrSize)\
virtual bool Set##cmd(uint8_t data[arrSize]) { static_assert(arrSize == sizeof(member), "Array setter size mismatch!"); bool r = SendCommandConstantUnicastWrite(Commands::cmd, data, sizeof(member)); memcpy(member, data, sizeof(member)); return r; }\
virtual const uint8_t* Get##cmd() const { return member; }

#define DECLARE_STATEFUNC_STRING(cmd, member)\
virtual bool Set##cmd(const char* newVal) { \
assert(strlen(newVal) <= member.BufferLength());\
member = newVal;\
return SendCommandConstantUnicastWrite(Commands::cmd, (const uint8_t*)member.Get(), member.BufferLength()); }\
virtual const char* Get##cmd() const { return member; }

struct PacketInfo;
class DeviceManager;

/* Base class for all devices, both incoming and emulated */
class Device
{
public:
    Device(DeviceManager* manager, DeviceType::Type type, const char* ip);
    virtual ~Device();


    DeviceType::Type GetType() const { return m_type; }

    virtual void Update() { }

    void SetHostAddress(const char* address) { m_host_address = address; }
    void SetBroadcastAddress(const char* address) { m_broadcast_address = address; }

    const char* GetDeviceAddress() const { return m_device_address; }
    const char* GetHostAddress() const { return m_host_address; }
    const char* GetBroadcastAddress() const { return m_broadcast_address; }

    bool HasHostAddress() const { return m_host_address.Length() > 0; }

    DeviceManager* GetManager() const { return m_manager; }
    void SetEmulated(bool enable) { m_is_emulated = enable; }
    bool IsEmulated() const { return m_is_emulated; }

    // Network
    bool SendCommandWrite(Commands::Type type, bool broadcast, const  uint8_t* payload, int32_t payloadLength);
    bool SendCommandConstantUnicastWrite(Commands::Type type, const uint8_t* payload, int32_t payloadLength);
    bool SendCommandUnicastRead(Commands::Type type);
    bool SendCommandCustom(Commands::Type type, uint8_t flag, const uint8_t* payload, int32_t payloadLength);
    bool SendPacket(const char* ip, Commands::Type type, uint8_t flag, const uint8_t* payload = nullptr, int32_t payloadLength = 0);

    // State functions
    DECLARE_STATEFUNC_UINT8_ARRAY(AmbientColor, m_ambient_color, 3)
    DECLARE_STATEFUNC_UINT8(AmbientModeType, m_ambient_mode_type)
    DECLARE_STATEFUNC_UINT8(AmbientScene, m_ambient_show_type)
    DECLARE_STATEFUNC_UINT8(Brightness, m_brightness)
    DECLARE_STATEFUNC_UINT8(VideoFrameDelay, m_fade_rate)
    DECLARE_STATEFUNC_STRING(GroupName, m_group_name)
    DECLARE_STATEFUNC_UINT8(GroupNumber, m_group_number)
    DECLARE_STATEFUNC_UINT8(Mode, m_mode)
    DECLARE_STATEFUNC_STRING(Name, m_name)
    DECLARE_STATEFUNC_UINT8_ARRAY(Saturation, m_saturation, 3)

    // State inits, for some things that need to be transferred
    void InitGroupName(const char* name) { m_group_name = name; }

    // State updating
    virtual void SetStateFromPayload(const uint8_t* payload, int32_t payloadLength) = 0;
    virtual int32_t SetStateToPayload(uint8_t* payload) = 0;

    virtual void HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt);

protected:
    DeviceType::Type m_type;
    StaticString<IPV4_ADDRSTRLEN> m_device_address;
    StaticString<IPV4_ADDRSTRLEN> m_host_address;
    StaticString<IPV4_ADDRSTRLEN> m_broadcast_address;
    DeviceManager* m_manager;
    bool m_is_emulated;

    uint8_t m_ambient_color[3]              = { 0, 0, 0 };
    uint8_t m_ambient_mode_type             = 0;
    uint8_t m_ambient_show_type             = 0;
    uint8_t m_brightness                    = 255;
    uint8_t m_fade_rate                     = 4;
    StaticString<16> m_group_name           = "unassigned";
    uint8_t m_group_number                  = 0;
    uint8_t m_mode                          = 0;
    StaticString<16> m_name;
    uint8_t m_saturation[3]                 = { 0xFF, 0xFF, 0xFF };
};

#endif // _Device_H_