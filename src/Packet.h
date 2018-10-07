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

#ifndef _Packet_H_
#define _Packet_H_

#include <stdint.h>
#include "Commands.h"
#include "StaticString.h"

#define PKT_RESPONSE            (1UL << 6)
#define PKT_BROADCAST           (1UL << 5)
#define PKT_RESPONSE_REQUEST    (1UL << 4)
#define PKT_WRITE               (1UL << 1)
#define PKT_WRITE_CONSTANT      (1UL << 0)

#define PKT_IS(x, y)            (((x) & y) != 0)

struct PacketHeader
{
    uint8_t m_magic;
    uint8_t m_length;
    uint8_t m_groupaddr;
    uint8_t m_flags;
    uint8_t m_cmd_upper;
    uint8_t m_cmd_lower;
};

struct PacketInfo
{
    Commands::Type m_type;
    const PacketHeader* m_hdr;
    const uint8_t* m_payload;
    int32_t m_payloadSize;
    uint8_t m_flag;
};

namespace PacketUtils
{
    inline bool ReadPacketHeader(const uint8_t* data, int32_t dataSize, PacketInfo& outInfo)
    {
        if (dataSize < sizeof(PacketHeader))
            return false;

        PacketHeader* hdr = (PacketHeader*)data;

        if (hdr->m_magic != DS_PACKET_MAGIC)
            return false;

        int packetSize = hdr->m_length + 2;

        if (packetSize > dataSize)
            return false;

        //uint8_t storedCrc = data[packetSize - 1];
        //uint8_t calcCrc = Utils::CalculateCRC8(data, packetSize - 1);

       // if (calcCrc != storedCrc)
        //{
        //    LOG_DEBUG("Packet CRCs did not match (stored=0x%x, calc=0x%x)", storedCrc, calcCrc);
        //    return false;
        //}

        Commands::Type packetType = Commands::INVALID;

        CMD cmd = { hdr->m_cmd_upper, hdr->m_cmd_lower };

        for (int i = Commands::INVALID + 1; i < Commands::NUM; ++i)
        {
            const CommandDescriptor& desc = Commands::Descriptors[i];

            if (desc.m_cmd.code == cmd.code)
            {
                packetType = static_cast<Commands::Type>(i);
            }
        }

        if (packetType == Commands::INVALID)
        {
            //LOG_WARN("Unknown packet received! (cmd_upper=0x%x, cmd_lower=0x%x)", hdr->m_cmd_upper, hdr->m_cmd_lower);
            return false;
        }

        outInfo.m_hdr = hdr;
        outInfo.m_type = packetType;
        outInfo.m_payload = data + sizeof(PacketHeader);
        outInfo.m_payloadSize = packetSize - sizeof(PacketHeader) - 1; /* (-1 for CRC) */
        outInfo.m_flag = hdr->m_flags; // copy the flag to minimize access

        return true;
    }

    template<typename T>
    static const T* GetPayloadAs(const PacketInfo& pkt)
    {
        if (pkt.m_payloadSize < sizeof(T))
            return nullptr;
        else
            return (T*)(pkt.m_payload);
    }

    template<typename T>
    static bool TrySetFromPayload(T& target, const PacketInfo& pkt)
    {
        assert(false);
        return false;
    }

    template<>
    bool TrySetFromPayload(uint8_t& target, const PacketInfo& pkt)
    {
        if (pkt.m_payloadSize < 1)
            return false;
        else
        {
            target = pkt.m_payload[0];
            return true;
        }
    }

    template<int32_t N>
    bool TrySetFromPayload(StaticString<N>& target, const PacketInfo& pkt)
    {
        if (pkt.m_payloadSize < 1)
            return false;
        else
        {
            target.CopyFrom(pkt.m_payload, pkt.m_payloadSize);
            return true;
        }
    }
}

#endif // _Packet_H_