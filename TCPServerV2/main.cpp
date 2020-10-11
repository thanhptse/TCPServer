#include <iostream>
#include <string>

#include "TCPListener.h"

void Listener_MessageReceived(TCPListener* listener, int client, std::string msg);

void main() {
	TCPListener server("127.0.0.1", 54000, Listener_MessageReceived);

	if (server.Init()) {
		server.Run();
	}
}

void Listener_MessageReceived(TCPListener *listener, int client, std::string msg) {
	listener->Send(client, msg);
}
