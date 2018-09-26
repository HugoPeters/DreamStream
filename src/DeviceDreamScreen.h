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

#ifndef _DeviceDreamScreen_H_
#define _DeviceDreamScreen_H_

#include "Device.h"

class DeviceDreamScreen : public Device
{
public:
	typedef Device Super;

	DeviceDreamScreen(DeviceManager* manager, const char* ip, bool is4k);
	virtual ~DeviceDreamScreen();

	bool GetIs4K() const { return m_is4k; }

	uint8_t GetBootState() const { return m_boot_state; }

	void SetStateFromPayload(const uint8_t* payload, int32_t payloadLength) override;
	int32_t SetStateToPayload(uint8_t* payload) override;

	virtual void HandlePacket(HandleState& hs, const UDPMessageInfo& msg, const PacketInfo& pkt) override;

	// State functions
	DECLARE_STATEFUNC_UINT8(Zones, m_zones)
	DECLARE_STATEFUNC_UINT8_ARRAY(ZonesBrightness, m_zones_brightness, 4)
	DECLARE_STATEFUNC_UINT8_ARRAY(FlexSetup, m_flex_setup, 6)
	DECLARE_STATEFUNC_UINT8(MusicModeType, m_music_mode_type)
	DECLARE_STATEFUNC_UINT8_ARRAY(MusicModeColors, m_music_mode_colors, 3)
	DECLARE_STATEFUNC_UINT8_ARRAY(MusicModeWeights, m_music_mode_weights, 3)
	DECLARE_STATEFUNC_UINT8_ARRAY(MinimumLuminosity, m_minimum_luminosity, 3)
	DECLARE_STATEFUNC_UINT8(IndicatorLightAutoOff, m_indicator_light_auto_off)
	DECLARE_STATEFUNC_UINT8(USBPowerEnable, m_usb_power_enable)
	DECLARE_STATEFUNC_UINT8(SectorBroadcastControl, m_sector_broadcast_control)
	DECLARE_STATEFUNC_UINT8(SectorBroadcastTiming, m_sector_broadcast_timing)
	DECLARE_STATEFUNC_UINT8(HDMIInput, m_hdmi_input)
	DECLARE_STATEFUNC_UINT8(MusicModeSource, m_music_mode_source)
	DECLARE_STATEFUNC_STRING(HDMIInputName1, m_hdmi_name_1)
	DECLARE_STATEFUNC_STRING(HDMIInputName2, m_hdmi_name_2)
	DECLARE_STATEFUNC_STRING(HDMIInputName3, m_hdmi_name_3)
	DECLARE_STATEFUNC_UINT8(CECPassthroughEnable, m_cec_passthrough_enable)
	DECLARE_STATEFUNC_UINT8(CECSwitchingEnable, m_cec_switching_enable)
	DECLARE_STATEFUNC_UINT8(HPDEnable, m_hpd_enable)
	DECLARE_STATEFUNC_UINT8(VideoFrameDelay, m_video_frame_delay)
	DECLARE_STATEFUNC_UINT8(HDMIActiveChannels, m_hdmi_active_channels)

	//TODO: ESP & PIC functionality needs custom setters as they require a key in the payload

	DECLARE_STATEFUNC_UINT8(ColorBoost, m_color_boost)
	DECLARE_STATEFUNC_UINT8(CECPowerEnable, m_cec_power_enable)
	DECLARE_STATEFUNC_UINT8(SKUSetup, m_sku_setup)
	DECLARE_STATEFUNC_UINT8(PillarboxingEnable, m_pillarboxing_enable)
	DECLARE_STATEFUNC_UINT8(HDRToneRemapping, m_hdr_tonemapping)

protected:
	bool m_is4k;

	uint8_t m_zones                             = 15;
	uint8_t m_zones_brightness[4]               = { 0xFF, 0xFF, 0xFF, 0xFF };
	uint8_t m_flex_setup[6]                     = { 8, 16, 48, 0, 7, 0 };
	uint8_t m_music_mode_type                   = 0;
	uint8_t m_music_mode_colors[3]              = { 2, 1, 0 };
	uint8_t m_music_mode_weights[3]             = { 100, 100, 100 };
	uint8_t m_minimum_luminosity[3]             = { 0, 0, 0 };
	uint8_t m_indicator_light_auto_off          = 1;
	uint8_t m_usb_power_enable                  = 0;
	uint8_t m_sector_broadcast_control          = 0;
	uint8_t m_sector_broadcast_timing           = 1;
	uint8_t m_hdmi_input                        = 0;
	uint8_t m_music_mode_source                 = 0;
	char m_hdmi_name_1[16]                      = "HDMI 1";
	char m_hdmi_name_2[16]                      = "HDMI 2";
	char m_hdmi_name_3[16]                      = "HDMI 3";
	uint8_t m_cec_passthrough_enable            = 1;
	uint8_t m_cec_switching_enable              = 1;
	uint8_t m_hpd_enable                        = 1;
	uint8_t m_video_frame_delay                 = 0;
	uint8_t m_letterboxing_enable               = 0;
	uint8_t m_hdmi_active_channels              = 0;
	uint8_t m_esp_firmware_version[2]           = { 0, 0 };
	uint8_t m_pic_version_number[2]             = { 0, 0 };
	uint8_t m_color_boost                       = 0;
	uint8_t m_cec_power_enable                  = 0;
	uint8_t m_sku_setup                         = 0;
	uint8_t m_boot_state                        = 0;
	uint8_t m_pillarboxing_enable               = 0;
	uint8_t m_hdr_tonemapping                   = 0;
};

#endif // _DeviceDreamScreen_H_