#include "Game.hpp"
#include "Ground.hpp"
#include <iostream>

Game::Game(sf::RenderWindow *window)
{
    this->m_currentMap = 0;
    this->m_window = window;

    this->m_mario = new Mario();
    // buffer.loadFromFile("audio/overworld.mp3");

    // sound.setBuffer(buffer);
    // sound.play();
}

void Game::loadMap(Map *map)
{
    this->m_map = map;
    this->m_nb_grids = map->getNumberOfGrids();

    if (!this->m_t_sky.loadFromFile(this->m_skyPath))
        throw std::invalid_argument("Could not load goomba texture");

    if (!this->m_t_background.loadFromFile(this->m_backgroundPath))
        throw std::invalid_argument("Could not load background texture");

    if (!this->m_t_brick.loadFromFile(this->m_brickPath))
        throw std::invalid_argument("Could not load brick texture");

    if (!this->m_t_ground.loadFromFile(
            this->m_groundPath))
        throw std::invalid_argument("Could not load ground texture");

    if (!this->m_t_ennemies.loadFromFile(this->m_ennemiesPath))
        throw std::invalid_argument("Could not load goomba texture");

    this->m_s_background = new sf::Sprite[m_nb_grids];
    this->m_s_sky = new sf::Sprite[2 * TILE_DIMENSION * TILE_DIMENSION];

    for (int i = 0; i < m_nb_grids; i++)
    {
        auto sprite = sf::Sprite();
        this->m_s_background[i] = sprite;
        this->m_s_background[i].setTexture(this->m_t_background);
        this->m_s_background[i].setTextureRect(
            sf::IntRect(0, 40, m_single_background_width, WINDOW_HEIGHT));
        this->m_s_background[i].setPosition(i * m_single_background_width, 0);
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < TILE_DIMENSION; j++)
        {
            for (int k = 0; k < TILE_DIMENSION; k++)
            {
                auto skySprite = sf::Sprite();
                const int coord = (TILE_DIMENSION * TILE_DIMENSION * (i)) +
                                  ((j * TILE_DIMENSION) + k);
                this->m_s_sky[coord] = skySprite;
                this->m_s_sky[coord].setTexture(this->m_t_sky);
                this->m_s_sky[coord].setTextureRect(
                    sf::IntRect(369, 1628, TILE_DIMENSION, TILE_DIMENSION));
                this->m_s_sky[coord].setColor(sf::Color(92, 148, 252));
                this->m_s_sky[coord].setPosition(
                    (j * TILE_DIMENSION) +
                        (i * TILE_DIMENSION * TILE_DIMENSION),
                    k * TILE_DIMENSION);
            }
        }
    }

    generateSpritesInMemory();
}

void Game::generateSpritesInMemory()
{
    int nbObjects = m_map->getNumberOfGrids() * 40;
    int first = m_current_grid;

    delete[] this->m_s_objects; // memory freed
    // this->s_objects = NULL; not necessary since we reallocate some space
    // below
    this->m_s_objects = new SpriteObject[nbObjects + 1];
    this->m_s_objects[nbObjects].type = "NULL";

    auto *save_ptr = this->m_s_objects;
    for (int i = 0; i < m_map->getNumberOfGrids(); i++)
    {
        auto *ptr = m_map->getNthGrid(i)->object;
        while (ptr)
        {
            this->m_s_objects->type = ptr->type;
            this->m_nb_sprites++;
            const float x =
                ptr->position.x +
                this->m_s_background[m_current_grid].getPosition().x +
                (TILE_DIMENSION * TILE_DIMENSION * (i - m_current_grid));

            const float y = ptr->position.y;
            if (ptr->type == "brick")
            {
                this->m_s_objects->body = new Ground(&this->m_t_brick, x, y, 272, 112, 16, 16, ptr->type);
            }
            else if (ptr->type == "goomba")
            {

                this->m_s_objects->body = new Body(&this->m_t_ennemies, x, y, 0, 16, 16, 16, ptr->type);
            }
            else if (ptr->type == "ground")
            {
                this->m_s_objects->body = new Ground(&this->m_t_ground, x, y, 32, 460, 16, 16, ptr->type);
            }
            else if (ptr->type == "stone")
            {
                this->m_s_objects->body = new Ground(&this->m_t_ground, x, y, 0, 33, 16, 16, ptr->type);
            }
            else if (ptr->type == "pipe")
            {
                this->m_s_objects->body = new Ground(&this->m_t_ground, x, y, 112, 612, 32, 63, ptr->type);
            }
            ptr = ptr->next;
            this->m_s_objects++;
        }
    }
    this->m_s_objects[this->m_nb_sprites].type = "NULL";
    SpriteObject *_m_s_objects = new SpriteObject[this->m_nb_sprites + 1];
    memcpy(_m_s_objects, save_ptr,
           sizeof(SpriteObject) * (this->m_nb_sprites + 1));
    this->m_s_objects = NULL;
    save_ptr = NULL;
    this->m_s_objects = _m_s_objects;
    _m_s_objects = NULL;
}

