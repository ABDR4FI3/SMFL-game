#pragma once

#include <iostream>
#include <vector>
#include<ctime>
#include<sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
/*
	this calss is something  like a game engine 
*/
using namespace sf;
using namespace std; 
class Game
{
private :
	//window 
	sf::RenderWindow* window;
	VideoMode videoMode;
	Event ev;

	//Mouse position 
	Vector2i MousePosOnWindow;
	Vector2f MousePosView; 
	/*
	* resoureces 
	* here we iclude all the resources used in the game 
	* like : 
	* audio 
	* texture 
	* fonts ... 
	*/
	Font font; 
	
	// text 
	Text uiText; 
	/*
		Game logic
		something like The Algorithms or Ai 
	*/
	
	
	bool endGame; 
	unsigned points;//we use unsigned to get only positif values ;
	int health; 
	int bonus;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int MaxEnemies;
	bool mouseHeld; 

	

	//game objects
	vector<RectangleShape>enemies;
	RectangleShape enemy;	

	//private functions 
	void initVar(); 
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
public:
	//constructor 
	Game();
	//destructor
	virtual ~Game();
	//accesors 
	const bool runing() const;
	const bool getEndGame() const; 

//metohdes
	void SpawnEnemy();
	void pollEvent();
	void UpdateMousePosition();
	void updateText();
	void UpdateEnemies();
	void update();

	void renderText(RenderTarget& target);
	void renderEnemies(RenderTarget& target);
	void render();
	
	

};

