#include "Precompiled.h"
#include "SocketAddress.h"

using namespace WOWGE;
using namespace WOWGE::Network;

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIPv4() = INADDR_ANY;
	GetPort() = 0;

}
SocketAddress::SocketAddress(const char*  host, uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetPort() = htons(port);
	inet_pton(AF_INET, host, &GetIPv4());
}
SocketAddress::SocketAddress(uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIPv4() = INADDR_ANY;
	GetPort() = htons(port);
}

std::string SocketAddress::GetHost() const
{
	char buffer[INET_ADDRSTRLEN];
	if (!inet_ntop(AF_INET, &(GetAsSockAddrIn()->sin_addr), buffer, INET_ADDRSTRLEN))
	{
		return "";
	}
	return buffer;
}