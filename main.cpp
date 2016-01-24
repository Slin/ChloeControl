#include <iostream>
#include "CLCNetwork.h"

int main()
{
	CLC::Network network;

	network.SendString(std::string("start"));

	int i = 0;
	while(i < 10000)
	{
		std::string inputData("up:");
		inputData += std::to_string(0.1f);
		network.SendString(inputData);

		network.Update();
		i++;
		usleep(1000);
	}

	network.SendString(std::string("stop"));
	network.Update();

	return 0;
}