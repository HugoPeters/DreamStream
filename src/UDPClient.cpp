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

#include "UDPClient.h"
#include "Utils.h"
#include <string.h>
#include <errno.h>

#if (defined(__linux__) || defined(_WIN32))
UDPClientImpl::UDPClientImpl()
	: m_socket(-1)
	, m_is_broadcast(false)
{
	memset(m_buffer, 0, sizeof(m_buffer));
}

UDPClientImpl::~UDPClientImpl()
{
}

bool UDPClientImpl::Init(const char* hostname, uint16_t port)
{
	Strfmt<16> portname("%i", port);

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

	struct addrinfo* res;

	int err = getaddrinfo(hostname, portname, &hints, &res);

	if (err != 0)
	{
		LOG_ERROR("Failed to resolve local address (%i)", err);
		return false;
	}

	m_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if (m_socket == -1)
	{
		LOG_ERROR("Failed to create socket: %s", strerror(errno));
		freeaddrinfo(res);
		return false;
	}

	// Set to non-blocking
	int32_t curflags = 0;

#ifdef _WIN32
	curflags = 1;

	err = ioctlsocket(m_socket, FIONBIO, (ULONG *)&curflags);

	if (err != 0)
	{
		LOG_ERROR("Failed to set socket to non-blocking (%i)", err);
		freeaddrinfo(res);
		return false;
	}
#elif defined(__linux__)
	curflags = fcntl(m_socket, F_GETFL);

	if (curflags < 0)
	{
		LOG_ERROR("Failed to set socket to non-blocking");
		freeaddrinfo(res);
		return false;
	}

	curflags |= O_NONBLOCK;

	err = fcntl(m_socket, F_SETFL, curflags);

	if (err != 0)
	{
		LOG_ERROR("Failed to set socket to non-blocking (%i)", err);
		freeaddrinfo(res);
		return false;
	}
#endif

	struct sockaddr_in bindInfo;

	bindInfo.sin_family = AF_INET;
	bindInfo.sin_port = htons(port);
	bindInfo.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(m_socket, (struct sockaddr*)&bindInfo, sizeof(bindInfo)) == -1)
	{
		LOG_ERROR("Failed to bind socket to address: %s", strerror(errno));
		freeaddrinfo(res);
		return false;
	}

	freeaddrinfo(res);

	return true;
}

bool UDPClientImpl::EnableBroadcast(bool enable)
{
	int ienable = enable ? 1 : 0;
	if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (void *)&ienable, sizeof(ienable)) == -1)
	{
		LOG_ERROR("Failed to set socketopt for broadcast mode: %s", strerror(errno));
		return false;
	}

	m_is_broadcast = enable;

	return true;
}

bool UDPClientImpl::Connect(const char* host, uint16_t port)
{
	//TODO
	return false;
}

bool UDPClientImpl::Send(const char* addr, uint16_t port, uint8_t* data, uint32_t dataSize)
{
	struct sockaddr_in inaddr;
	memset(&inaddr, 0, sizeof(inaddr));

	inaddr.sin_family = AF_INET;
	inaddr.sin_port = htons(port);
	inaddr.sin_addr.s_addr = inet_addr(addr);

	int32_t numBytesSent = sendto(m_socket, data, dataSize, 0, (sockaddr*)&inaddr, sizeof(inaddr));

	if (numBytesSent == -1)
	{
		LOG_ERROR("Failed to send %i bytes: %s", dataSize, strerror(errno));
		return false;
	}

	return true;
}

int32_t UDPClientImpl::Receive(UDPMessage* outMsg /*= 0*/, UDPMessageInfo* outMsgInfo /*= 0*/)
{
	struct sockaddr_in from;
	socklen_t from_len = sizeof(from);

	ssize_t count = recvfrom(m_socket, m_buffer, sizeof(m_buffer), 0, (struct sockaddr*)&from, &from_len);

	if (count == -1)
	{
		/* Expected error when non-blocking */
		if (errno == EAGAIN)
			return 0;

		LOG_ERROR("Failed to receive: %s", strerror(errno));
		return 0;
	}
	else if (count == sizeof(m_buffer))
	{
		LOG_WARN("Datagram was too large for allocated buffer");
	}

	if (outMsg)
	{
		outMsg->m_length = count;
		outMsg->m_data = m_buffer;
	}

	if (outMsgInfo)
	{
		inet_ntop(AF_INET, &(from.sin_addr), outMsgInfo->m_ipv4addr, INET_ADDRSTRLEN);
	}

	return count;
}

bool UDPClientImpl::Select(int32_t timeoutSec, int32_t timeoutUSec)
{
	FD_ZERO(&m_errorFds);
	FD_ZERO(&m_readFds);
	FD_ZERO(&m_writeFds);
	FD_SET(m_socket, &m_errorFds);
	FD_SET(m_socket, &m_readFds);
	FD_SET(m_socket, &m_writeFds);

	struct timeval  timeout;

	if (timeoutSec > 0 || timeoutUSec > 0)
	{
		timeout.tv_sec = timeoutSec;
		timeout.tv_usec = timeoutUSec;
	}

	int32_t numDescriptors = select(m_socket + 1, &m_readFds, &m_writeFds, &m_errorFds, &timeout);

	if (numDescriptors == 0)
	{
		LOG_WARN("Socket timed out during select()");
	}
	else if ((FD_ISSET(m_socket, &m_readFds)) || (FD_ISSET(m_socket, &m_writeFds)))
	{
		int32_t err = 0;
		socklen_t errSize = sizeof(err);

		if (getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &err, &errSize) == 0)
		{
			if (err != 0)
			{
				LOG_ERROR("Socket error: %s", strerror(err));
			}
			else
			{
				return true;
			}
		}
	}
	
	return false;
}


#endif