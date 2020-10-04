#include <iostream>
#include  <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

void main() {
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		std::cerr << "Cant init winsock !" << std::endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "Cant create a socket" << std::endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; // Client's name
	char service[NI_MAXSERV]; // Port the client is connect on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << "  connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "connected on port " << ntohs(client.sin_port) << std::endl;
	}

	closesocket(listening);

	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) {
			std::cerr << "Error in recv()." << std::endl;
			break;
		}

		std::cout << std::string(buf, 0, bytesReceived) << std::endl;

		// Send message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	closesocket(clientSocket);

	WSACleanup();

	std::system("pause");
}