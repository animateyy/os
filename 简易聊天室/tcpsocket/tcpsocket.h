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


//�������
BOOL init_Socket();

//�ر������
BOOL close_Socket();
//����������socket
SOCKET createServerSocket();
//�����ͻ���socket
SOCKET createClientSocket(const char *ip);
//�����ļ�����
unsigned __stdcall RECV(void* pfd);
unsigned __stdcall SEND(void* pfd);


#endif

