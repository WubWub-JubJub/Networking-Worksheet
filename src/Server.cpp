#include "Server.h"

void Server::init()
{
	if (listener == nullptr)
	{
		listener = std::make_unique<sf::TcpListener>();
	}
	if (listener->listen(53000) != sf::Socket::Status::Done)
	{
		std::cout << "Couldnt bind listener to port 53000" << std::endl;
	}
}

void Server::run()
{
	while (running)
	{
		sf::TcpSocket& c_sock = *connections.emplace_back(std::make_unique<sf::TcpSocket>());

		if (listener->accept(c_sock) != sf::Socket::Status::Done)
		{
			connections.pop_back();
			std::cout << "Connection failed on server side" << std::endl;
			continue;
		}
		std::cout << "Client connected at" << c_sock.getRemotePort() << std::endl;
	}
}