#include "Game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{

    this->currentMap = 0;
    this->window = window;
    std::cout << this->backgroundPath;
    if (!this->backgroundTexture.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    this->sprite = sf::Sprite(this->backgroundTexture);

    this->mario = new Mario();
}

int Game::getCurrentMap()
{
    return this->currentMap;
}

void Game::tick()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->mario->setDirection(1);
        this->mario->increaseAcceleration();

        if (!(this->mario->sprite.getPosition().x < WINDOW_WIDTH / 2.5))
            sprite.move(sf::Vector2f(-1, 0));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->mario->setDirection(-1);
        this->mario->increaseAcceleration();
    }
    else
    {
        this->mario->decreaseAcceleration();
    }
    this->mario->move();

    this->drawSprites();
}

void Game::drawSprites()
{
    this->window->draw(this->sprite);
    this->window->draw(this->mario->sprite);
}