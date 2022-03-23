#ifndef  _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<stdbool.h>
#include<stdio.h>
#include <windows.h>
#include <process.h>


#define err(errMsg) printf("[line:%d]%s failed code %d" ,__LINE__,errMsg, WSAGetLastError());
#define PORT 8888


//打开网络库
BOOL init_Socket();

//关闭网络库
BOOL close_Socket();
//创建服务器socket
SOCKET createServerSocket();
//创建客户端socket
SOCKET createClientSocket(const char *ip);
//发送文件函数
unsigned __stdcall RECV(void* pfd);
unsigned __stdcall SEND(void* pfd);


#endif

