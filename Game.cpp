#include "Game.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{

    this->currentMap = 0;
    this->window = window;

    this->mario = new Mario();
}

void Game::loadMap(Map *map)
{
    this->map = map;
    this->NB_GRIDS = map->getNumberOfGrids();
    std::cout << this->backgroundPath;
    if (!this->t_background.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->t_brick.loadFromFile(this->brickPath))
        throw std::invalid_argument("Could not load background texture");

    this->s_background = new sf::Sprite[NB_GRIDS];

    for (int i = 0; i < NB_GRIDS; i++)
    {
        auto sprite = sf::Sprite();
        this->s_background[i] = sprite;
        this->s_background[i].setTexture(this->t_background);
        this->s_background[i].setTextureRect(sf::IntRect(0, 40, SINGLE_BACKGROUND_WIDTH, WINDOW_HEIGHT));
        this->s_background[i].setPosition(i * SINGLE_BACKGROUND_WIDTH, 0);
    }

    generateSpritesInMemory();
}

void Game::generateSpritesInMemory() {
    int nbObjects = map->getNthGrid(current_grid)->NB_SPRITES + map->getNthGrid(current_grid+1)->NB_SPRITES; 
    int first = current_grid;
    if (current_grid-1 >= 0) {
        nbObjects += map->getNthGrid(current_grid-1)->NB_SPRITES;
        first--;
    }
    delete []this->s_objects; // memory freed
    //this->s_objects = NULL; not necessary since we reallocate some space below
    this->s_objects = new sf::Sprite[nbObjects];
    auto *save_ptr = this->s_objects;
    for (int i = first; i < current_grid + 2; i++) {
        auto *ptr = map->getNthGrid(i)->object;
        while (ptr) {
            if (ptr->type == "brick") {
                this->s_objects->setTexture(t_brick);
                this->s_objects->setTextureRect(sf::IntRect(272, 112, TILE_DIMENSION, TILE_DIMENSION));
                this->s_objects->setPosition(ptr->position.x+ this->s_background[current_grid].getPosition().x + (16 * 16  * (i - current_grid)), ptr->position.y );
            }
            ptr = ptr->next;
            this->s_objects++;
        }
    }
    this->s_objects = save_ptr;
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

    int old_current_grid = this->current_grid;
    this->current_grid = (int)(this->mario->realCoordinates.x / (16 * 16)); 
    if (this->current_grid != old_current_grid) {
        generateSpritesInMemory();
    }
    std::cout<<"current grid:" + std::to_string(current_grid)<<std::endl;
}

void Game::drawSprites()
{

    for (int i = 0; i < NB_GRIDS; i++)
    {
        this->window->draw(this->s_background[i]);
    }

    this->drawObjects();
    this->window->draw(this->mario->getSprite());
}

void Game::shiftSceneBackward()
{
    for (int i = 0; i < NB_GRIDS; i++)
        this->s_background[i].move(sf::Vector2f(-this->mario->getVelocity(), 0));

    this->shiftObjectsBackward();
    
}

void Game::shiftObjectsBackward() {
    auto *save_ptr = this->s_objects;
    int first = current_grid -1;
    if (first < 0)
        first++;
    for (int i = first; i < current_grid + 2; i++) {
        int nb = 0;
        while (nb < map->getNthGrid(i)->NB_SPRITES) {
            this->s_objects->move(sf::Vector2f(-this->mario->getVelocity(), 0));
            this->s_objects++;
            nb++;
        }
    }
    this->s_objects = save_ptr;
}

void Game::drawObjects() {
    auto *save_ptr = this->s_objects;
    int first = current_grid -1;
    if (first < 0)
        first++;
    for (int i = first; i < current_grid + 2; i++) {
    int nb = 0;
        while (nb < map->getNthGrid(i)->NB_SPRITES ) {
                this->window->draw(*this->s_objects);
            nb++;
            this->s_objects++;
        }
    }
    this->s_objects = save_ptr;
}