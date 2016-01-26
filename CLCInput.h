//
// Created by Nils Daumann on 24.01.16.
//

#ifndef CHLOECONTROL_CLCINPUT_H
#define CHLOECONTROL_CLCINPUT_H

#include "hidapi.h"

namespace CLC
{
	class Input
	{
	public:
		struct StickData
		{
		public:
			float x;
			float y;
		};

		enum AxisControls
		{
			LeftStick,
			RightStick,
			LeftTrigger,
			RightTrigger
		};

		enum Button
		{
			Triangle = 0x80,
			Circle = 0x40,
			X = 0x20,
			Square = 0x10
		};

		Input();
		~Input();

		void Update();
		StickData GetStick(AxisControls axisControl);
		float GetTrigger(AxisControls axisControl);
		bool GetButton(Button button);

	private:
		hid_device *_device;
		char _leftStickX;
		char _leftStickY;
		char _rightStickX;
		char _rightStickY;
		unsigned char _leftTrigger;
		unsigned char _rightTrigger;
		unsigned char _buttons1;
	};
}

#endif //CHLOECONTROL_CLCINPUT_H
