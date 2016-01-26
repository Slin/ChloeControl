//
// Created by Nils Daumann on 24.01.16.
//

#include "CLCInput.h"
#include <cstdlib>
#include <iostream>

namespace CLC
{
	Input::Input() : _leftStickX(0), _leftStickY(0), _leftTrigger(0), _rightStickX(0), _rightStickY(0), _rightTrigger(0), _buttons1(0)
	{
		hid_init();

/*		hid_device_info *devs = hid_enumerate(0x0, 0x0);
		hid_device_info *cur_dev = devs;
		while(cur_dev)
		{
			printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
				   cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
			printf("\n");
			printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
			printf("  Product:      %ls\n", cur_dev->product_string);
			printf("\n");
			cur_dev = cur_dev->next;
		}
		hid_free_enumeration(devs);*/

		//PS4 controller
		_device = hid_open(0x54c, 0x5c4, NULL);

		hid_set_nonblocking(_device, 1);
	}

	Input::~Input()
	{
		hid_exit();
	}

	void Input::Update()
	{
		unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*100);
		while(hid_read(_device, data, 100) > 0)
		{
			//Basic PS4 Controller input data via USB
			if(data[0] == 0x01)
			{
				_leftStickX = data[1]-128;
				_leftStickY = data[2]-128;
				_rightStickX = data[3]-128;
				_rightStickY = data[4]-128;

				_leftTrigger = data[8];
				_rightTrigger = data[9];

				_buttons1 = data[5];
			}
		}
		free(data);
	}

	Input::StickData Input::GetStick(AxisControls axisControl)
	{
		StickData stickData;

		if(axisControl == AxisControls::LeftStick)
		{
			stickData.x = _leftStickX/128.0f;
			stickData.y = _leftStickY/128.0f;
		}
		else if(axisControl == AxisControls::RightStick)
		{
			stickData.x = _rightStickX/128.0f;
			stickData.y = _rightStickY/128.0f;
		}

		return stickData;
	}

	float Input::GetTrigger(AxisControls axisControl)
	{
		if(axisControl == AxisControls::LeftTrigger)
		{
			return _leftTrigger/255.0f;
		}
		else if(axisControl == AxisControls::RightTrigger)
		{
			return _rightTrigger/255.0f;
		}
	}

	bool Input::GetButton(Button button)
	{
		return (_buttons1 & button);
	}
}
