//Name: Síne Whooley
//Student Number: C00271403
//Date: 10/03/22
//------------------
//Project: Shell Smash Project
//------------------


#ifndef GAME_HPP
#define GAME_HPP

//Global Variables (Constant through out)
const float RADIUS = 15.0f;
const float SCREEN_HEIGHT = 600.f;
const float SCREEN_WIDTH = 800.0f;
const float POWER_ADJUSTMENT = 0.2f;
const float LOW_FRICTION = 0.995f;
const float HIGH_FRICTION = 0.991f;
const float STALL_SPEED = 0.5f;
const float SLOW_SPEED = 15.0f;


/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include "Vector2.h"

class Game
{
public:
	Game();
	~Game();
	//--------------
	/// main method for game
	//--------------
	void run();

private:

	//---------------
	// Declarations
	// Using t_ for all parameter names
	// all void as they dont need to return anything
	//---------------
	void processEvents();
	void processKeys(sf::Event t_event);
	void processMousePressed(sf::Event t_event);
	void processMouseMove(sf::Event t_event);
	void processMouseRelease(sf::Event t_event);
	
	
	void update(sf::Time t_deltaTime);
	void render();
	void setUpShell(); //Creating definition 
	void moveShell();
	void checkBoundry(sf::Vector2f t_position, sf::Vector2f &t_velocity );
	void applyFriction(sf::Vector2f& t_velocity);

	//--------------------
	// Member variables 
	// Using m_ for all member names
	//--------------------
	sf::RenderWindow m_window;					// main SFML window
	bool m_exitGame;							// control exiting game

	sf::CircleShape m_shell;					// setting shape of shell to a circle
	sf::Vector2f m_position{ 400.0f, 300.0f };	// position of shell
	sf::Vector2f m_velocity{ 0.0f, 0.0f };		// velocity of shell

	

	sf::VertexArray m_aimLine{ sf::Lines };				//Line for aiming
	bool m_readyToFire{ true };							//Player allowed to fire
	bool m_aimingNow{ false };							//Player is aiming (nothing will happen while its false)
														//wont start until ready to aimingNow is true

};

#endif // !GAME_HPP

