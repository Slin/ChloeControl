//
// Created by Nils Daumann on 24.01.16.
//

#ifndef CHLOECONTROL_CLCNETWORK_H
#define CHLOECONTROL_CLCNETWORK_H

#include "enet/enet.h"
#include <string>

namespace CLC
{
	class Network
	{
	public:
		Network();
		~Network();

		void Update();
		void SendString(const std::string &string);

		float orientation[3];

	private:
		ENetHost *_client;
		ENetEvent _event;
		ENetPeer *_peer;

		bool _isConnected;
	};
}

#endif //CHLOECONTROL_CLCNETWORK_H
