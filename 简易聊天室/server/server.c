#include"../tcpsocket/tcpsocket.h"
#include <string.h>
#pragma warning(disable : 4996)


int main()
{
	HANDLE hThread[2];
	unsigned threadID[2];

	init_Socket();
	SOCKET serfd = createServerSocket();
	printf("wait client connect...\n");
	//如果有客户端请求连接
	SOCKET clifd = accept(serfd, NULL, NULL);
	if (INVALID_SOCKET == clifd)
	{
		err("accept");
	}
	else
	{
		printf("connect success!\n");
	}
	unsigned char recvbuf[BUFSIZ] = { 0 };
	while (true) {
		if (0 < recv(clifd, recvbuf, BUFSIZ, 0))
		{
			printf("recv:%s\n", recvbuf);
			memset(recvbuf, 0, sizeof(recvbuf));
			hThread[0] = (HANDLE)_beginthreadex(NULL, 0, SEND, &clifd, 0, &threadID[0]);
			hThread[1] = (HANDLE)_beginthreadex(NULL, 0, RECV, &clifd, 0, &threadID[1]);

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);


		}
	}
	closesocket(clifd);
	closesocket(serfd);
	close_Socket();
	printf("server--------end------\n");
	return 0;
}
