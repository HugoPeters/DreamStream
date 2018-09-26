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

#ifndef _Enums_H_
#define _Enums_H_

namespace DeviceMode
{
	enum
	{
		SLEEP = 0,
		VIDEO = 1,
		MUSIC = 2,
		AMBIENT = 3
	};
}

namespace AmbientModeType
{
	enum
	{
		RGB = 0,
		SCENE = 1
	};
}

namespace AmbientScene
{
	enum
	{
		RANDOM_COLOR = 0,
		FIRESIDE = 1,
		TWINKLE = 2,
		OCEAN = 3,
		RAINBOW = 4,
		JULY4TH = 5,
		HOLIDAY = 6,
		POP = 7,
		ENCHANTED_FOREST = 8
	};
}

#endif // _Enums_H_