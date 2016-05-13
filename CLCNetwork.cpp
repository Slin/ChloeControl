//
// Created by Nils Daumann on 24.01.16.
//

#include "CLCNetwork.h"
#include <iostream>

namespace CLC
{
	Network::Network() : _isConnected(false)
	{
		if (enet_initialize () != 0)
		{
			std::cout << "An error occurred while initializing ENet" << std::endl;
			return;
		}

		_client = enet_host_create(nullptr /* the address to bind the server host to */,
								   1      /* allow up to 1 clients and/or outgoing connections */,
								   2      /* allow up to 2 channels to be used, 0 and 1 */,
								   0      /* assume any amount of incoming bandwidth */,
								   0      /* assume any amount of outgoing bandwidth */);
		if(!_client)
		{
			std::cout << "An error occurred while trying to create an ENet client host." << std::endl;
			return;
		}

		ENetAddress address;
		enet_address_set_host(&address, "chloe.local");
		address.port = 1234;

		/* Initiate the connection, allocating the two channels 0 and 1. */
		_peer = enet_host_connect(_client, &address, 2, 0);
		if(!_peer)
		{
			std::cout << "No available peers for initiating an ENet connection." << std::endl;
			return;
		}

		/* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service(_client, &_event, 5000) > 0 &&
			_event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connected to chloe.local!" << std::endl;
			_isConnected = true;
		}
		else
		{
			/* Either the 5 seconds are up or a disconnect event was */
			/* received. Reset the peer in the event the 5 seconds   */
			/* had run out without any significant event.            */
			enet_peer_reset(_peer);
			std::cout << "Connection to chloe.local failed." << std::endl;
			_isConnected = false;
		}
	}

	Network::~Network()
	{
		enet_peer_disconnect(_peer, 0);

		/* Allow up to 3 seconds for the disconnect to succeed
		 * and drop any packets received packets.
		 */
		while(enet_host_service(_client, &_event, 3000))
		{
			switch(_event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(_event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "Disconnected from chloe.local." << std::endl;
					return;
				default:
					break;
			}
		}

		enet_peer_reset(_peer);

		enet_host_destroy(_client);
		enet_deinitialize();
	}

	void Network::Update()
	{
		if(!_isConnected)
			return;

		while(enet_host_service(_client, &_event, 0))
		{
			switch(_event.type)
			{
				case ENET_EVENT_TYPE_RECEIVE:
				{
					std::string packageData(reinterpret_cast<char *>(_event.packet->data));
					enet_packet_destroy(_event.packet);

					if(packageData.find("currentorientation:") == 0)
					{
						std::string values = packageData.substr(19);
						float x = std::stof(values.substr(0, values.find(",")));
						values = values.substr(values.find(",")+1);
						float y = std::stof(values.substr(0, values.find(",")));
						values = values.substr(values.find(",")+1);
						float z = std::stof(values.substr(0, values.find(",")));

						orientation[0] = x;
						orientation[1] = y;
						orientation[2] = z;
					}
					break;
				}

				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "Disconnected from chloe.local. Kicked from server!" << std::endl;
					_isConnected = false;
					break;

				default:
					break;
			}
		}
	}

	void Network::SendString(const std::string &string)
	{
		ENetPacket * packet = enet_packet_create(string.c_str(), string.length() + 1, 0);
		enet_peer_send(_peer, 0, packet);
	}
}