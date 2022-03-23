#include "tcpsocket.h"
#include<stdio.h>
#pragma warning(disable : 4996)


BOOL init_Socket()
{
	//parm1:请求的socket版本 patm2:传出参数
	WSADATA wsadate;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsadate))
	{
		err("WSAStartup");
		return FALSE;
	}
	return TRUE;
}

BOOL close_Socket()
{
	if (0 !=WSACleanup())
	{
		err("WSACleanup");
		return FALSE;
	}
	return TRUE;
}

SOCKET createServerSocket()
{
	//1,创建空的Socket
	//parm1:af地址协议簇 ipv4 ipv6
	//parm2:type 传输协议类型 流式套接字 数据报
	//parm3:protoc
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		err("socket");
		return INVALID_SOCKET;
	}
	//2,给socket绑定ip地址和端口号 买卡才能打电话
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//和创建socket时必须一样
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = ADDR_ANY;//inet_addr"127.0.0.1"
	if (SOCKET_ERROR == bind(fd, &addr, sizeof(addr)))
	{
		err("bind");
		return FALSE;
	}

	//3,监听电话
	listen(fd, 10);

	return fd;
}

SOCKET createClientSocket(const char *ip)
{
	//1,创建空的Socket
	//parm1:af地址协议簇 ipv4 ipv6
	//parm2:type 传输协议类型 流式套接字 数据报
	//parm3:protoc
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		err("socket");
		return INVALID_SOCKET;
	}
	//2,与服务器建立连接
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;//和创建socket时必须一样
	addr.sin_port = htons(PORT);//大端存储和小端存储
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (INVALID_SOCKET == connect(fd, (struct sockaddr*)&addr, sizeof(addr)))
	{
		err("connect");
		return FALSE;
	}

	return fd;
}

//recv函数封装
unsigned __stdcall RECV(void* pfd)
{
	SOCKET fd;
	fd = *((SOCKET*)pfd);
	unsigned char recvbuf[BUFSIZ] = { 0 };
	unsigned char sendbuf[BUFSIZ] = { 0 };
	while (TRUE)
	{
		if (0 < recv(fd, recvbuf, BUFSIZ, 0))
		{
			printf("recv:%s\n", recvbuf);
			memset(recvbuf, 0, sizeof(recvbuf));
			//发送消息
		}
		
	}
}
//SEND函数封装
unsigned __stdcall SEND(void* pfd)
	{
		SOCKET fd;
		fd = *((SOCKET*)pfd);
		unsigned char recvbuf[BUFSIZ] = { 0 };
		unsigned char sendbuf[BUFSIZ] = { 0 };
		while (true)
		{
			printf("send>");
			gets_s(sendbuf, BUFSIZ);
			if (SOCKET_ERROR == send(fd, sendbuf, strlen(sendbuf), 0))
			{
				err("send");
			}
			memset(sendbuf, 0, sizeof(sendbuf));
		}

	}

