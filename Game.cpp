#include "Game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{

    this->currentMap = 0;
    this->window = window;
    std::cout << this->backgroundPath;
    if (!this->t_background.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->t_brick.loadFromFile(this->brickPath))
        throw std::invalid_argument("Could not load background texture");

    this->s_background = new sf::Sprite[NB_BACKGROUNDS];

    for (int i = 0; i < NB_BACKGROUNDS; i++)
    {
        auto sprite = sf::Sprite();
        this->s_background[i] = sprite;
        this->s_background[i].setTexture(this->t_background);
        this->s_background[i].setTextureRect(sf::IntRect(0, 40, SINGLE_BACKGROUND_WIDTH, WINDOW_HEIGHT));
        this->s_background[i].setPosition(i * SINGLE_BACKGROUND_WIDTH, 0);
    }

    this->s_brick = sf::Sprite();
    this->s_brick.setTexture(t_brick);
    this->s_brick.setTextureRect(sf::IntRect(272, 112, TILE_DIMENSION, TILE_DIMENSION));
    this->s_brick.setPosition(TILE_DIMENSION * 7, TILE_DIMENSION*5);

    this->mario = new Mario();
}

int Game::getCurrentMap()
{
    return this->currentMap;
}

void Game::tick(sf::Clock *clock)
{

    sf::Event event;

    // std::cout << "mario velocity: " + std::to_string(this->mario->getVelocity()) << std::endl;
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
            // std::cout << "Key released:" + std::to_string(event.key.code) << std::endl;
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

    if (this->mario->marioIsFreezed())
    {

        this->shiftSceneBackward();
    }

    this->mario->moveX();
    this->mario->moveY();
    std::cout << std::to_string(this->mario->realCoordinates.x) << std::endl;
    this->drawSprites();
}

void Game::drawSprites()
{

    for (int i = 0; i < NB_BACKGROUNDS; i++)
    {
        this->window->draw(this->s_background[i]);
    }
    this->window->draw(this->mario->getSprite());
    this->window->draw(this->s_brick);
}

void Game::shiftSceneBackward()
{
    for (int i = 0; i < NB_BACKGROUNDS; i++)
        this->s_background[i].move(sf::Vector2f(-this->mario->getVelocity(), 0));
    
    this->s_brick.move(sf::Vector2f(-this->mario->getVelocity(), 0));
}