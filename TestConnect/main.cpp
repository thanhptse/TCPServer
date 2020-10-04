#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void main() {
	std::string ipAddress = "127.0.0.1";
	int port = 54000;

	// Init winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		std::cerr << "Cant start Winsock, err: " << wsResult << std::endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Cant create socket, err: " << WSAGetLastError() << std::endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		std::cerr << "Cant connect to server, err: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	char buf[4096];
	std::string userInput;

	do {
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0) {
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0) {
					std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
				}
			}
		}
	} while (userInput.size() > 0);

	closesocket(sock);
	WSACleanup();
}