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

#ifndef _UDPClient_H_
#define _UDPClient_H_

#include <cstdint>

#if (defined(__linux__) || defined(_WIN32))
//#include "clsocket/ActiveSocket.h"
//#include "clsocket/PassiveSocket.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define IPV4_ADDRSTRLEN INET_ADDRSTRLEN
#endif

struct UDPMessage
{
	int32_t m_length;
	uint8_t* m_data;
};

struct UDPMessageInfo
{
	char m_ipv4addr[IPV4_ADDRSTRLEN];
};

struct UDPClient
{
	UDPClient() { }
	virtual ~UDPClient() { }

	virtual bool Init(const char* hostname, uint16_t port) = 0;
	virtual bool EnableBroadcast(bool enable) = 0;
	virtual bool IsBroadcast() const = 0;
	virtual bool Connect(const char* ip, uint16_t port) = 0;
	virtual bool Send(const char* addr, uint16_t port, uint8_t* data, uint32_t dataSize) = 0;
	virtual int32_t Receive(UDPMessage* outMsg = 0, UDPMessageInfo* outMsgInfo = 0) = 0;
	virtual bool Select(int32_t timeoutSec, int32_t timeoutUSec) = 0;
};

#if (defined(__linux__) || defined(_WIN32))
struct UDPClientImpl : public UDPClient
{
	UDPClientImpl();
	~UDPClientImpl();

	bool Init(const char* hostname, uint16_t port) override;
	bool EnableBroadcast(bool enable) override;
	bool IsBroadcast() const override { return m_is_broadcast; }
	bool Connect(const char* ip, uint16_t port) override;
	bool Send(const char* addr, uint16_t port, uint8_t* data, uint32_t dataSize) override;
	int32_t Receive(UDPMessage* outMsg, UDPMessageInfo* outMsgInfo) override;
	bool Select(int32_t timeoutSec, int32_t timeoutUSec) override;

private:
	int m_socket;
	uint8_t m_buffer[549];
	bool m_is_broadcast;
	fd_set m_writeFds;
	fd_set m_readFds;
	fd_set m_errorFds;

};
#else
#error No UDPClient implementation!
#endif

namespace UDPClientFactory
{
	static UDPClient* CreateClient()
	{
		return new UDPClientImpl();
	}
}

#endif // _UDPClient_H_
