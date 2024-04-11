#include "Game.hpp"

#include <iostream>

Game::Game(sf::RenderWindow *window) {
    this->currentMap = 0;
    this->m_window = window;

    this->m_mario = new Mario();
}

void Game::loadMap(Map *map) {
    this->m_map = map;
    this->NB_GRIDS = map->getNumberOfGrids();
    std::cout << this->m_backgroundPath;
    if (!this->m_t_background.loadFromFile(this->m_backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->m_t_brick.loadFromFile(this->m_brickPath))
        throw std::invalid_argument("Could not load brick texture");

    if (!this->m_t_ennemies.loadFromFile(this->m_ennemiesPath))
        throw std::invalid_argument("Could not load goomba texture");

    this->m_s_background = new sf::Sprite[NB_GRIDS];

    for (int i = 0; i < NB_GRIDS; i++) {
        auto sprite = sf::Sprite();
        this->m_s_background[i] = sprite;
        this->m_s_background[i].setTexture(this->m_t_background);
        this->m_s_background[i].setTextureRect(
            sf::IntRect(0, 40, SINGLE_BACKGROUND_WIDTH, WINDOW_HEIGHT));
        this->m_s_background[i].setPosition(i * SINGLE_BACKGROUND_WIDTH, 0);
    }

    generateSpritesInMemory();
}

void Game::generateSpritesInMemory() {
    int nbObjects = m_map->getNumberOfGrids() * 20;
    int first = m_current_grid;

    delete[] this->m_s_objects;  // memory freed
    // this->s_objects = NULL; not necessary since we reallocate some space
    // below
    this->m_s_objects = new SpriteObject[nbObjects + 1];
    this->m_s_objects[nbObjects].type = "NULL";

    auto *save_ptr = this->m_s_objects;
    for (int i = 0; i < m_map->getNumberOfGrids(); i++) {
        auto *ptr = m_map->getNthGrid(i)->object;
        while (ptr) {
            this->m_s_objects->type = ptr->type;
            if (ptr->type == "brick") {
                this->m_s_objects->sprite->setTexture(m_t_brick);
                this->m_s_objects->sprite->setTextureRect(
                    sf::IntRect(272, 112, TILE_DIMENSION, TILE_DIMENSION));
                this->m_s_objects->sprite->setPosition(
                    ptr->position.x +
                        this->m_s_background[m_current_grid].getPosition().x +
                        (TILE_DIMENSION * TILE_DIMENSION * (i - m_current_grid)),
                    ptr->position.y);
            } else if (ptr->type == "goomba") {
                const float x =
                    ptr->position.x +
                    this->m_s_background[m_current_grid].getPosition().x +
                    (TILE_DIMENSION * TILE_DIMENSION * (i - m_current_grid));

                const float y = ptr->position.y;
                this->m_s_objects->body = new Body(this->m_ennemiesPath, x, y);
            }
            ptr = ptr->next;
            this->m_s_objects++;
        }
    }
    this->m_s_objects = save_ptr;
}

int Game::getCurrentMap() { return this->currentMap; }

void Game::tick(sf::Clock *clock) {
    sf::Event event;

    // std::cout << "mario velocity: " +
    // std::to_string(this->mario->getVelocity()) << std::endl;

    while (this->m_window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                this->m_window->close();
            case sf::Event::KeyReleased:
                // std::cout << "Key released:" +
                // std::to_string(event.key.code)
                // << std::endl;
                if (!this->m_lost) switch (event.key.code) {
                        case sf::Keyboard::Key::Right:
                        case sf::Keyboard::Key::Left:
                            this->m_mario->setDirectionX(0);
                            break;
                    }
                break;
        }
    }

    if (!this->m_lost) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            this->m_mario->setDirectionX(1);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            this->m_mario->setDirectionX(-1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            this->m_mario->resetY();
            this->m_mario->jump();
        }

        if (clock->getElapsedTime().asSeconds() >= 0.1f) {
            this->frameCount++;
            clock->restart();
            this->m_mario->loadSpriteForward(this->frameCount);
        }

        if (this->m_mario->marioIsFreezed()) {
            this->shiftSceneBackward();
        }

        if (this->m_mario->isOverlaping() && this->m_mario->hasHitEnnemy()) {
            this->m_lost = true;
        }
        this->m_mario->loop(this->m_s_objects, m_nb_sprites);
        this->drawSprites();

    }

    this->m_current_grid = (int)(this->m_mario->realCoordinates.x /
                               (TILE_DIMENSION * TILE_DIMENSION));

    // std::cout<<"current grid:" + std::to_string(current_grid)<<std::endl;
}

void Game::drawSprites() {
    for (int i = 0; i < NB_GRIDS; i++) {
        this->m_window->draw(this->m_s_background[i]);
    }

    this->drawObjects();
    this->m_window->draw(*this->m_mario->getSprite());
}

void Game::shiftSceneBackward() {
    for (int i = 0; i < NB_GRIDS; i++)
        this->m_s_background[i].move(
            sf::Vector2f(-this->m_mario->getVelocity(), 0));

    this->shiftObjectsBackward();
}

void Game::shiftObjectsBackward() {
    auto *save_ptr = this->m_s_objects;

    for (int i = 0; i < m_map->getNumberOfGrids(); i++) {
        int nb = 0;
        while (nb < m_map->getNthGrid(i)->NB_SPRITES) {
            if (this->m_s_objects->type == "brick") {
                this->m_s_objects->sprite->move(
                    sf::Vector2f(-this->m_mario->getVelocity(), 0));
            } else {
                this->m_s_objects->body->getSprite()->move(
                    sf::Vector2f(-this->m_mario->getVelocity(), 0));
            }
            this->m_s_objects++;
            nb++;
        }
    }
    this->m_s_objects = save_ptr;
}

void Game::drawObjects() {
    this->m_nb_sprites = 0;
    auto *save_ptr = this->m_s_objects;

    for (int i = 0; i < m_map->getNumberOfGrids(); i++) {
        int nb = 0;

        while (nb < m_map->getNthGrid(i)->NB_SPRITES) {
            if (this->m_s_objects->type == "brick") {
                this->m_window->draw(*this->m_s_objects->sprite);
            } else if (this->m_s_objects->type == "goomba") {
                if (this->m_s_background[i].getPosition().x < WINDOW_WIDTH) {
                    this->m_s_objects->body->loop(save_ptr, 100);

                    this->m_window->draw(*this->m_s_objects->body->getSprite());
                }
            }
            nb++;
            this->m_s_objects++;
        }
        this->m_nb_sprites += nb;
    }
    this->m_s_objects = save_ptr;
}
