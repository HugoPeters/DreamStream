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
#include "DeviceType.h"
#include "Commands.h"
#include <cassert>
#include "Packet.h"

// Sorry for the mess, but not going to write all this boilerplate code
#define DECLARE_STATEVAR_UINT8(cmd, member)\
virtual bool Set##cmd(uint8_t newVal) { bool r = SendCommandCustom(Commands::cmd, PKT_WRITE, (uint8_t*)&newVal, sizeof(newVal)); if (r) { member = newVal; } return r; }\
virtual uint8_t Get##cmd() const { return member; }

#define DECLARE_STATEVAR_UINT8_ARRAY(cmd, member, arrSize)\
virtual bool Set##cmd(uint8_t data[arrSize]) { static_assert(arrSize == sizeof(member), "Array setter size mismatch!"); bool r = SendCommandConstantUnicastWrite(Commands::cmd, data, sizeof(member)); if (r) { memcpy(member, data, sizeof(member)); } return r; }\
virtual const uint8_t* Get##cmd() const { return member; }

#define DECLARE_STATEVAR_STRING(cmd, member)\
virtual bool Set##cmd(const char* newVal) { \
assert(strlen(newVal) < sizeof(member) - 1);\
char tmp[sizeof(member)]; memset(tmp, 0, sizeof(tmp)); strcpy(tmp, newVal);\
bool r = SendCommandConstantUnicastWrite(Commands::cmd, (uint8_t*)tmp, sizeof(member));\
if (r) { memcpy(member, tmp, sizeof(member)); } return r; }\
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

    void SetHostAddress(const char* address) { if (address) strcpy(m_host_address, address); else memset(m_host_address, 0, sizeof(m_host_address)); }
    void SetBroadcastAddress(const char* address) { strcpy(m_broadcast_address, address); }

    const char* GetDeviceAddress() const { return m_device_address; }
    const char* GetHostAddress() const { return m_host_address; }
    const char* GetBroadcastAddress() const { return m_broadcast_address; }

    bool HasHostAddress() const { return strlen(m_host_address) > 0; }

    DeviceManager* GetManager() const { return m_manager; }
    void SetEmulated(bool enable) { m_is_emulated = enable; }
    bool IsEmulated() const { return m_is_emulated; }

    // Network
    bool SendCommandWrite(Commands::Type type, bool broadcast, uint8_t* payload, int32_t payloadLength);
    bool SendCommandConstantUnicastWrite(Commands::Type type, uint8_t* payload, int32_t payloadLength);
    bool SendCommandUnicastRead(Commands::Type type);
    bool SendCommandCustom(Commands::Type type, uint8_t flag, uint8_t* payload, int32_t payloadLength);
    bool SendPacket(const char* ip, Commands::Type type, uint8_t flag, uint8_t* payload = nullptr, int32_t payloadLength = 0);

    // State variables
    DECLARE_STATEVAR_UINT8_ARRAY(AmbientColor, m_ambient_color, 3)
    DECLARE_STATEVAR_UINT8(AmbientModeType, m_ambient_mode_type)
    DECLARE_STATEVAR_UINT8(AmbientScene, m_ambient_show_type)
    DECLARE_STATEVAR_UINT8(Brightness, m_brightness)
    DECLARE_STATEVAR_UINT8(VideoFrameDelay, m_fade_rate)
    DECLARE_STATEVAR_STRING(GroupName, m_group_name)
    DECLARE_STATEVAR_UINT8(GroupNumber, m_group_number)
    DECLARE_STATEVAR_UINT8(Mode, m_mode)
    DECLARE_STATEVAR_STRING(Name, m_name)
    DECLARE_STATEVAR_UINT8_ARRAY(Saturation, m_saturation, 3)

    // State inits, for some things that need to be transferred
    void InitGroupName(const char* name) { strcpy(m_group_name, name); }

    // State updating
    virtual void SetStateFromPayload(const uint8_t* payload, int32_t payloadLength) = 0;
    virtual int32_t SetStateToPayload(uint8_t* payload) = 0;

    virtual void HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt);

protected:
    DeviceType::Type m_type;
    char m_device_address[IPV4_ADDRSTRLEN];
    char m_host_address[IPV4_ADDRSTRLEN];
    char m_broadcast_address[IPV4_ADDRSTRLEN];
    DeviceManager* m_manager;
    bool m_is_emulated;

    uint8_t m_ambient_color[3]              = { 0, 0, 0 };
    uint8_t m_ambient_mode_type             = 0;
    uint8_t m_ambient_show_type             = 0;
    uint8_t m_brightness                    = 0;
    uint8_t m_fade_rate                     = 4;
    char m_group_name[16]                   = "unassigned";
    uint8_t m_group_number                  = 0;
    uint8_t m_mode                          = 0;
    char m_name[16];
    uint8_t m_saturation[3]                 = { 0xFF, 0xFF, 0xFF };
};

#endif // _Device_H_