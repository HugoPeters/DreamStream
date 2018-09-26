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

/////// Logging
// If you wish to disable logging (e.g. memory restraints on smaller devices)
// simply comment out this define, or define to 0
// You can also limit the buffer size, and log messages will simply cut off if they reach the limit
// Do note we also allocate 64 extra bytes for printing the severity and newline (see Logger::Log)
#define LOG_BUFF_SIZE			512

/////// DreamScreen specific
#define DS_PORT					8888
#define DS_PACKET_MAGIC			0xFC
#define DS_STATE_PAYLOAD_SIZE	0xFF

/////// Program
#define PACKET_BUFF_SIZE		(0xFF + 1)

// When defined, the functionality to manage other DreamScreen devices is enabled
// This means that the program will not only act as a device itself, but can also control others.
//#define ENABLE_CLIENT_DEVICES	

#define SECTOR_SUBSCRIPTION_TIMEOUT_MS 7000

// TODO
// Define this to 1 to remove all command names for debugging purposes
// If done so, it will use the cmd regions for printing
#define CMD_STRIP_NAMES			1

// Enable this to print the packet buffer when they are sent/received
//#define DEBUG_DUMP_CMD_DATA