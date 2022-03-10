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


/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	//---------------
	// Declarations
	// Using t_ for all parameter names
	// all void as they dont need to return anything
	//---------------
	void processEvents();
	void processKeys(sf::Event t_event);
	
	
	
	void update(sf::Time t_deltaTime);
	void render();
	void setUpShell(); //Creating definition 
	void moveShell();


	//--------------------
	// Member variables 
	// Using m_ for all member names
	//--------------------
	sf::RenderWindow m_window;					// main SFML window
	sf::CircleShape m_shell;					// setting shape of shell to a circle
	sf::Vector2f m_position{ 400.0f, 300.0f };	// position of shell
	sf::Vector2f m_velocity;

	bool m_exitGame;							// control exiting game

};

#endif // !GAME_HPP

