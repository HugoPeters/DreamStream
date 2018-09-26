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

#include "Packet.h"
#include "Utils.h"
#include "Defines.h"

bool PacketUtils::ReadPacketHeader(const uint8_t* data, int32_t dataSize, PacketInfo& outInfo)
{
    if (dataSize < sizeof(PacketHeader))
        return false;

    PacketHeader* hdr = (PacketHeader*)data;

    if (hdr->m_magic != DS_PACKET_MAGIC)
        return false;

    int packetSize = hdr->m_length + 2;

    if (packetSize > dataSize)
        return false;

    uint8_t storedCrc = data[packetSize - 1];
    uint8_t calcCrc = Utils::CalculateCRC8(data, packetSize - 1);

    if (calcCrc != storedCrc)
    {
        LOG_DEBUG("Packet CRCs did not match (stored=0x%x, calc=0x%x)", storedCrc, calcCrc);
        return false;
    }

    Commands::Type packetType = Commands::INVALID;

    CMD cmd = { hdr->m_cmd_upper, hdr->m_cmd_lower };

    //TODO: this can be optimized by setting the enum index to the cmd code?

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
        LOG_WARN("Unknown packet received! (cmd_upper=0x%x, cmd_lower=0x%x)", hdr->m_cmd_upper, hdr->m_cmd_lower);
        return false;
    }

    outInfo.m_hdr = hdr;
    outInfo.m_type = packetType;
    outInfo.m_payload = data + sizeof(PacketHeader);
    outInfo.m_payloadSize = packetSize - sizeof(PacketHeader) - 1; /* (-1 for CRC) */
    outInfo.m_flag = hdr->m_flags; // copy the flag to minimize access

    return true;
}
