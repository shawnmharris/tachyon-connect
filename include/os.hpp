#pragma once

#include <stdio.h>
#include <string>
#include <cstring>
#include <memory>
#include <exception>

#ifdef _WIN32
#include <winsock2.h>
#include <wininet.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet")
#elif __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
typedef __int64_t SOCKET;
#define SOCKET_ERROR -1
#define SOCKADDR sockaddr
#define INVALID_SOCKET  (SOCKET)(~0)
#define closesocket close
#define WSAEWOULDBLOCK EWOULDBLOCK
#endif





