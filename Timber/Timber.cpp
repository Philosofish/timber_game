// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream>
#include <SFML\Graphics.hpp>

using namespace sf;

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player / branch
// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber", Style::Fullscreen);
	
	// Draw some text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setString("Press enter to Start!");
	scoreText.setString("Score = 0");

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	messageText.setFillColor(Color::Red);
	scoreText.setFillColor(Color::Red);

	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(	textRect.left +
							textRect.width / 2.0f,
							textRect.top +
							textRect.height / 2.0f
						);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Tree
	Texture treeBackground;
	treeBackground.loadFromFile("graphics/tree.png");

	Sprite spriteTree;
	spriteTree.setTexture(treeBackground);
	spriteTree.setPosition(810, 0);

	// Bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool beeActive = false;
	float beeSpeed = 0.0f;

	// Clouds 
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 800);
	spriteCloud2.setPosition(0, 800);
	spriteCloud3.setPosition(0, 800);

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Control time
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWitdth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWitdth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWitdth / 2, 980);

	Time gameTimeTotal;
	float timeRemaning = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWitdth / timeRemaning;


	bool paused = true;

	// Prepare 6 branches

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Setthe sprite´s origin to dead center
		// We can then spin it round without chaning its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 702);

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log releted vars
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}
		// Player input
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score
			score = 0;
			timeRemaning = 5;

			// Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				// Add to the amount of time remaining
				timeRemaning += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
			}
			// Handle the cursor key

		}

		if (!paused)
		{
			// Mesure time
			Time dt = clock.restart();

			// Substract from the amount of time remaining
			timeRemaning -= dt.asSeconds();
			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaning, timeBarHeight));

			if (timeRemaning <= 0.0f)
			{
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}

			// Setup the bee
			if (!beeActive)
			{
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// How hight is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				// Move the bee
				spriteBee.setPosition(	spriteBee.getPosition().x -
										(beeSpeed * dt.asSeconds()),
										spriteBee.getPosition().y
									);
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}

			// Cloud1
			if (!cloud1Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How height is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				// Move the cloud
				spriteCloud1.setPosition(spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y
				);
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}

			// Cloud2
			if (!cloud2Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How height is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				// Move the cloud
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// Cloud3
			if (!cloud3Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How height is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{

				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX  * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// Has the log reached the right hand side?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

		}
		// Draw scene
		window.clear();

		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(timeBar);
		window.draw(spriteBee);
		
		window.draw(spriteTree);

		// Draw branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// Draw the player
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);

		window.draw(scoreText);
		if (paused)
		{
			// Draw the score
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
		
	}

	return 0;
}

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE

	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}