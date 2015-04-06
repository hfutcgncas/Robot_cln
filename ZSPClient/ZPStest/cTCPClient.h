//InitSock.h
#pragma once

#include <WinSock2.h>
#pragma comment(lib,"WS2_32")

class CInitSock
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVER = 2)
	{
		//初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVER);
		if (::WSAStartup(sockVersion, &wsaData) != 0)
			exit(0);
	}
	~CInitSock()
	{
		::WSACleanup();
	}
};

class cTCPClient
{
private:
	u_short port;
	sockaddr_in servAddr;
public:
	SOCKET s;

	char buff[256];

public:
	cTCPClient(u_short Port, char* IP)
	{
		port = Port;
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(port);

		servAddr.sin_addr.S_un.S_addr = inet_addr(IP);

		s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	bool ConectToServer()
	{
		if (connect(s, (sockaddr *)&servAddr, sizeof(servAddr)) == -1)
		{
			printf("Failed connect()\n");
			return 0;
		}
		return 1;
	}
	bool RecvDataFromServer()
	{
		int nRev = ::recv(s, buff, 256, 0);
		if (nRev>0)
		{
			buff[nRev] = '\0';
			//printf("接受到数据:%s", buff);
			return true;
		}
		return false;
	}
	void CloseTCPClient()
	{
		::closesocket(s);
	}

};