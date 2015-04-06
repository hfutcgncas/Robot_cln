#pragma once

#define  _WINSOCK_DEPRECATED_NO_WARNINGS 

#include "stdio.h"
#include <iostream>
#include <io.h> //_access

#include <WS2tcpip.h>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class cUDPInterface
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	SOCKET sockClient;
	SOCKADDR_IN addrSrv;
	


public:
	char recvBuf[100];

	bool ComInit();
	void RecvData();
};
