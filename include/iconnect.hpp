#pragma once

#include <tachyon/component.hpp>

#define MESG_MAX_PAYLOAD 1400

struct sockaddr;
struct msgBuf;

class IConnect : public tachyon::Component
{
public:
	virtual ~IConnect() {}

	virtual int Bind(const char *host, unsigned short port) = 0;
	virtual bool PostInit() = 0;
	virtual int Recv(msgBuf *inMsg) = 0;
	virtual int Reply(const msgBuf &outMsg) = 0;
	virtual int Send(const msgBuf &outMsg, const char *host, unsigned short port) = 0;
	virtual int SetBlocking() = 0;
	virtual int SetNonBlocking() = 0;
};

#pragma pack(push, 1)

struct msgBuf
{
	unsigned short dataLen;
	unsigned char data[MESG_MAX_PAYLOAD];
};

#pragma pack(pop)

