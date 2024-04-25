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


    if (!this->m_t_hud.loadFromFile(this->m_hudPath))
        throw std::invalid_argument("Could not load goomba texture");

    this->m_s_hud = new sf::Sprite;
    this->m_s_hud->setTexture(this->m_t_hud);
    this->m_s_hud->setPosition(0,0);
    this->m_s_hud->setTextureRect(sf::IntRect(0 , 8, 255, 39-7 ));
    this->m_s_background = new sf::Sprite[m_nb_grids];
    this->m_s_background = new sf::Sprite[m_nb_grids];
    this->m_s_sky = new sf::Sprite[2 * TILE_DIMENSION * TILE_DIMENSION];

    for (int i = 0; i < m_nb_grids; i++)
    {
        sf::Sprite sprite = sf::Sprite();
        this->m_s_background[i] = sprite;
        this->m_s_background[i].setTexture(this->m_t_background);
        this->m_s_background[i].setTextureRect(
            sf::IntRect(0, 8, m_single_background_width, WINDOW_WIDTH));
        this->m_s_background[i].setPosition(i * m_single_background_width, 0);
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < TILE_DIMENSION; j++)
        {
            for (int k = 0; k < TILE_DIMENSION; k++)
            {
                sf::Sprite skySprite = sf::Sprite();
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

    this->m_s_score_digit_1 = new sf::Sprite;
    this->m_s_score_digit_2 = new sf::Sprite;
    this->m_s_score_digit_3 = new sf::Sprite;
    this->m_s_score_digit_4 = new sf::Sprite;
    this->m_s_score_digit_5 = new sf::Sprite;
    this->m_s_score_digit_6 = new sf::Sprite;


    this->m_s_score_digit_1->setTexture(this->m_t_hud);
    this->m_s_score_digit_2->setTexture(this->m_t_hud);
    this->m_s_score_digit_3->setTexture(this->m_t_hud);
    this->m_s_score_digit_4->setTexture(this->m_t_hud);
    this->m_s_score_digit_5->setTexture(this->m_t_hud);
    this->m_s_score_digit_6->setTexture(this->m_t_hud);

    this->updateInfos();
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

    SpriteObject *save_ptr = this->m_s_objects;
    for (int i = 0; i < m_map->getNumberOfGrids(); i++)
    {
        Object *ptr = m_map->getNthGrid(i)->object;
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
        std::vector<Event> marioEvents = this->m_mario->loop(this->m_s_objects);
        bool hasKilledGoomba = false;
        for (Event e : marioEvents) {
            if (e.getType() == GOOMBA_KILLED) {
                this->m_score += 100;
                hasKilledGoomba = true;
            }
        }
        if (hasKilledGoomba)
            this->updateInfos();
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
    this->drawText();

    this->m_current_grid = (int)(this->m_mario->realCoordinates.x /
                                 (TILE_DIMENSION * TILE_DIMENSION));
}

void Game::updateInfos() {

    std::string currentScore = std::to_string(this->m_score);
    int size = currentScore.length();
    for (int i = 0; i < 6 - size; i++) {
        currentScore = '0' +  currentScore;
    }

    this->m_s_score_digit_1->setTextureRect(sf::IntRect(charactersSet[currentScore[0]][0],charactersSet[currentScore[0]][1],8,8));
    this->m_s_score_digit_2->setTextureRect(sf::IntRect(charactersSet[currentScore[1]][0],charactersSet[currentScore[1]][1],8,8));
    this->m_s_score_digit_3->setTextureRect(sf::IntRect(charactersSet[currentScore[2]][0],charactersSet[currentScore[2]][1],8,8));
    this->m_s_score_digit_4->setTextureRect(sf::IntRect(charactersSet[currentScore[3]][0],charactersSet[currentScore[3]][1],8,8));
    this->m_s_score_digit_5->setTextureRect(sf::IntRect(charactersSet[currentScore[4]][0],charactersSet[currentScore[4]][1],8,8));
    this->m_s_score_digit_6->setTextureRect(sf::IntRect(charactersSet[currentScore[5]][0],charactersSet[currentScore[5]][1],8,8));

    this->m_s_score_digit_1->setPosition(24,24);
    this->m_s_score_digit_2->setPosition(32,24);
    this->m_s_score_digit_3->setPosition(40,24);
    this->m_s_score_digit_4->setPosition(48,24);
    this->m_s_score_digit_5->setPosition(56,24);
    this->m_s_score_digit_6->setPosition(64,24);
}

void Game::drawText() 
{
    this->m_window->draw(*this->m_s_hud);

    this->m_window->draw(*this->m_s_score_digit_1);
    this->m_window->draw(*this->m_s_score_digit_2);
    this->m_window->draw(*this->m_s_score_digit_3);
    this->m_window->draw(*this->m_s_score_digit_4);
    this->m_window->draw(*this->m_s_score_digit_5);
    this->m_window->draw(*this->m_s_score_digit_6);

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
            sf::Vector2f(-this->m_mario->getVelocityX(), 0));

    this->shiftObjectsBackward();
}

void Game::shiftObjectsBackward()
{
    SpriteObject *save_ptr = this->m_s_objects;

    for (int i = 0; i < m_map->getNumberOfGrids(); i++)
    {
        int nb = 0;
        while (nb < m_map->getNthGrid(i)->NB_SPRITES)
        {

            this->m_s_objects->body->getSprite()->move(
                sf::Vector2f(-this->m_mario->getVelocityX(), 0));

            this->m_s_objects++;
            nb++;
        }
    }
    this->m_s_objects = save_ptr;
}

void Game::drawObjects()
{
    SpriteObject *save_ptr = this->m_s_objects;

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