#include "cUDPInterface.h"





bool cUDPInterface::ComInit()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}
	sockClient = socket(AF_INET, SOCK_DGRAM, 0);

	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	return true;
}

void cUDPInterface::RecvData()
{
	int len = sizeof(SOCKADDR);
	recvfrom(sockClient, recvBuf, 100, 0, (SOCKADDR*)&addrSrv, &len);
	printf("%s\n", recvBuf);
}
