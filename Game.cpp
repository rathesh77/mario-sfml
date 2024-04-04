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
    sf::Event event;
    while (this->window->pollEvent(event))
    {

        switch (event.type)
        {
        case sf::Event::Closed:
            this->window->close();
        case sf::Event::KeyReleased:
            std::cout << "Key released:" + std::to_string(event.key.code) << std::endl;
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Right:
            case sf::Keyboard::Key::Left:
                this->mario->setDirection(0);
                break;
            }
            break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        std::cout << "right" << std::endl;
        this->mario->setDirection(1);

        if (!(this->mario->sprite.getPosition().x < WINDOW_WIDTH / 2.5))
            this->sprite.move(sf::Vector2f(-3, 0));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->mario->setDirection(-1);
        std::cout << "left" << std::endl;
    }

    if (!(this->mario->sprite.getPosition().x < WINDOW_WIDTH / 2.5) && this->mario->getDirection() == 1)
    {
        this->mario->setDirection(0);
        this->mario->setVel(0);
    }

    this->mario->move();
    this->drawSprites();
}

void Game::drawSprites()
{
    this->window->draw(this->sprite);
    this->window->draw(this->mario->sprite);
}