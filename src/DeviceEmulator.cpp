/********************************************************************************
* Copyright (c) 2018 by Hugo Peters. http://hugo.fyi/							*
*																				*
* Permission is hereby granted, free of charge, to any person obtaining a copy	*
* of this software and associated documentation files (the "Software"), to deal	*
* in the Software without restriction, including without limitation the rights	*
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell		*
* copies of the Software, and to permit persons to whom the Software is			*
* furnished to do so, subject to the following conditions:						*
*																				*
* The above copyright notice and this permission notice shall be included in	*
* all copies or substantial portions of the Software.							*
*																				*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	*
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		*
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE	*
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER		*
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	*
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		*
* THE SOFTWARE.																	*
*********************************************************************************/

#include "DeviceEmulator.h"
#include "Commands.h"
#include "Packet.h"
#include "DeviceManager.h"
#include "DeviceType.h"
#include "PacketHelpers.h"

DeviceSideKickEmu::DeviceSideKickEmu(DeviceManager* manager)
	: DeviceSideKick(manager, "127.0.0.1")
	, m_has_active_sector_subscription(false)
{
	SetEmulated(true);

	m_group_number = 0x1;

	memset(m_name, 0, sizeof(m_name));
	strcpy(m_name, "DREAMSTREAM");
}

DeviceSideKickEmu::~DeviceSideKickEmu()
{

}

void DeviceSideKickEmu::Update()
{
	if (m_has_active_sector_subscription)
	{
		if (m_sector_subscription_timeout.Elapsed() > SECTOR_SUBSCRIPTION_TIMEOUT_MS)
		{
			// timeout!
			m_has_active_sector_subscription = false;

		#ifndef ENABLE_CLIENT_DEVICES
			SetHostAddress(nullptr);
		#endif

			LOG_INFO("Sector subscription timed out!");
		}
	}
}

void DeviceSideKickEmu::HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt)
{
	Super::HandlePacket(hs, msg, pkt);

	if (PKT_IS(PKT_RESPONSE | PKT_BROADCAST, pkt.m_flag))
	{
		switch (pkt.m_type)
		{
			case Commands::SectorData: HandlePacket_Response_SectorData(msg, pkt); hs.SetHandled(); break;
			default: hs.SetUnhandled(); break;
		}
	}

	if (PKT_IS(PKT_RESPONSE_REQUEST, pkt.m_flag))
	{
		switch (pkt.m_type)
		{
			case Commands::SubscribeToSectorData: HandlePacket_Read_SubscribeToSectorData(msg, pkt); hs.SetHandled(); break;
			case Commands::CurrentState: HandlePacket_Read_CurrentState(msg, pkt); hs.SetHandled(); break;
			case Commands::Ping: SendPacket(msg.m_ipv4addr, Commands::Ping, PKT_RESPONSE); hs.SetHandled(); break;
			default: hs.SetUnhandled(); break;
		}
	}

	if (PKT_IS(PKT_WRITE_CONSTANT, pkt.m_flag))
	{
		switch (pkt.m_type)
		{
			case Commands::Brightness: PacketUtils::TrySetFromPayload(&m_brightness, pkt);  hs.SetHandled(); break;
			case Commands::Mode: PacketUtils::TrySetFromPayload(&m_mode, pkt);  hs.SetHandled(); break;
			case Commands::GroupNumber: PacketUtils::TrySetFromPayload(&m_group_number, pkt);  hs.SetHandled(); break;
			case Commands::Name: PacketUtils::TrySetFromPayload(m_name, pkt);  hs.SetHandled(); break;
			case Commands::GroupName: PacketUtils::TrySetFromPayload(m_name, pkt);  hs.SetHandled(); break;
			default: hs.SetUnhandled(); break;
		}
	}
}

void DeviceSideKickEmu::HandlePacket_Read_CurrentState(const UDPMessageInfo& msg, const PacketInfo& pkt)
{
	uint8_t payload[DS_STATE_PAYLOAD_SIZE];
	memset(payload, 0, sizeof(payload));
	int32_t realPayloadSize = SetStateToPayload(payload);
	SendPacket(msg.m_ipv4addr, Commands::CurrentState, PKT_RESPONSE, payload, realPayloadSize);
}

void DeviceSideKickEmu::HandlePacket_Read_SubscribeToSectorData(const UDPMessageInfo& msg, const PacketInfo& pkt)
{
	if (!m_has_active_sector_subscription)
	{
	#ifndef ENABLE_CLIENT_DEVICES
		// We are not bound to any device at this point, so let's bind to this one.
		SetHostAddress(msg.m_ipv4addr);
	#endif

		m_has_active_sector_subscription = true;
		LOG_INFO("Received SubscribeToSectorData broadcast, requesting subscription to sector data...");
	}

	// send back the acknowledgement
	uint8_t flag = 1;
	SendCommandCustom(Commands::SubscribeToSectorData, PKT_RESPONSE_REQUEST, &flag, 1);
	m_sector_subscription_timeout.Reset();
}

void DeviceSideKickEmu::HandlePacket_Response_SectorData(const UDPMessageInfo& msg, const PacketInfo& pkt)
{
	if (PKT_IS(PKT_RESPONSE, pkt.m_hdr->m_flags))
	{
		if (const PacketSectorData* data = PacketUtils::GetPayloadAs<PacketSectorData>(pkt))
		{
		}
	}
}