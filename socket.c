#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include "socket_handler.h"
#pragma comment(lib, "Ws2_32.lib")
#define MAX_MESSAGES 1000
SOCKET sock;
char buffer[2048];
char* messageQueue[MAX_MESSAGES];
int messageCount = 0;
// 接收数据并存储到消息队列
void receive_data() {
	u_long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	
	int result = recv(sock, buffer, sizeof(buffer) - 1, 0);
	if (result > 0) {
		buffer[result] = '\0';
		if (messageCount < MAX_MESSAGES) {
			messageQueue[messageCount] = strdup(buffer);
			messageCount++;
		}
	} else if (result == 0) {
		// 连接关闭
	} else {
		int error = WSAGetLastError();
		if (error != WSAEWOULDBLOCK) {
			// 处理错误
		}
	}
}

// 初始化套接字并连接到服务器
__declspec(dllexport) int init_socket(const char* ip, int port) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
	
	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) != 0) {
		closesocket(sock);
		WSACleanup();
		return -1;
	}
	
	return 0;
}

// 发送数据
__declspec(dllexport) int send_data(const char* data) {
	int result = send(sock, data, strlen(data), 0);
	return result == SOCKET_ERROR ? -1 : 0; 
}

// 获取并移除最早的消息
__declspec(dllexport) const char* get_message() {
	receive_data();
	if (messageCount > 0) {
		const char* msg = messageQueue[0];
		for (int i = 1; i < messageCount; i++) {
			messageQueue[i - 1] = messageQueue[i];
		}
		messageCount--;
		return msg;
	}
	
	return "";
}

// 关闭套接字
__declspec(dllexport) void close_socket() {
	closesocket(sock);
	WSACleanup();
	for (int i = 0; i < messageCount; i++) {
		free(messageQueue[i]);
	}
	messageCount = 0;
}
