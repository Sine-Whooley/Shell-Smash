//Name: Síne Whooley
//Student Number: C00271403
//Date: 10/03/22
//------------------
//Project: Shell Smash Project
//------------------



#include "Game.h"
#include "Vector2.h"
#include <iostream>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "Shell Smash" },
	m_exitGame{false} //when true game will exit
{
	setUpShell();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type)				// window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type)				//user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMousePressed(newEvent);
		}
		if (sf::Event::MouseMoved == newEvent.type)
		{
			processMouseMove(newEvent);
		}
		if (sf::Event::MouseButtonReleased == newEvent.type)
		{
			processMouseRelease(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processMousePressed(sf::Event t_event)
{
	if (m_readyToFire)							//Only runs if readyToFire is true, if its false, it wont run
	{
		sf::Vertex point;
		point.color = sf::Color::White;

		m_aimLine.clear();
											
		point.position = m_position;
										
		m_aimLine.append(point);				

		point.position.x = static_cast<float>(t_event.mouseButton.x);
		point.position.y = static_cast<float>(t_event.mouseButton.y);
		m_aimLine.append(point);

		//Sets up the bool
		m_readyToFire = false;
		m_aimingNow = true;
	}
}

void Game::processMouseMove(sf::Event t_event)
{
	sf::Vertex point;
	point.color = sf::Color::White;


	if (m_aimingNow)
	{
		//find out what a vertex array is and what the actual contents of the vertex array are
		m_aimLine[1].position.x = static_cast<float>(t_event.mouseMove.x);
		m_aimLine[1].position.y = static_cast<float>(t_event.mouseMove.y);
	}
}

void Game::processMouseRelease(sf::Event t_event)
{
	sf::Vector2f direction{ 0.0f, 0.0f };
	sf::Vector2f mouseLocation{ 0.0f, 0.0f }; //temp vector

	if (m_aimingNow)
	{
		mouseLocation.x = static_cast<float>(t_event.mouseButton.x);
		mouseLocation.y = static_cast<float>(t_event.mouseButton.y);

		direction = mouseLocation - m_position;

		m_aimingNow = false;
		m_velocity = direction * POWER_ADJUSTMENT;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	
	//Every update will call these methods
	moveShell();
	checkBoundry(m_position, m_velocity);
	applyFriction(m_velocity);
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_window.draw(m_shell);

	//Displays it on screen
	if (m_aimingNow)
	{
		m_window.draw(m_aimLine);					
	}

	m_window.display();
}

void Game::setUpShell()
{
	m_shell.setFillColor(sf::Color::Green);
	m_shell.setPosition(m_position);
	m_shell.setRadius(RADIUS);
	m_shell.setOrigin(RADIUS, RADIUS);
}

void Game::moveShell()
{
	m_position += m_velocity;
	m_shell.setPosition(m_position);

	if (m_velocity == sf::Vector2f{ 0.0f,0.0f } && !m_aimingNow)
	{
		m_readyToFire = true;
	}
}


//----------------
//Check edges of the screen
//Invert to the X or Y component of velocity to simulate a bounce
//----------------
void Game::checkBoundry(sf::Vector2f t_position, sf::Vector2f& t_velocity)
{
	//Boundry for the top edge (based on the y co-ordinate)
	if (t_position.y < RADIUS)
	{
		t_velocity.y = -t_velocity.y;
	}

	//Boundry for right edge (based on the x co-ordinate)
	if (t_position.x > SCREEN_WIDTH - RADIUS)
	{
		t_velocity.x = -t_velocity.x;
	}

	//Boundry for bottom edge( based on y co-ordinate)
	if (t_position.y > SCREEN_HEIGHT - RADIUS)
	{
		t_velocity.y = -t_velocity.y;
	}

	//Boundry for left edge (based on the x co-ordinate)#
	if (t_position.x < RADIUS)
	{
		t_velocity.x = -t_velocity.x;
	}
}

void Game::applyFriction(sf::Vector2f& t_velocity)
{

	if (vectorLengthSquared(t_velocity) < SLOW_SPEED)			// Slow speed squared
	{
		if (vectorLengthSquared(t_velocity) < STALL_SPEED)		//reducing speed by this amount
		{
			t_velocity = sf::Vector2f{ 0.0f, 0.0f };			//Speed at which it stops 
		}
		t_velocity = t_velocity * LOW_FRICTION;					//Reduce by a specific amount
	}
	else
	{
		t_velocity = t_velocity * HIGH_FRICTION;
	}
}
