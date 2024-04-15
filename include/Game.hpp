#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>

#include "Map.hpp"
#include "Mario.hpp"


class Game {
   public:
    int frameCount = 0;

    Game(sf::RenderWindow *);

    int getCurrentMap();
    void tick(sf::Clock *);
    void shiftSceneBackward();
    void shiftObjectsBackward();
    void drawObjects();

    void loadMap(Map *);
    void generateSpritesInMemory();

   private:
    int m_currentMap;
    int m_nb_grids;
    int m_single_background_width = TILE_DIMENSION * TILE_DIMENSION;

    std::string m_backgroundPath = "sprites/background2_non_transparent.png";
    std::string m_skyPath = "sprites/background2_non_transparent.png";
    std::string m_brickPath = "sprites/bricks.png";
    std::string m_groundPath = "sprites/tileset.png";

    std::string m_ennemiesPath = "sprites/ennemies_transparent.png";

    sf::Sprite *m_s_background;
    sf::Sprite *m_s_sky;

    SpriteObject *m_s_objects = new SpriteObject[1];

    sf::Texture m_t_sky;
    sf::Texture m_t_background;
    sf::Texture m_t_brick;
    sf::Texture m_t_ground;

    sf::Texture m_t_ennemies;

    sf::RenderWindow *m_window;

    Mario *m_mario;

    Map *m_map;

    void drawSprites();

    int m_nb_sprites = 0;
    int m_current_grid = 0;

    bool m_lost = false;

    //sf::SoundBuffer buffer;
    //sf::Sound sound;

};