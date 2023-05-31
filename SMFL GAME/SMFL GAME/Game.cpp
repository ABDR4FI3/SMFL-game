#include "Game.h"

void Game::initVar()
{
	this->window = nullptr;

	//Game logic 
	this->endGame = false; 
	this->points = 0;
	this->health = 25; 
	this->bonus = 0; 
	this->MaxEnemies = 10;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->mouseHeld = false;
}



void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 400;
	this->window = new sf::RenderWindow (this->videoMode, "SFML works!", Style::Titlebar | Style::Close);
	this->window->setFramerateLimit(60);
}
void Game::initFonts()
{
	if (!this->font.loadFromFile("fonts/OptimusPrinceps.ttf")) {
		cout << "failed to load the font ";
	}
}
void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(20);
	this->uiText.setFillColor(Color::Yellow);
	this->uiText.setString("NONE");
	this->uiText.setLetterSpacing(3);
}
void Game::initEnemies()
{
	this->enemy.setPosition(Vector2f(100.f, 100.f)); 
	this->enemy.setSize(Vector2f(100.f, 100.f)); 
	this->enemy.setFillColor(Color::Cyan);
	//this->enemy.setOutlineColor(Color::Green);
	//this->enemy.setOutlineThickness(1.f);

}
//constructor
Game::Game() {
	this->initVar();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

//destructor
Game :: ~Game()
{
	delete this->window;
}
//accesors 
const bool Game::runing() const
{
	return this->window->isOpen();
	
}

const bool Game::getEndGame() const
{
	return this->endGame;
}




//functions

void Game::SpawnEnemy()
{
	/*
		Spawn the enemies 
		* sets their colors and size 
		* set Random Position
		* set Random Color
		* add enemy to the vector 
	*/

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f

	);
	//Randomize enemy type
	int type = rand() % 4;//random number from 0 to 3 
	switch (type)
	{
	case 0 :
		this->enemy.setSize(Vector2f(10.f, 10.f));
		this->enemy.setFillColor(Color::Red);
		this->bonus = 100;
		break;
	case 1 :
		this->enemy.setSize(Vector2f(40.f, 40.f));
		this->enemy.setFillColor(Color::Blue);
		this->bonus = 50;
		break;
	case 2 :
		this->enemy.setSize(Vector2f(70.f, 70.f));
		this->enemy.setFillColor(Color::Yellow);
		this->bonus = 25;
		break;
	case 3 :
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Cyan);
		this->bonus = 10;
		break;
	default:
		break;
	}

	//push the eneemy 
	this->enemies.push_back(this->enemy);
	//remove the enemy at the end of screen 
}

void Game::pollEvent()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case Event::Closed :
			this->window->close();
			break;
		case Event::KeyPressed:
			if (this->ev.key.code == Keyboard:: Escape )
			{
				this->window->close();
				
			}
			break;
		}
	}
}

void Game::UpdateMousePosition()
{
	/*
		update the mouse Position 
		relatively to our Window 
	*/
	this->MousePosOnWindow = Mouse::getPosition(*this->window);
	this->MousePosView = this->window->mapPixelToCoords(this->MousePosOnWindow);
}

void Game::updateText()
{
	stringstream ss;
	//we gonna write in the Var ss here below 
	ss << "Points " << this->points<<endl<<"Health "<<this->health;

	this->uiText.setString(ss.str());
}

void Game::UpdateEnemies() {

	/*
		this function update the timer (clock) for enemy 
		Spawning 
		and spawn a new enemy if the amount of Enemies is 
		less than the Max Amount 
		moves the enemies down
		removes the enemies at the edge of the screen 
	*/
	if (this->enemies.size() < this->MaxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			//Spawn the enemy and Reset The timer 
			this->SpawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	/* 
		Move the enemies
		 and update enemies and 
		cheking if its below the screen limits 
		if that's the case , we delete it 
	*/

	for (int i = 0; i< this->enemies.size() ; i++) {
		bool deleted = false;

		//enemy mouvment and speed
		this->enemies[i].move(0.f, 2.f);
		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			/*
				 Here we decrement the health by one point 
				 if the enemy reached the buttom of the window 
			*/
			this->health -= 1;
		}

	}
	//check if clicked upon
	
	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (this->mouseHeld == false) {
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->MousePosView)) {
					//here the user will win points if he killed 
					//the enemy 
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
					//10 points per kill 
					this->points += this->bonus;

				}

			}

		}
	}
	else
		this->mouseHeld = false;


}
void Game ::update()
{

	this->pollEvent();
	if (!this->endGame) {//if this endgame == false 
		this->UpdateMousePosition();
		this->updateText();
		this->UpdateEnemies();
	}

	//ending game condition
	if (this->health <= 0) {
		this->endGame = true; 
	}
}

void Game::renderText(RenderTarget& target)
{
	target.draw(this->uiText);
}
void Game::renderEnemies(RenderTarget& target){
	//loop to render all the enemies 
	for (auto& e : this->enemies) {
		target.draw(e);
		//e.move(0.f, 5.f);
	}

}
//render  
void Game::render()
{
	/*
		here we will render the game objects
		1st we will clear the old frame
		2nd render the objects 
		3rd display the frame in the window 
	*/
	this->window->clear(Color());

	//Draw the game 
	this->renderEnemies(*this->window);
	this->renderText(*this->window);

	this->window->display();
}


