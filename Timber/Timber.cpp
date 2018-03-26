// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML\Graphics.hpp>

using namespace sf;

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber", Style::Fullscreen);
	
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	while (window.isOpen())
	{
		// Player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Update the scene
	
		// Draw scene
		window.clear();

		window.draw(spriteBackground);
		
		// Show everything we just drew
		window.display();

	}

    return 0;
}

