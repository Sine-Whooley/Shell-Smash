//Name: S?ne Whooley
//Student Number: C00271403
//Date: 10/03/22
//------------------
//Project: Shell Smash Project
//------------------



#include "Game.h"
#include "Vector2.h"
#include <iostream>

using namespace std;

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
	setUpShells();
	setUpScores();
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
											
		point.position = m_positions[m_lastShell - 1];
										
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

		direction = mouseLocation - m_positions[m_lastShell - 1];

		m_aimingNow = false;
		m_velocitys[m_lastShell - 1] = direction * POWER_ADJUSTMENT;
	}
}

void Game::drawShells()
{
	for (int i = 0; i < m_lastShell; i++)
	{
		m_shell.setPosition(m_positions[i]);
		if (m_isGreen[i])
		{
			m_shell.setFillColor(sf::Color::Green);
		}
		else
		{
			m_shell.setFillColor(sf::Color::Red);
		}

		m_window.draw(m_shell);
	}

}

void Game::drawScores()
{
	for (int i = 0; i < NO_SCORES; i++)
	{
		if (m_scoreDuration[i] > 0)
		{
			if (m_scoreWhite)
			{
				m_ScoreText.setFillColor(sf::Color::White);
			}
			else
			{
				m_ScoreText.setFillColor(sf::Color::Blue);
			}
			m_ScoreText.setPosition(m_scoreLocation[i]);
			m_ScoreText.setString(std::to_string(m_scoreValues[i]));
			m_window.draw(m_ScoreText);
		}
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
	checkBoundries();
	frictionToAll();
	checkCollisions();
	ageScores();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	drawShells();
	drawScores();
	setUpScores();

	//Displays it on screen
	if (m_aimingNow)
	{
		m_window.draw(m_aimLine);					
	}
	//m_window.draw(m_ScoreText);
	m_window.display();
	
}

void Game::setUpShells()
{
	sf::Vector2f redLocations[6] =
	{
		{300.0f, 300.0f},
		{250.0f, 250.0f},
		{250.0f, 350.0f},
		{200.0f, 300.0f},
		{200.0f, 400.0f},
		{200.0f, 200.0f}
	}; //Sets up an Array with 6 values

	const int NO_OF_RED_SHELLS = 6;

	m_shell.setFillColor(sf::Color::Green);
	m_shell.setRadius(RADIUS);
	m_shell.setOrigin(RADIUS, RADIUS);

	for (int i = 0; i < NO_OF_RED_SHELLS; i++)
	{
		m_velocitys[i] = sf::Vector2f {0.0f, 0.0f};
		m_positions[i] = redLocations[i];
		m_isGreen[i] = false;
	}
	for (int i = NO_OF_RED_SHELLS; i < NO_OF_SHELLS; i++)
	{
		m_positions[i] = sf::Vector2f{ 700.0f, 300.0f };
		m_velocitys[i] = sf::Vector2f{ 0.0f, 0.0f };
		m_isGreen[i] = true;
	}

	//m_velocitys[1] = sf::Vector2f{ 4.0f, 6.0f };
}

void Game::setUpScores()
{
	if (!m_font.loadFromFile("ASSETS\\FONTS\\arial-black.ttf"))
	{
		cout << "Problem with Font" << endl;
	}
	m_ScoreText.setFont(m_font);
	m_ScoreText.setCharacterSize(20U);
	m_ScoreText.setFillColor(sf::Color::White);
	m_ScoreText.setPosition(150.0f, 100.0f);
	m_ScoreText.setString("teheheheh");

	for (int i = 0; i < NO_SCORES; i++)
	{
		m_scoreDuration[i] = 0;
		m_scoreLocation[i] = sf::Vector2f{ 0.0f, 0.0f };
		m_scoreWhite[i] = true;
		m_scoreValues[i] = 0;
	}
	//m_scoreDuration[0] = 200;
	//m_scoreValues[0] = 20;
}

void Game::ageScores()
{
	for (int i = 0; i < NO_SCORES; i++)
	{
		if (m_scoreDuration[i] > 0)
		{
			m_scoreDuration[i] --;
		}
	}
}

void Game::createScore(sf::Vector2f t_location, bool t_isWhite)
{
	bool found = false;
	int index = -1;

	while (!found && index < NO_SCORES)
	{
		index++;

		if(m_scoreDuration[index] == 0)
		{
			found = true;
		}
	}

	if (index == NO_SCORES)
	{
		index = 0;
	}

	m_scoreDuration[index] = 60;
	m_scoreLocation[index] = t_location;
	m_scoreWhite[index] = t_isWhite;

	if (t_isWhite)
	{
		m_scoreValues[index] = m_currentScore * 10;
	}
	else
	{
		m_scoreValues[index] = m_currentScore * 100;
	}

	if (m_currentScore < 24) 
	{
	m_currentScore++;
	}
	
}

void Game::moveShell()
{
	bool allStopped = true;

	for(int i =0; i < m_lastShell; i++)
	{
		m_positions[i] += m_velocitys[i];

		if (m_velocitys[i] != sf::Vector2f{ 0.0f, 0.0f })
		{
			allStopped = false;
		}
	}

	if (allStopped & !m_readyToFire) //m_velocitys[0] == sf::Vector2f{ 0.0f,0.0f } && !m_aimingNow)
	{
		m_readyToFire = true;
		placeNewShell();
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

void Game::checkBoundries()
{
	for (int i = 0; i < m_lastShell; i++)
	{
		checkBoundry(m_positions[i], m_velocitys[i]);
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

void Game::frictionToAll()
{
	for (int i = 0; i < m_lastShell; i++)
	{
		applyFriction(m_velocitys[i]);
	}
}


//Check if the two Circles collide/ overlap
/// <param name="t_firstIndex"> Index of first Shell </param>
/// <param name="secondIndex"> Index of second Shell </param>
/// <returns> True is overlapping </returns>
bool Game::checkForACollision(int t_firstIndex, int t_secondIndex)
{
	sf::Vector2f difference = m_positions[t_firstIndex] - m_positions[t_secondIndex];

	if (-DIAMETER > difference.x || DIAMETER < difference.x)
	{
		return false;					//Too far apart on x-axis
	}

	if (-DIAMETER > difference.y || DIAMETER < difference.y)
	{
		return false;					//Too far apart on x-axis
	}

	float distanceSquared = 0.0f;
	distanceSquared = vectorLengthSquared(difference);

	if (distanceSquared < DIAMETER_SQUARED)
	{
		return true; 
	}

	return false;						//Overlapping Corner
}

void Game::checkCollisions()
{
	for (int i = 0; i < m_lastShell; i++)
	{
		for (int j = i+1; j < m_lastShell; j++)
		{
			if (checkForACollision(i, j))
			{
				processCollision(i,j);
			}
		}
	}
}


//Split shell velocity along line and plane of collision
//Swap the component along the line of centers
/// <param name="t_firstIndex"></param>
/// <param name="t_secondIndex"></param>
void Game::processCollision(int t_firstIndex, int t_secondIndex)
{
	bool greenOnGreen = false;

	sf::Vector2f midPoint = (m_positions[t_firstIndex] + m_positions[t_secondIndex]) / 2.0f;
	sf::Vector2f lineOfCenters = m_positions[t_firstIndex] - m_positions[t_secondIndex];

	sf::Vector2f oneSwap = vectorProjection(m_velocitys[t_firstIndex], lineOfCenters);
	sf::Vector2f twoSwap = vectorProjection(m_velocitys[t_secondIndex], lineOfCenters);

	sf::Vector2f keepOne = vectorRejection(m_velocitys[t_firstIndex], lineOfCenters);
	sf::Vector2f keepTwo = vectorRejection(m_velocitys[t_secondIndex], lineOfCenters);

	m_velocitys[t_firstIndex] = keepOne + twoSwap;
	m_velocitys[t_secondIndex] = keepTwo + oneSwap;

	while (checkForACollision(t_firstIndex, t_secondIndex))
	{
		m_positions[t_firstIndex] += m_velocitys[t_firstIndex] * 0.01f;  // Move 10% away
		m_positions[t_secondIndex] += m_velocitys[t_secondIndex] * 0.01f;
	}
	if (m_isGreen[t_firstIndex] && m_isGreen[t_secondIndex])
	{
		greenOnGreen = true;
	}
	createScore(midPoint, !greenOnGreen);
}

void Game::placeNewShell()
{
	int index = 0;

	if (m_lastShell < NO_OF_SHELLS-1)
	{
		m_lastShell++;
		for (int i = 0; i < m_lastShell; i++)
		{
			while (!checkForACollision(index, m_lastShell))
			{
				m_positions[m_lastShell] += sf::Vector2f{ -0.03f, 0.0f };
			}
		}
	}
}
