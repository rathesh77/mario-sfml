#include "Game.hpp"

#include <iostream>

Game::Game(sf::RenderWindow *window) {
    this->currentMap = 0;
    this->window = window;

    this->mario = new Mario();
}

void Game::loadMap(Map *map) {
    this->map = map;
    this->NB_GRIDS = map->getNumberOfGrids();
    std::cout << this->backgroundPath;
    if (!this->t_background.loadFromFile(this->backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->t_brick.loadFromFile(this->brickPath))
        throw std::invalid_argument("Could not load brick texture");

    if (!this->t_ennemies.loadFromFile(this->ennemiesPath))
        throw std::invalid_argument("Could not load goomba texture");

    this->s_background = new sf::Sprite[NB_GRIDS];

    for (int i = 0; i < NB_GRIDS; i++) {
        auto sprite = sf::Sprite();
        this->s_background[i] = sprite;
        this->s_background[i].setTexture(this->t_background);
        this->s_background[i].setTextureRect(
            sf::IntRect(0, 40, SINGLE_BACKGROUND_WIDTH, WINDOW_HEIGHT));
        this->s_background[i].setPosition(i * SINGLE_BACKGROUND_WIDTH, 0);
    }

    generateSpritesInMemory();
}

void Game::generateSpritesInMemory() {
    int nbObjects = map->getNumberOfGrids() * 20;
    int first = current_grid;

    delete[] this->s_objects;  // memory freed
    // this->s_objects = NULL; not necessary since we reallocate some space
    // below
    this->s_objects = new SpriteObject[nbObjects];
    auto *save_ptr = this->s_objects;
    for (int i = 0; i < map->getNumberOfGrids(); i++) {
        auto *ptr = map->getNthGrid(i)->object;
        while (ptr) {
            this->s_objects->type = ptr->type;
            if (ptr->type == "brick") {
                this->s_objects->sprite->setTexture(t_brick);
                this->s_objects->sprite->setTextureRect(
                    sf::IntRect(272, 112, TILE_DIMENSION, TILE_DIMENSION));
            } else if (ptr->type == "goomba") {
                this->s_objects->sprite->setTexture(t_ennemies);
                this->s_objects->sprite->setTextureRect(
                    sf::IntRect(0, 16, TILE_DIMENSION, TILE_DIMENSION));
            }
            this->s_objects->sprite->setPosition(
                ptr->position.x +
                    this->s_background[current_grid].getPosition().x +
                    (TILE_DIMENSION * TILE_DIMENSION * (i - current_grid)),
                ptr->position.y);
            ptr = ptr->next;
            this->s_objects++;
        }
    }
    this->s_objects = save_ptr;
}

int Game::getCurrentMap() { return this->currentMap; }

void Game::tick(sf::Clock *clock) {
    sf::Event event;

    // std::cout << "mario velocity: " +
    // std::to_string(this->mario->getVelocity()) << std::endl;

    while (this->window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                this->window->close();
            case sf::Event::KeyReleased:
                // std::cout << "Key released:" +
                // std::to_string(event.key.code)
                // << std::endl;
                if (!this->lost) switch (event.key.code) {
                        case sf::Keyboard::Key::Right:
                        case sf::Keyboard::Key::Left:
                            this->mario->setDirectionX(0);
                            break;
                    }
                break;
        }
    }

    if (!this->lost) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            this->mario->setDirectionX(1);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            this->mario->setDirectionX(-1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            this->mario->resetY();
            this->mario->jump();
        }

        if (clock->getElapsedTime().asSeconds() >= 0.1f) {
            this->frameCount++;
            clock->restart();
            this->mario->loadSpriteForward(this->frameCount);
        }

        this->moveGoombas();
        if (this->mario->marioIsFreezed()) {
            this->shiftSceneBackward();
        }

        if (this->mario->isOverlaping()) {
            this->mario->overlap = false;
            if (this->mario->hasHitEnnemy()) {
                this->lost = true;
            }

        } else {
            this->mario->updateHorizontalVelocity();
            this->mario->updateVerticalVelocity();
            this->mario->postCollisionsDetection();

            this->mario->detectCollisions(this->s_objects, this->NB_SPRITES);
        }
        this->mario->moveX();
        this->mario->moveY();

        this->drawSprites();

        this->mario->resetY();
    }

    this->current_grid = (int)(this->mario->realCoordinates.x /
                               (TILE_DIMENSION * TILE_DIMENSION));

    // std::cout<<"current grid:" + std::to_string(current_grid)<<std::endl;
}

void Game::drawSprites() {
    for (int i = 0; i < NB_GRIDS; i++) {
        this->window->draw(this->s_background[i]);
    }

    this->drawObjects();
    this->window->draw(this->mario->getSprite());
}

void Game::shiftSceneBackward() {
    for (int i = 0; i < NB_GRIDS; i++)
        this->s_background[i].move(
            sf::Vector2f(-this->mario->getVelocity(), 0));

    this->shiftObjectsBackward();
}

void Game::shiftObjectsBackward() {
    auto *save_ptr = this->s_objects;

    for (int i = 0; i < map->getNumberOfGrids(); i++) {
        int nb = 0;
        while (nb < map->getNthGrid(i)->NB_SPRITES) {
            this->s_objects->sprite->move(
                sf::Vector2f(-this->mario->getVelocity(), 0));
            this->s_objects++;
            nb++;
        }
    }
    this->s_objects = save_ptr;
}

void Game::drawObjects() {
    this->NB_SPRITES = 0;
    auto *save_ptr = this->s_objects;

    for (int i = 0; i < map->getNumberOfGrids(); i++) {
        int nb = 0;

        while (nb < map->getNthGrid(i)->NB_SPRITES) {
            this->window->draw(*this->s_objects->sprite);
            nb++;
            this->s_objects++;
        }
        this->NB_SPRITES += nb;
    }
    this->s_objects = save_ptr;
}

void Game::moveGoombas() {
    auto *save_ptr = this->s_objects;
    for (int i = 0; i < map->getNumberOfGrids(); i++) {
        int nb = 0;

        while (nb < map->getNthGrid(i)->NB_SPRITES) {
            if (save_ptr->type == "goomba" &&
                this->s_background[i].getPosition().x < WINDOW_WIDTH)
                save_ptr->sprite->move(-1.0f, 0);
            nb++;
            save_ptr++;
        }
    }
}