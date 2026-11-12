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

		workers.emplace_back([&] {
			listen(c_sock);
			std::cout << "Disconnect detected" << std::endl;
			std::lock_guard<std::mutex> lck(mutex);

			for (int i = 0; i < connections.size(); i++)
			{
				if (connections[i]->getRemotePort() == c_sock.getRemotePort())
				{
					connections.erase(std::next(connections.begin(), i));
					break;
				}
			}
			});
	}
}

void Server::input()
{
	while (running)
	{
		std::string str;
		std::getline(std::cin, str);
		std::cin.clear();
		std::lock_guard<std::mutex>lck(mutex);
		for (std::unique_ptr<sf::TcpSocket>& connection : connections)
		{
			connection->send(reinterpret_cast<char*>(str.data()), str.length());
		}
	}
}

void Server::listen(sf::TcpSocket& c_socket)
{
	bool continue_receiving = true;

	char data[1028];
	std::size_t received;

	while (continue_receiving)
	{
		sf::Socket::Status status = c_socket.receive(data, 1028, received);

		if (status == sf::Socket::Status::Disconnected)
		{
			continue_receiving = false;
			std::cout << "Disconnected " << std::endl;
		}

		if (received < 1028) { data[received] = '\0'; }	
		relay(data, c_socket);

		std::cout << reinterpret_cast<char*>(data) << std::endl;
	}
	c_socket.disconnect();
}

void Server::relay(std::string buffer, sf::TcpSocket& c_socket)
{
	char* message = reinterpret_cast<char*>(buffer.data());
	std::lock_guard<std::mutex> lck(mutex);

	for (std::unique_ptr<sf::TcpSocket>& connection : connections)
	{
		if (connection->getRemotePort() != c_socket.getRemotePort())
		{
			connection->send(message, buffer.size());
		}
	}
}