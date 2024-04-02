#include "Game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{

    this->current_map = 0;
    this->window = window;

    std::cout << this->backgroundPath;
    if (!this->backgroundTexture.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    this->sprites[0] = sf::Sprite(this->backgroundTexture);
    this->sprites[1] = sf::Sprite(this->backgroundTexture);
    this->sprites[1].setPosition(window->getPosition().x, 0);

    this->mario = Mario(window->getSize().y - 200);
}

int Game::getCurrentMap()
{
    return this->current_map;
}

void Game::tick()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        for (sf::Sprite &sprite : sprites)
            sprite.setPosition(sprite.getPosition().x - 0.06, 0);
        if (sprites[1].getPosition().x <= 0)
        {
            sprites[0].setPosition(this->window->getSize().x, 0);
            auto temp = sprites[0];
            sprites[0] = sprites[1];
            sprites[1] = temp;
        }
    }

    this->drawSprites();
}

void Game::drawSprites()
{
    for (const sf::Sprite &sprite : this->sprites)
        this->window->draw(sprite);
    
    this->window->draw(this->mario.sprite);
}