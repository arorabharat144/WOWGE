#ifndef INCLUDED_NETWORK_UDPSOCKET_H
#define INCLUDED_NETWORK_UDPSOCKET_H

#include "SocketAddress.h"

namespace WOWGE 
{
namespace Network
{

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();

	bool Open();
	void Close();

	bool Bind(const SocketAddress& fromAddress);
	bool SetBroadcast(bool broadcast);

	int SendTo(const void* buffer, int len, const SocketAddress& toAddress);
	int RecieveFrom(void* buffer, int len, SocketAddress& fromAddress);

private:
	UDPSocket(const UDPSocket&) = delete;
	UDPSocket& operator=(const UDPSocket&) = delete;

	SOCKET mSocket;

};

} //namespace Network
} //namespace WOWGE

#endif // INCLUDED_NETWORK_UDPSOCKET_H