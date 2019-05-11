#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	uint16_t port{ 8888 };
	//Initialize Winsock version 2.2
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//Create a socket using TCP/IP
	SOCKET listener{ socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) };

	//Fill server address information
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);

	std::cout << "Listening on port " << port << "\n";

	//Bind the address to our listener socket and listen for connection
	bind(listener, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));

	listen(listener, 10);

	//Serve forever...
	while (true)
	{
		//Accept any incoming client connection **Blocking**
		SOCKET client{ accept(listener, nullptr, nullptr) };

		char buffer[1024];
		int bytesReceived{ recv(client, buffer, sizeof(buffer),0) };
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cout << "recv failed\n";
			return -1;
		}
		if (bytesReceived == 0)
		{
			std::cout << "Connection closed\n";
		}
		else
		{
			buffer[bytesReceived] = '\0';
			std::cout << "Received: " << buffer << "\n";
		}

		std::cout << "Sending: " << buffer << "\n";

		//Send client message
		int bytesSent{ send(client, buffer, sizeof(buffer) - 1, 0) };
		
		closesocket(client);
	}

	//Close all sockets
	closesocket(listener);

	//Shutdown Winsock
	WSACleanup();

	system("pause");
	return 0;
}