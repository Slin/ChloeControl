#include <iostream>
#include <cmath>
#include "CLCNetwork.h"
#include "CLCInput.h"

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <OpenGL/glu.h>

void InitOpenGL()
{
	// Set color and depth clear value
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 500.f);
}

void Draw(float rotx, float roty, float rotz)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Apply some transformations for the cube
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -200.f);

	glRotatef(rotx+45.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(roty, 1.0f, 0.0f, 0.0f);
	glRotatef(rotz, 0.0f, 0.0f, 1.0f);


	//Draw a cube
	glBegin(GL_QUADS);//draw some squares
	glColor3i(0,1,1);
	glVertex3f(-50.0f, -50.0f, -50.0f);
	glVertex3f(-50.0f,  50.0f, -50.0f);
	glVertex3f( 50.0f,  50.0f, -50.0f);
	glVertex3f( 50.0f, -50.0f, -50.0f);

	glColor3f(0,0,1);
	glVertex3f(-50.f, -50.f, 50.f);
	glVertex3f(-50.f,  50.f, 50.f);
	glVertex3f( 50.f,  50.f, 50.f);
	glVertex3f( 50.f, -50.f, 50.f);

	glColor3f(1,0,1);
	glVertex3f(-50.f, -50.f, -50.f);
	glVertex3f(-50.f,  50.f, -50.f);
	glVertex3f(-50.f,  50.f,  50.f);
	glVertex3f(-50.f, -50.f,  50.f);

	glColor3f(0,1,0);
	glVertex3f(50.f, -50.f, -50.f);
	glVertex3f(50.f,  50.f, -50.f);
	glVertex3f(50.f,  50.f,  50.f);
	glVertex3f(50.f, -50.f,  50.f);

	glColor3f(1,1,0);
	glVertex3f(-50.f, -50.f,  50.f);
	glVertex3f(-50.f, -50.f, -50.f);
	glVertex3f( 50.f, -50.f, -50.f);
	glVertex3f( 50.f, -50.f,  50.f);

	glColor3f(1,0,0);
	glVertex3f(-50.f, 50.f,  50.f);
	glVertex3f(-50.f, 50.f, -50.f);
	glVertex3f( 50.f, 50.f, -50.f);
	glVertex3f( 50.f, 50.f,  50.f);

	glEnd();
}

int main()
{
	CLC::Network network;
	CLC::Input input;

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 2;
	settings.minorVersion = 1;
	sf::Window App(sf::VideoMode(800, 600, 32), "Chloe Control", sf::Style::Default, settings);
	InitOpenGL();

	float speed = 0.0f;

	while(!input.GetButton(CLC::Input::Button::Square) && App.isOpen())
	{
		while(!input.GetButton(CLC::Input::Button::Triangle) && !input.GetButton(CLC::Input::Button::Square) && App.isOpen())
		{
			input.Update();
		}

		network.SendString(std::string("start"));

		while(!input.GetButton(CLC::Input::Button::X) && !input.GetButton(CLC::Input::Button::Square) && App.isOpen())
		{
			sf::Event Event;
			while (App.pollEvent(Event))
			{
				// Close window : exit
				if (Event.type == sf::Event::Closed)
					App.close();

				// Escape key : exit
				if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
					App.close();
			}

			input.Update();

			if(input.GetButton(CLC::Input::Button::Circle))
			{
				network.SendString("makelevel");
			}

			speed = input.GetTrigger(CLC::Input::AxisControls::RightTrigger)*0.5f;

			std::string inputData("up:");
			inputData += std::to_string(speed);
			network.SendString(inputData);

			CLC::Input::StickData axisLeft = input.GetStick(CLC::Input::AxisControls::LeftStick);
			if(fabsf(axisLeft.x)+fabsf(axisLeft.y) > 0.1f)
			{
				inputData = "targetorientation:";
				inputData += std::to_string(0.0f);
				inputData += ",";
				inputData += std::to_string(axisLeft.y*0.1f);
				inputData += ",";
				inputData += std::to_string(axisLeft.x*0.1f);
				network.SendString(inputData);
			}

			network.Update();

			Draw(network.orientation[0], network.orientation[1], network.orientation[2]);
			App.display();

			usleep(1000);
		}

		speed = 0.0f;
		network.SendString(std::string("stop"));
		network.Update();
	}

	return 0;
}
