
#include "Game.h"
#include "Tile.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  
}

Game::~Game()
{
    
}

bool Game::init()
{
    board = std::make_shared<Board>(window);
    board->init(); 

    host_text.initialiseText("Host");
    host_text.getText()->setPosition(
        sf::Vector2f(window.getSize().x * 0.85 - host_text.getText()->getGlobalBounds().size.x * 0.85,
            window.getSize().y * 0.2 - host_text.getText()->getGlobalBounds().size.y * 0.2));

    join_text.initialiseText("Join");
    join_text.getText()->setPosition(sf::Vector2f(window.getSize().x * 0.85 - join_text.getText()->getGlobalBounds().size.x * 0.85, window.getSize().y * 0.4 - join_text.getText()->getGlobalBounds().size.y * 0.4));
    

    return true;
}

void Game::update(float dt)
{

}

void Game::render()
{
     
    switch (state)
    {
        case MENU:
        {
            board->render();
            window.draw(*host_text.getText());
            window.draw(*join_text.getText());
            break;
        } 
       

    }
   
}

void Game::mouseButtonPressed(const sf::Event::MouseButtonPressed* event)
{
    board->mouseButtonPressed(event);
   
}

void Game::mouseButtonReleased(const sf::Event::MouseButtonReleased* event)
{
    board->mouseButtonReleased(event); 
    
    switch(state)
    {
        case GameState::MENU:
        {
            if (host_text.getText()->getGlobalBounds().contains(sf::Vector2f(event->position)))
            {
                state = GAMEPLAY;
                std::cout << "creating server" << std::endl;
                server = std::make_unique<Server>();
                server->init();
                server->run();
            }
            else if (join_text.getText()->getGlobalBounds().contains(sf::Vector2f(event->position)))
            {
                state = GAMEPLAY;
                std::cout << "Attempting to connect to Seerver" << std::endl;
                client = std::make_unique<Client>();
                client->connect();
                client->run();
            }
                
     
            break;
        }
    }
}

void Game::keyPressed(const sf::Event::KeyPressed* event)
{

}

void Game::keyReleased(const sf::Event::KeyReleased* event)
{

}

void Game::mouseMoved(const sf::Event::MouseMoved* event)
{
    board->mouseMoved(event);
}




