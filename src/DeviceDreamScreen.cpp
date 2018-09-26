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

#include "DeviceDreamScreen.h"
#include "Utils.h"
#include "Packet.h"

DeviceDreamScreen::DeviceDreamScreen(DeviceManager* manager, const char* ip, bool is4k)
	: Device(manager, is4k ? DeviceType::DreamScreen4K : DeviceType::DreamScreenHD, ip)
	, m_is4k(is4k)
{
}

DeviceDreamScreen::~DeviceDreamScreen()
{

}

void DeviceDreamScreen::SetStateFromPayload(const uint8_t* payload, int32_t payloadLength)
{
	memcpy(m_name, &payload[0], 16);
	if (strlen(m_name) == 0) strcpy(m_name, m_is4k ? "DreamScreen 4K" : "DreamScreen HD");
	memcpy(m_group_name, &payload[16], 16);
	if (strlen(m_group_name) == 0) strcpy(m_group_name, "Group");

	m_group_number = payload[32];
	m_mode = payload[33];
	m_brightness = payload[34];
	m_zones = payload[35];
	memcpy(m_zones_brightness, &payload[36], 4);
	memcpy(m_ambient_color, &payload[40], 3);

	memcpy(m_saturation, &payload[43], 3);
	memcpy(m_flex_setup, &payload[46], 6);
	m_music_mode_type = payload[52];
	memcpy(m_music_mode_colors, &payload[53], 3);
	memcpy(m_music_mode_weights, &payload[56], 3);
	memcpy(m_minimum_luminosity, &payload[59], 3);
	m_ambient_show_type = payload[62];

	m_fade_rate = payload[63];
	// ?
	m_indicator_light_auto_off = payload[69];
	m_usb_power_enable = payload[70];
	m_sector_broadcast_control = payload[71];
	m_sector_broadcast_timing = payload[72];
	m_hdmi_input = payload[73];
	m_music_mode_source = payload[74];
	memcpy(m_hdmi_name_1, &payload[75], 16);
	memcpy(m_hdmi_name_2, &payload[91], 16);
	memcpy(m_hdmi_name_3, &payload[107], 16);
	m_cec_passthrough_enable = payload[123];
	m_cec_switching_enable = payload[124];
	m_hpd_enable = payload[125];
	// ?
	m_video_frame_delay = payload[127];
	m_letterboxing_enable = payload[128];
	m_hdmi_active_channels = payload[129];
	memcpy(m_esp_firmware_version, &payload[130], 2);
	memcpy(m_pic_version_number, &payload[132], 2);
	m_color_boost = payload[134];

	if (payloadLength >= 137)
		m_cec_power_enable = payload[135];
	
	if (payloadLength >= 138)
		m_sku_setup = payload[136];

	if (payloadLength >= 139)
		m_boot_state = payload[137];

	if (payloadLength >= 140)
		m_pillarboxing_enable = payload[138];

	if (payloadLength >= 141)
		m_hdr_tonemapping = payload[139];
}

int32_t DeviceDreamScreen::SetStateToPayload(uint8_t* payload)
{
	LOG_WARN("TODO: DeviceDreamScreen::SetStateToPayload");
	return 0;
}

void DeviceDreamScreen::HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt)
{
	Super::HandlePacket(hs, msg, pkt);

	if (PKT_IS(PKT_BROADCAST | PKT_RESPONSE, pkt.m_flag))
	{
		switch (pkt.m_type)
		{
			case Commands::CurrentState: SetStateFromPayload(pkt.m_payload, pkt.m_payloadSize); hs.SetHandled(); break;
			default: hs.SetUnhandled(); break;
		}
	}
}