int Game::getCurrentMap() { return this->m_currentMap; }

void Game::tick(sf::Clock *clock)
{
    sf::Event event;

    while (this->m_window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            this->m_window->close();
        case sf::Event::KeyReleased:
            if (!this->m_lost)
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Right:
                case sf::Keyboard::Key::Left:
                    this->m_mario->setDirectionX(0);
                    break;
                }
            break;
        }
    }

    if (!this->m_lost)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            this->m_mario->setDirectionX(1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            this->m_mario->setDirectionX(-1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            //this->m_mario->resetY();
            this->m_mario->jump();
        }

        if (clock->getElapsedTime().asSeconds() >= 0.1f)
        {
            this->frameCount++;
            clock->restart();
            this->m_mario->loadSpriteForward(this->frameCount);
        }

        if (this->m_mario->marioIsFreezed())
        {
            this->shiftSceneBackward();
        }

        if (this->m_mario->getY() >= WINDOW_HEIGHT ||
            (this->m_mario->isOverlaping() && this->m_mario->hasHitEnnemy()))
        {
            this->m_lost = true;
            // sound.stop();
        }
        this->m_mario->loop(this->m_s_objects);
    }
    else
    {
        /*if (sound.getStatus() == sf::Sound::Stopped) {
            buffer.loadFromFile("audio/mario_dies.mp3");
            sound.setBuffer(buffer);
            sound.play();
        }*/
    }
    this->drawSprites();

    this->m_current_grid = (int)(this->m_mario->realCoordinates.x /
                                 (TILE_DIMENSION * TILE_DIMENSION));
}

void Game::drawSprites()
{
    for (int i = 0; i < 2 * TILE_DIMENSION * TILE_DIMENSION; i++)
    {
        this->m_window->draw(this->m_s_sky[i]);
    }
    for (int i = 0; i < m_nb_grids; i++)
    {
        this->m_window->draw(this->m_s_background[i]);
    }

    this->drawObjects();
    this->m_window->draw(*this->m_mario->getSprite());
}

void Game::shiftSceneBackward()
{
    for (int i = 0; i < m_nb_grids; i++)
        this->m_s_background[i].move(
            sf::Vector2f(-this->m_mario->getVelocity(), 0));

    this->shiftObjectsBackward();
}

void Game::shiftObjectsBackward()
{
    auto *save_ptr = this->m_s_objects;

    for (int i = 0; i < m_map->getNumberOfGrids(); i++)
    {
        int nb = 0;
        while (nb < m_map->getNthGrid(i)->NB_SPRITES)
        {

            this->m_s_objects->body->getSprite()->move(
                sf::Vector2f(-this->m_mario->getVelocity(), 0));

            this->m_s_objects++;
            nb++;
        }
    }
    this->m_s_objects = save_ptr;
}

void Game::drawObjects()
{
    auto *save_ptr = this->m_s_objects;

    for (int i = 0; i < m_map->getNumberOfGrids(); i++)
    {
        int nb = 0;

        while (nb < m_map->getNthGrid(i)->NB_SPRITES)
        {

            // the commented code belows clear sprites that are offscreen. Might need to use it later
            /*if (this->m_s_objects->body->getPosition().x < -TILE_DIMENSION) {
                this->m_s_objects->body = new Body;
                nb++;
                this->m_s_objects++;
                continue;
            }*/
            if (this->m_s_objects->type == "goomba")
            {
                if (this->m_s_background[i].getPosition().x < WINDOW_WIDTH)
                {
                    this->m_s_objects->body->loop(save_ptr);

                    this->m_window->draw(*this->m_s_objects->body->getSprite());
                }
            }
            else
            {
                this->m_window->draw(*this->m_s_objects->body->getSprite());
            }
            nb++;
            this->m_s_objects++;
        }
    }
    this->m_s_objects = save_ptr;
}