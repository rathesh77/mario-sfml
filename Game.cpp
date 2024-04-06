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

void Game::tick(sf::Clock *clock)
{

    sf::Event event;

    std::cout << "mario velocity: " + std::to_string(this->mario->getVelocity()) << std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->mario->setDirection(1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->mario->setDirection(-1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        this->mario->jump();
    }

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

    if (clock->getElapsedTime().asSeconds() >= 0.1f)
    {
        this->frameCount++;
        clock->restart();
        this->mario->loadSpriteForward(this->frameCount);
    }

    this->mario->updateVelocity();
    if ((this->mario->getPosition().x >= WINDOW_WIDTH / 2.5))
    {
        if (!this->mario->decelerating && this->mario->getDirection() < 1)
        {
            this->mario->move();
        }
        else
        {
            this->sprite.move(sf::Vector2f(-this->mario->getVelocity(), 0));
        }
    }
    else
    {
        this->mario->move();
    }
    this->drawSprites();
}

void Game::drawSprites()
{
    this->window->draw(this->sprite);
    this->window->draw(this->mario->sprite);
}