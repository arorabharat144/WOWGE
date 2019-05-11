#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace WOWGE;
using namespace WOWGE::Network;

void Network::Initialize()
{
	// Initialize Winsock version 2.2
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}
void Network::Terminate()
{
	// Shutdown Winsock
	WSACleanup();
}