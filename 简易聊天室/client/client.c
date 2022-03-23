#include"../tcpsocket/tcpsocket.h"
#include <string.h>
#pragma warning(disable : 4996)


int main()
{
	HANDLE hThread[2];
	unsigned threadID[2];

	
	bool recvfile(SOCKET server, char* recvbuf);
	init_Socket();

	SOCKET fd = createClientSocket("127.0.0.1");
	SEND(&fd);
	unsigned char recvbuf[BUFSIZ] = { 0 };
	while (true) {
		if (0 < recv(fd, recvbuf, BUFSIZ, 0))
		{
			printf("recv:%s\n", recvbuf);
			memset(recvbuf, 0, sizeof(recvbuf));
			hThread[0] = (HANDLE)_beginthreadex(NULL, 0, SEND, &fd, 0, &threadID[0]);
			hThread[1] = (HANDLE)_beginthreadex(NULL, 0, RECV, &fd, 0, &threadID[1]);

			CloseHandle(hThread[0]);
			CloseHandle(hThread[1]);


		}
	}
	


	closesocket(fd);
	close_Socket();
	printf("client------end------\n");
	getchar();
	return 0;
}

