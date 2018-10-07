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

// simple xmacro to define commands
#ifndef DECLARE_COMMAND
#define DECLARE_COMMAND(...)
#endif

//              Enum Type,                Cmd Upper   Cmd Lower
DECLARE_COMMAND(INVALID,                  0xFF,       0xFF        )
DECLARE_COMMAND(ResetESP,                 0x01,       0x05        )
DECLARE_COMMAND(Name,                     0x01,       0x07        )
DECLARE_COMMAND(GroupName,                0x01,       0x08        )
DECLARE_COMMAND(GroupNumber,              0x01,       0x09        )
DECLARE_COMMAND(CurrentState,             0x01,       0x0A        )
DECLARE_COMMAND(Ping,                     0x01,       0x0B        )
DECLARE_COMMAND(SubscribeToSectorData,    0x01,       0x0C        )
DECLARE_COMMAND(ReadBootloaderMode,       0x01,       0x15        )

DECLARE_COMMAND(ReadPICVersionNumber,     0x02,       0x02        )
DECLARE_COMMAND(ReadDiagnostics,          0x02,       0x03        )

DECLARE_COMMAND(Mode,                     0x03,       0x01        )
DECLARE_COMMAND(Brightness,               0x03,       0x02        )
DECLARE_COMMAND(Zones,                    0x03,       0x03        )
DECLARE_COMMAND(ZonesBrightness,          0x03,       0x04        )
DECLARE_COMMAND(AmbientColor,             0x03,       0x05        )
DECLARE_COMMAND(Saturation,               0x03,       0x06        )
DECLARE_COMMAND(AmbientModeType,          0x03,       0x08        )
DECLARE_COMMAND(MusicModeType,            0x03,       0x09        )
DECLARE_COMMAND(MusicModeColors,          0x03,       0x0A        )
DECLARE_COMMAND(MusicModeWeights,         0x03,       0x0B        )
DECLARE_COMMAND(MinimumLuminosity,        0x03,       0x0C        )
DECLARE_COMMAND(AmbientScene,             0x03,       0x0D        )
// missing
DECLARE_COMMAND(IndicatorLightAutoOff,    0x03,       0x13        )
DECLARE_COMMAND(USBPowerEnable,           0x03,       0x14        )
// missing
DECLARE_COMMAND(SectorData,               0x03,       0x16        )
DECLARE_COMMAND(SectorAssignment,         0x03,       0x17        )
DECLARE_COMMAND(SectorBroadcastControl,   0x03,       0x18        )
DECLARE_COMMAND(SectorBroadcastTiming,    0x03,       0x19        )
DECLARE_COMMAND(HDMIInput,                0x03,       0x20        )
DECLARE_COMMAND(MusicModeSource,          0x03,       0x21        )
//DECLARE_COMMAND(AppMusicData,           0x03,       0x21        ) seems to be leftover
// missing
DECLARE_COMMAND(HDMIInputName1,           0x03,       0x23        )
DECLARE_COMMAND(HDMIInputName2,           0x03,       0x24        )
DECLARE_COMMAND(HDMIInputName3,           0x03,       0x25        )
DECLARE_COMMAND(CECPassthroughEnable,     0x03,       0x26        )
DECLARE_COMMAND(CECSwitchingEnable,       0x03,       0x27        )
DECLARE_COMMAND(HPDEnable,                0x03,       0x28        )
// missing
DECLARE_COMMAND(VideoFrameDelay,          0x03,       0x2A        )
DECLARE_COMMAND(LetterboxingEnable,       0x03,       0x2B        )
DECLARE_COMMAND(HDMIActiveChannels,       0x03,       0x2C        )
DECLARE_COMMAND(ColorBoost,               0x03,       0x2D        )
DECLARE_COMMAND(CECPowerEnable,           0x03,       0x2E        )
DECLARE_COMMAND(PillarboxingEnable,       0x03,       0x2F        )
DECLARE_COMMAND(SKUSetup,                 0x03,       0x40        )
DECLARE_COMMAND(FlexSetup,                0x03,       0x41        )
DECLARE_COMMAND(HDRToneRemapping,         0x03,       0x60        )

DECLARE_COMMAND(BootloaderFlags,          0x04,       0x01        )
DECLARE_COMMAND(ResetPIC,                 0x04,       0x02        )
DECLARE_COMMAND(ESPConnectedToWiFi,       0x04,       0x0D        )
DECLARE_COMMAND(OtherConnectedToWiFi,     0x04,       0x14        )