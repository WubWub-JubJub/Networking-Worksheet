#include "Client.h"

void Client::connect()
{
	if (socket == nullptr)
	{
		socket = std::make_unique < sf::TcpSocket>();
	}

	if (socket->connect(*sf::IpAddress::fromString("127.0.0.1"), 53000) == sf::Socket::Status::Done)
	{
		std::cout << "Youre connected!" << std::endl;
		connected = true;
	}
	else
	{
		std::cout << "failed to connect to 127.0.0.1::53000" << std::endl;
	}
}

void Client::input()
{
	while (running)
	{
		std::string str;
		std::getline(std::cin, str);
		std::cin.clear();

		if (connected)
		{
			socket->send(reinterpret_cast<char*>(str.data()), str.length());
		}
	}
}

void Client::run()
{
	running = true;
	std::thread input_thread([&] {input();});
	input_thread.detach();

	while (running)
	{
		char static_buffer[1028];
		while (connected)
		{
			std::size_t received;
			sf::Socket::Status status = socket->receive(static_buffer, 1028, received);

			if (status == sf::Socket::Status::Disconnected)
			{
				connected = false;
				std::cout << "Clean disconnected" << std::endl;
				socket->disconnect();
				break;
			}
			else
			{
				if (received < 1028) static_buffer[received] = '\0';
				std::cout << static_buffer << std::endl;
			}
		}
	}
}