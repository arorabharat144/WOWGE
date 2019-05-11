#include "Precompiled.h"
#include "TCPSocket.h"

using namespace WOWGE;
using namespace WOWGE::Network;

TCPSocket::TCPSocket()
{

}
TCPSocket::~TCPSocket()
{

}

bool TCPSocket::Open()
{
	return true;
}
void TCPSocket::Close()
{

}

bool TCPSocket::Connect(const SocketAddress& address)
{
	return true;
}
bool TCPSocket::TCPSocket::Bind(const SocketAddress& fromAddress)
{
	return true;
}
bool TCPSocket::Listen(int backLog)
{
	return true;
}
TCPSocket* TCPSocket::Accept(SocketAddress& fromAddress)
{
	return nullptr;
}

int TCPSocket::Send(const void* buffer, int len)
{
	return 0;
}
int TCPSocket::Recieve(void* buffer, int len)
{
	return 0;
}