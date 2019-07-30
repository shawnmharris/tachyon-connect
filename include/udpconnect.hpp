#pragma once

#include <tachyon/component.hpp>
#include <iconnect.hpp>
#include <os.hpp>
#include <netaddress.hpp>

namespace msgudp
{
	class UDPConnect : public IConnect
	{
	public:
		virtual ~UDPConnect();

		virtual int Bind(const char *host, unsigned short port);
		virtual bool PostInit();
		virtual int Recv(msgBuf *inMsg);
		virtual int Reply(const msgBuf &outMsg);
		virtual int Send(const msgBuf &outMsg, const char *host, unsigned short port);
		virtual int SetBlocking();
		virtual int SetNonBlocking();
		
		static tachyon::Component *Create();

	protected:
		UDPConnect();
		int bind(const char *host, unsigned short port);
		int recvFrom(char *mesg, unsigned short *bytesRecv, unsigned short maxLen, sockaddr_in *addr, int flags);
		int sendTo(sockaddr_in &addr, const char *mesg, unsigned short mesgLen, int flags);
		
		SOCKET m_socket;
		NetAddress m_reply;
	};
}


extern "C"
{
	EXPORT_API bool InitLibrary(tachyon::MasterFactory *m);
}
