#include <iostream>
#include  <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

// 1: Socket()
// 2: Bind()
// 3: Listen()
// 4: Accept() - Called by server
// 5: Connect() - Called by Client
// 6: Read() / Send() / Sendto()
// 7: Write() / Recv() / Recvfrom()
// 8: Close()
// Socket programming usually have 8 step like that
void main() {
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		std::cerr << "Cant init winsock !" << std::endl;
		return;
	}

	// 1: Socket() - A connection Endpoint
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM for TCP, SOCK_DGRAM for UDP
	//  function socket(...) return a socket description  on success and -1 on an error
	// so that why have have this if
	if (listening == INVALID_SOCKET) {
		std::cerr << "Cant create a socket" << std::endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET; // AF_INET and PF_INET are the same thing
	hint.sin_port = htons(54000); // Host Byte Order to Network Byte Order (short)
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // All IP address available for an interface

	// 2: Bind() - Attaching to an IP and port
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// 3:Listen() - Wait for a connection
	listen(listening, SOMAXCONN); // We can replace SOMAXCONN to other number like 10, so that we will allow a maximum of 10 connections to be in pending state

	sockaddr_in client;
	int clientSize = sizeof(client);

	// 4: Accept() - A new connection
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize); // accept new connections from new clients try to connect to the server

	char host[NI_MAXHOST]; // Client's name
	char service[NI_MAXSERV]; // Port the client is connect on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		std::cout << host << "  connected on port " << service << std::endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); //inet_ntop extends the inet_ntoa(change ip address format from Binary to Ascii dotted)
		std::cout << host << "connected on port " << ntohs(client.sin_port) << std::endl; // ntohs stand for Network Byte Order to Host Byte Order (short)
	}

	closesocket(listening);

	char buf[4096];

	// 6,7: Send / Recv - Data(receive data from `client` and send back something to their)
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

	// 8: Close() - Bye byeeee
	closesocket(clientSocket);

	WSACleanup();

	std::system("pause");
}