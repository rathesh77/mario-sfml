#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

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
  void updateInfos();

  int m_nb_sprites = 0;
  int m_current_grid = 0;

  bool m_lost = false;

  sf::SoundBuffer buffer;
  sf::Sound sound;

  int m_score = 0;
  int m_coins = 0;
  std::string m_world = "0";
  int m_time = 0;  // in seconds
  int m_lives = 0;

  sf::Font *font = new sf::Font;

  std::map<char, std::vector<int> > charactersSet = {
      {'0', std::vector<int>{264 + (9 * 0), 8}},
      {'1', std::vector<int>{264 + (9 * 1), 8}},
      {'2', std::vector<int>{264 + (9 * 2), 8}},
      {'3', std::vector<int>{264 + (9 * 3), 8}},
      {'4', std::vector<int>{264 + (9 * 4), 8}},
      {'5', std::vector<int>{264 + (9 * 5), 8}},
      {'6', std::vector<int>{264 + (9 * 6), 8}},
      {'7', std::vector<int>{264 + (9 * 7), 8}},
      {'8', std::vector<int>{264 + (9 * 8), 8}},
      {'9', std::vector<int>{264 + (9 * 9), 8}},
      {'A', std::vector<int>{264 + (9 * 10), 8}},
      {'B', std::vector<int>{264 + (9 * 11), 8}},
      {'C', std::vector<int>{264 + (9 * 12), 8}},
      {'D', std::vector<int>{264 + (9 * 13), 8}},
      {'E', std::vector<int>{264 + (9 * 14), 8}},
      {'F', std::vector<int>{264 + (9 * 15), 8}},

      {'G', std::vector<int>{264 + (9 * 0), 17}},
      {'H', std::vector<int>{264 + (9 * 1), 17}},
      {'I', std::vector<int>{264 + (9 * 2), 17}},
      {'J', std::vector<int>{264 + (9 * 3), 17}},
      {'K', std::vector<int>{264 + (9 * 4), 17}},
      {'L', std::vector<int>{264 + (9 * 5), 17}},
      {'M', std::vector<int>{264 + (9 * 6), 17}},
      {'N', std::vector<int>{264 + (9 * 7), 17}},
      {'O', std::vector<int>{264 + (9 * 8), 17}},
      {'P', std::vector<int>{264 + (9 * 9), 17}},
      {'Q', std::vector<int>{264 + (9 * 10), 17}},
      {'R', std::vector<int>{264 + (9 * 11), 17}},
      {'S', std::vector<int>{264 + (9 * 12), 17}},
      {'T', std::vector<int>{264 + (9 * 13), 17}},
      {'U', std::vector<int>{264 + (9 * 14), 17}},
      {'V', std::vector<int>{264 + (9 * 15), 17}},

      {'W', std::vector<int>{264 + (9 * 0), 26}},
      {'X', std::vector<int>{264 + (9 * 1), 26}},
      {'Y', std::vector<int>{264 + (9 * 2), 26}},
      {'Z', std::vector<int>{264 + (9 * 3), 26}},
      {'-', std::vector<int>{264 + (9 * 4), 26}},
      {'x', std::vector<int>{264 + (9 * 5), 26}},
      {'!', std::vector<int>{264 + (9 * 6), 26}},
      {'.', std::vector<int>{264 + (9 * 7), 26}},
      {'_', std::vector<int>{264 + (9 * 8), 26}},
  };
};