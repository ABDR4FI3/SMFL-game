#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include "Game.h"

using namespace sf; 
int main(){

    //init Srand (init the funtion Srandom)
    srand(static_cast<unsigned>(time(NULL)));


    //init game engine 

    Game Mygame;
    Mygame.update();
    

    //game loop 
    
    while ( Mygame.runing() && !Mygame.getEndGame())
    {
        //update 
        Mygame.update();
         //render 
        Mygame.render();

    }

    return 0;
}