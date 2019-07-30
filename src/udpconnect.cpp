#include <tachyon/component.hpp>
#include <os.hpp>
#include <networkutil.hpp>
#include <udpconnect.hpp>
#include <netaddress.hpp>

#define NAME "IConnect.UDPConnect"

namespace msgudp
{
	UDPConnect::UDPConnect()
	{
	}

	UDPConnect::~UDPConnect()
	{
		closesocket(m_socket);
	}
	
	int UDPConnect::Bind(const char *host, unsigned short port)
	{
		NetAddress addr(host, port);
		return bind(addr.Host().c_str(), addr.Port());
	}

	bool UDPConnect::PostInit()
	{
		NetworkUtil &nu = NetworkUtil::Instance();

		if (!nu.IsNetworkInit())
		{
			std::cerr << "network error " << NetworkUtil::Instance().GetLastError() << std::endl;
			return false;
		}

		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (m_socket == INVALID_SOCKET)
		{
			std::cerr << "socket error " << NetworkUtil::Instance().GetLastError() << std::endl;
			return false;
		}

		return true;
	}

	int UDPConnect::Recv(msgBuf *inMsg)
	{
		sockaddr_in addr;
		int ret = recvFrom(reinterpret_cast<char *>(inMsg->data), &(inMsg->dataLen), MESG_MAX_PAYLOAD, &addr, 0);

		if (ret > 0)
		{
			m_reply = NetAddress(addr);
		}

		return ret;
	}

	int UDPConnect::Reply(const msgBuf &outMsg)
	{
		if (!m_reply.isValid())
		{
			std::cerr << "no valid reply address" << std::endl;
			return -1;
		}

		sockaddr_in replyAddr = m_reply.Addr();
		return sendTo(replyAddr, reinterpret_cast<const char *>(outMsg.data), outMsg.dataLen, 0);
	}

	int UDPConnect::Send(const msgBuf &outMsg, const char *host, unsigned short port)
	{
		NetAddress netAddr(host, port);

		if (outMsg.dataLen > MESG_MAX_PAYLOAD)
		{
			std::cerr << "message payload exceeds max data size" << std::endl;
			return false;
		}

		sockaddr_in addr = netAddr.Addr();
		return sendTo(addr, reinterpret_cast<const char *>(outMsg.data), outMsg.dataLen, 0);
	}



	int UDPConnect::SetNonBlocking()
	{
#ifdef _WIN32
		u_long mode = 1;
		return ioctlsocket(m_socket, FIONBIO, &mode);
#elif __linux__		
		return fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif
	}

	int UDPConnect::SetBlocking()
	{
#ifdef _WIN32
		u_long mode = 0;
		return ioctlsocket(m_socket, FIONBIO, &mode);
#elif __linux__
		int fcntl(m_socket, F_SETFL, O_BLOCK);
#endif		
	}

	tachyon::Component *UDPConnect::Create()
	{
		return new UDPConnect();
	}

	int UDPConnect::bind(const char *host, unsigned short port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(port);

		int ret = ::bind(m_socket, reinterpret_cast<SOCKADDR *>(&addr), sizeof(addr));

		if (ret == SOCKET_ERROR)
		{
			std::cerr << "bind error " << NetworkUtil::Instance().GetLastError() << std::endl;
		}

		return ret;
	}

	// returns total bytes recv
	int UDPConnect::recvFrom(char *mesg, unsigned short *bytesRecv, unsigned short maxLen, sockaddr_in *addr, int flags)
	{
		socklen_t addrSize = sizeof(*addr);
		int ret = recvfrom(m_socket, mesg, maxLen, flags, reinterpret_cast<SOCKADDR *>(addr), &addrSize);

		if (ret == WSAEWOULDBLOCK)
		{
			return 0;
		}

		if (ret == SOCKET_ERROR)
		{
			std::cerr << "recvfrom error" << NetworkUtil::Instance().GetLastError() << std::endl;
			return ret;
		}

		*bytesRecv = ret;
		return ret;
	}

	// returns total bytes sent
	int UDPConnect::sendTo(sockaddr_in &addr, const char *mesg, unsigned short mesgLen, int flags)
	{
		int ret = sendto(m_socket, mesg, mesgLen, flags, reinterpret_cast<SOCKADDR *>(&addr), sizeof(addr));

		if (ret == WSAEWOULDBLOCK)
		{
			return 0;
		}

		if (ret == SOCKET_ERROR)
		{
			std::cerr << "sendto error " << NetworkUtil::Instance().GetLastError() << std::endl;
		}

		return ret;
	}
}


EXPORT_API bool InitLibrary(tachyon::MasterFactory *m)
{
	m->Register(NAME, msgudp::UDPConnect::Create);
	msgudp::NetworkUtil &nu = msgudp::NetworkUtil::Instance();

	return true;
}


