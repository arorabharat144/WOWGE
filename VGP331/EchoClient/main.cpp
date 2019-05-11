#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	std::string hostAddress{ "127.0.0.1" }; //localhost
	uint16_t port{ 8888 };

	//Check for arguments
	if (argc >= 2)
	{
		hostAddress = argv[1];
	}
	if (argc >= 3)
	{
		port = static_cast<uint16_t>(atoi(argv[2]));
	}

	//Initialize Winsock version 2.2
	WSAData wsaData;
	int result{ WSAStartup(MAKEWORD(2, 2), &wsaData) };
	if (result != 0)
	{
		std::cout << "WSAStartup failed!\n";
		return -1;
	}

	//Create a socket using TCP/IP
	SOCKET mySocket{ socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) };
	if (mySocket == INVALID_SOCKET)
	{
		std::cout << "Socket failed!\n";
		return -1;
	}

	//Resolve host using address
	in_addr iaHost;
	//inet_pton
	iaHost.s_addr = inet_addr(hostAddress.c_str());
	HOSTENT* hostEntry{ gethostbyaddr((const char*)&iaHost, sizeof(iaHost), AF_INET) };
	if (hostEntry == nullptr)
	{
		std::cout << "gethostbyaddr failed!\n";
		return -1;
	}

	//Fill host address information and connect
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	serverInfo.sin_port = htons(port);

	int result{ connect(mySocket, (LPSOCKADDR)&serverInfo, sizeof(serverInfo)) };
	if (result == SOCKET_ERROR)
	{
		std::cout << "connect failed!\n";
		return -1;
	}

	std::cout << "Connected to server!\n";
	std::string message{ "hello!" };

	std::cout << "Sending: " << message << "\n";

	//Send user message
	int bytesSent{ send(mySocket, message.c_str(), static_cast<int>(message.size()), 0) };
	if (bytesSent == SOCKET_ERROR)
	{
		std::cout << "send failed!\n";
		return -1;
	}

	//Wait for server response **blocking**
	char buffer[1024];
	int bytesReceived{ recv(mySocket, buffer, sizeof(buffer), 0) };
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

	//Close all sockets
	closesocket(mySocket);

	//Shutdown Winsock
	WSACleanup();

	system("pause");
	return 0;
}