#pragma once

#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER_SIZE (49152)


class TCPListener;

typedef void(*MessageReceivedHandler)(TCPListener* listener, int socketId, std::string msg);

class TCPListener {
public:
	TCPListener(std::string ipAddress, int port, MessageReceivedHandler handler);

	~TCPListener();

	void Send(int clientSocket, std::string msg);

	bool Init();

	void Run();

	void CleanUp();

private:
	SOCKET CreateSocket();

	SOCKET WaitForConnection(SOCKET listening);

	// Adderss of the server
	std::string					m_ipAddress;

	// Listening port
	int							m_port;

	// Callback function to handle message received
	MessageReceivedHandler		MessageReceived;
};