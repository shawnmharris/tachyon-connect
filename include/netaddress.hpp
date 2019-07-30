#pragma once

namespace msgudp
{
	class NetAddress
	{
	public:
		NetAddress() : m_error(true)
		{
		}

		NetAddress(const char *host, unsigned short port) : m_host(host), m_port(port)
		{
			m_error = false;
			memset(&m_address, 0, sizeof(m_address));
			m_address.sin_port = htons(port);
			m_address.sin_family = AF_INET;
			inet_pton(AF_INET, host, &m_address.sin_addr);
		}

		NetAddress(sockaddr_in a) : m_address(a)
		{
			m_error = false;
			char ip[40];
			memset(ip, 0, 40);
			inet_ntop(AF_INET, &m_address.sin_addr, ip, 256);
			m_host = ip;
			m_port = ntohs(m_address.sin_port);
		}

		NetAddress &operator=(const NetAddress &other)
		{
			m_host = other.m_host;
			m_port = other.m_port;
			m_address = other.m_address;
			m_error = other.m_error;
			return *this;
		}

		std::string Host() const { return m_host; }
		unsigned short Port() const { return m_port; }
		sockaddr_in Addr() const { return m_address; }
		bool isValid() const { return !m_error; }

	protected:
		std::string m_host;
		unsigned short m_port;
		sockaddr_in m_address;
		bool m_error;
	};
}