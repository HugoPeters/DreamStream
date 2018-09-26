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

// simple xmacro to define commands
#ifndef DECLARE_COMMAND
#define DECLARE_COMMAND(...)
#endif

//				Enum Type,				Name						Cmd Upper	Cmd Lower
DECLARE_COMMAND(INVALID,				"INVALID",					0xFF,		0xFF		)
DECLARE_COMMAND(ResetESP,				"ResetESP",					0x01,		0x05		)
DECLARE_COMMAND(Name,					"Name",						0x01,		0x07		)
DECLARE_COMMAND(GroupName,				"GroupName",				0x01,		0x08		)
DECLARE_COMMAND(GroupNumber,			"GroupNumber",				0x01,		0x09		)
DECLARE_COMMAND(CurrentState,			"CurrentState",				0x01,		0x0A		)
DECLARE_COMMAND(Ping,					"Ping",						0x01,		0x0B		)
DECLARE_COMMAND(SubscribeToSectorData,	"SubscribeToSectorData",	0x01,		0x0C		)
DECLARE_COMMAND(ReadBootloaderMode,		"ReadBootloaderMode",		0x01,		0x15		)

DECLARE_COMMAND(ReadPICVersionNumber,	"ReadPICVersionNumber",		0x02,		0x02		)
DECLARE_COMMAND(ReadDiagnostics,		"ReadDiagnostics",			0x02,		0x03		)

DECLARE_COMMAND(Mode,					"Mode",						0x03,		0x01		)
DECLARE_COMMAND(Brightness,				"Brightness",				0x03,		0x02		)
DECLARE_COMMAND(Zones,					"Zones",					0x03,		0x03		)
DECLARE_COMMAND(ZonesBrightness,		"ZonesBrightness",			0x03,		0x04		)
DECLARE_COMMAND(AmbientColor,			"AmbientColor",				0x03,		0x05		)
DECLARE_COMMAND(Saturation,				"Saturation",				0x03,		0x06		)
DECLARE_COMMAND(AmbientModeType,		"AmbientModeType",			0x03,		0x08		)
DECLARE_COMMAND(MusicModeType,			"MusicModeType",			0x03,		0x09		)
DECLARE_COMMAND(MusicModeColors,		"MusicModeColors",			0x03,		0x0A		)
DECLARE_COMMAND(MusicModeWeights,		"MusicModeWeights",			0x03,		0x0B		)
DECLARE_COMMAND(MinimumLuminosity,		"MinimumLuminosity",		0x03,		0x0C		)
DECLARE_COMMAND(AmbientScene,			"AmbientScene",				0x03,		0x0D		)
// missing
DECLARE_COMMAND(IndicatorLightAutoOff,	"IndicatorLightAutoOff",	0x03,		0x13		)
DECLARE_COMMAND(USBPowerEnable,			"USBPowerEnable",			0x03,		0x14		)
// missing
DECLARE_COMMAND(SectorData,				"SectorData",				0x03,		0x16		)
DECLARE_COMMAND(SectorAssignment,		"SectorAssignment",			0x03,		0x17		)
DECLARE_COMMAND(SectorBroadcastControl,	"SectorBroadcastControl",	0x03,		0x18		)
DECLARE_COMMAND(SectorBroadcastTiming,	"SectorBroadcastTiming",	0x03,		0x19		)
DECLARE_COMMAND(HDMIInput,				"HDMIInput",				0x03,		0x20		)
DECLARE_COMMAND(MusicModeSource,		"MusicModeSource",			0x03,		0x21		)
//DECLARE_COMMAND(AppMusicData,			"AppMusicData",				0x03,		0x21		) seems to be leftover
// missing
DECLARE_COMMAND(HDMIInputName1,			"HDMIInputName1",			0x03,		0x23		)
DECLARE_COMMAND(HDMIInputName2,			"HDMIInputName2",			0x03,		0x24		)
DECLARE_COMMAND(HDMIInputName3,			"HDMIInputName3",			0x03,		0x25		)
DECLARE_COMMAND(CECPassthroughEnable,	"CECPassthroughEnable",		0x03,		0x26		)
DECLARE_COMMAND(CECSwitchingEnable,		"CECSwitchingEnable",		0x03,		0x27		)
DECLARE_COMMAND(HPDEnable,				"HPDEnable",				0x03,		0x28		)
// missing
DECLARE_COMMAND(VideoFrameDelay,		"VideoFrameDelay",			0x03,		0x2A		)
DECLARE_COMMAND(LetterboxingEnable,		"LetterboxingEnable",		0x03,		0x2B		)
DECLARE_COMMAND(HDMIActiveChannels,		"HDMIActiveChannels",		0x03,		0x2C		)
DECLARE_COMMAND(ColorBoost,				"ColorBoost",				0x03,		0x2D		)
DECLARE_COMMAND(CECPowerEnable,			"CECPowerEnable",			0x03,		0x2E		)
DECLARE_COMMAND(PillarboxingEnable,		"PillarboxingEnable",		0x03,		0x2F		)
DECLARE_COMMAND(SKUSetup,				"SKUSetup",					0x03,		0x40		)
DECLARE_COMMAND(FlexSetup,				"FlexSetup",				0x03,		0x41		)
DECLARE_COMMAND(HDRToneRemapping,		"HDRToneRemapping",			0x03,		0x60		)

DECLARE_COMMAND(BootloaderFlags,		"BootloaderFlags",			0x04,		0x01		)
DECLARE_COMMAND(ResetPIC,				"ResetPIC",					0x04,		0x02		)
DECLARE_COMMAND(ESPConnectedToWiFi,		"ESPConnectedToWiFi",		0x04,		0x0D		)
DECLARE_COMMAND(OtherConnectedToWiFi,	"OtherConnectedToWiFi",		0x04,		0x14		)