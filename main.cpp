#include <iostream>
#include <cmath>
#include "CLCNetwork.h"
#include "CLCInput.h"

int main()
{
	CLC::Network network;
	CLC::Input input;

	float speed = 0.0f;

	while(!input.GetButton(CLC::Input::Button::Square))
	{
		while(!input.GetButton(CLC::Input::Button::Triangle) && !input.GetButton(CLC::Input::Button::Square))
		{
			input.Update();
		}

		network.SendString(std::string("start"));

		while(!input.GetButton(CLC::Input::Button::X) && !input.GetButton(CLC::Input::Button::Square))
		{
			input.Update();

			speed = input.GetTrigger(CLC::Input::AxisControls::RightTrigger)*1.0f;

			std::string inputData("up:");
			inputData += std::to_string(speed);
			network.SendString(inputData);

			std::cout << inputData << std::endl;

			network.Update();
			usleep(1000);
		}

		speed = 0.0f;
		network.SendString(std::string("stop"));
		network.Update();
	}

	return 0;
}