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

    std::string m_hudPath = "sprites/HUD.png";
    std::string m_backgroundPath = "sprites/background2_non_transparent.png";
    std::string m_skyPath = "sprites/background2_non_transparent.png";
    std::string m_brickPath = "sprites/bricks.png";
    std::string m_groundPath = "sprites/tileset.png";

    std::string m_ennemiesPath = "sprites/ennemies_transparent.png";

    sf::Sprite *m_s_background;
    sf::Sprite *m_s_sky;
    SpriteObject *m_s_objects = new SpriteObject[1];
    sf::Sprite *m_s_hud;

    sf::Sprite *m_s_score_digit_1;
    sf::Sprite *m_s_score_digit_2;
    sf::Sprite *m_s_score_digit_3;
    sf::Sprite *m_s_score_digit_4;
    sf::Sprite *m_s_score_digit_5;
    sf::Sprite *m_s_score_digit_6;


    sf::Texture m_t_sky;
    sf::Texture m_t_background;
    sf::Texture m_t_brick;
    sf::Texture m_t_ground;
    sf::Texture m_t_ennemies;
    sf::Texture m_t_hud;

    sf::RenderWindow *m_window;

    Mario *m_mario;

    Map *m_map;

    void drawSprites();
    void drawText();
    void printInfos();

    int m_nb_sprites = 0;
    int m_current_grid = 0;

    bool m_lost = false;

    //sf::SoundBuffer buffer;
    //sf::Sound sound;

    sf::Text m_text_score;
    sf::Text m_text_coins;
    sf::Text m_text_world;
    sf::Text m_text_time;
    sf::Text m_text_lives;

    int m_score = 0;
    int m_coins = 0;
    std::string m_world = "0";
    int m_time = 0; // in seconds
    int m_lives = 0;

    sf::Font *font = new sf::Font;

    std::map<char, std::vector<int> > charactersSet = {
        {'0', std::vector<int>(265+ (16 * 0), 7)},
        {'1', std::vector<int>(265+ (16 * 1), 7)},
        {'2', std::vector<int>(265+ (16 * 2), 7)},
        {'3', std::vector<int>(265+ (16 * 3), 7)},
        {'4', std::vector<int>(265+ (16 * 4), 7)},
        {'5', std::vector<int>(265+ (16 * 5), 7)},
        {'6', std::vector<int>(265+ (16 * 6), 7)},
        {'7', std::vector<int>(265+ (16 * 7), 7)},
        {'8', std::vector<int>(265+ (16 * 8), 7)},
        {'9', std::vector<int>(265+ (16 * 9), 7)},
        {'A', std::vector<int>(265+ (16 * 10), 7)},
        {'B', std::vector<int>(265+ (16 * 11), 7)},
        {'C', std::vector<int>(265+ (16 * 12), 7)},
        {'D', std::vector<int>(265+ (16 * 13), 7)},
        {'E', std::vector<int>(265+ (16 * 14), 7)},
        {'F', std::vector<int>(265+ (16 * 15), 7)},

        {'G', std::vector<int>(265+ (16 * 0), 14)},
        {'H', std::vector<int>(265+ (16 * 1), 14)},
        {'I', std::vector<int>(265+ (16 * 2), 14)},
        {'J', std::vector<int>(265+ (16 * 3), 14)},
        {'K', std::vector<int>(265+ (16 * 4), 14)},
        {'L', std::vector<int>(265+ (16 * 5), 14)},
        {'M', std::vector<int>(265+ (16 * 6), 14)},
        {'N', std::vector<int>(265+ (16 * 7), 14)},
        {'O', std::vector<int>(265+ (16 * 8), 14)},
        {'P', std::vector<int>(265+ (16 * 9), 14)},
        {'Q', std::vector<int>(265+ (16 * 10), 14)},
        {'R', std::vector<int>(265+ (16 * 11), 14)},
        {'S', std::vector<int>(265+ (16 * 12), 14)},
        {'T', std::vector<int>(265+ (16 * 13), 14)},
        {'U', std::vector<int>(265+ (16 * 14), 14)},
        {'V', std::vector<int>(265+ (16 * 15), 14)},

        {'W', std::vector<int>(265+ (16 * 0), 21)},
        {'X', std::vector<int>(265+ (16 * 1), 21)},
        {'Y', std::vector<int>(265+ (16 * 2), 21)},
        {'Z', std::vector<int>(265+ (16 * 3), 21)},
        {'-', std::vector<int>(265+ (16 * 4), 21)},
        {'x', std::vector<int>(265+ (16 * 5), 21)},
        {'!', std::vector<int>(265+ (16 * 6), 21)},
        {'.', std::vector<int>(265+ (16 * 7), 21)},
        {'_', std::vector<int>(265+ (16 * 8), 21)},
    };
};