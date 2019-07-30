#pragma once

namespace msgudp
{
	class NetworkUtil
	{
	public:
		static NetworkUtil &Instance()
		{
			static NetworkUtil instance;
			return instance;
		}	
		
		~NetworkUtil()
		{
			Cleanup();
		}
		
		bool IsNetworkInit()
		{
#ifdef _WIN32
			SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s == INVALID_SOCKET && WSAGetLastError() == WSANOTINITIALISED) 
			{
				return false;
			}

			closesocket(s);
#endif
			return true;			
		}

		int GetLastError()
		{
#ifdef _WIN32
			return WSAGetLastError();
#else
			return errno;
#endif			
		}
		
protected:		
		NetworkUtil() { Init(); }
		
		bool Init()
		{
#ifdef _WIN32
			WSADATA wsaData;
			WORD version = MAKEWORD(2, 2);

			if (WSAStartup(version, &wsaData) != 0)
			{
				std::cerr << "winsock initialization error" << std::endl;
				return false;
			}
#endif
			return true;
		}
		
		
		void Cleanup()
		{
#ifdef _WIN32
			WSACleanup();
#endif					
		}
	};
}

