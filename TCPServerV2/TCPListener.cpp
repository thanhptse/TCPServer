#include "TCPListener.h"

TCPListener::TCPListener(std::string ipAddress, int port, MessageReceivedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), MessageReceived(handler)
{

}

TCPListener::~TCPListener() {
	CleanUp();
}

void TCPListener::Send(int clientSocket, std::string msg) {
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

bool TCPListener::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);

	return wsInit == 0;
}

void TCPListener::Run() {
	char buf[MAX_BUFFER_SIZE];

	while (true) {
		SOCKET listening = CreateSocket();
		if (listening == INVALID_SOCKET) {
			break;
		}

		SOCKET client = WaitForConnection(listening);
		if (client == INVALID_SOCKET) {
			continue;
		}

		closesocket(listening);

		int bytesReceived = 0;
		do {
			ZeroMemory(buf, MAX_BUFFER_SIZE);
			bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);
			if (bytesReceived > 0 && MessageReceived != NULL) {
				MessageReceived(this, client, std::string(buf, 0, bytesReceived));
			}
		} while (bytesReceived > 0);

		closesocket(client);
	}
}

void TCPListener::CleanUp() {
	WSACleanup();
}

SOCKET TCPListener::CreateSocket() {
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

	int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
	if (bindOk == SOCKET_ERROR) {
		return -1;
	}
	int listenOk = listen(listening, SOMAXCONN);
	if (listenOk == SOCKET_ERROR) {
		return -1;
	}

	return listening;

}

SOCKET TCPListener::WaitForConnection(SOCKET listening) {
	SOCKET client = accept(listening, NULL, NULL);
	return client;
}