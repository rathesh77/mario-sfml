#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define BOUNDING_X_LEFT 0
#define BOUNDING_Y_BOTTOM TILE_DIMENSION * 12
#define BOUNDING_X_MIDDLE WINDOW_WIDTH / 2.5

#define TILE_DIMENSION 16
#define WINDOW_WIDTH 16 * 16
#define WINDOW_HEIGHT 16 * 15

class Constants {
public:
  Constants();
  virtual ~Constants();
};

#endif /* CONSTANTS_H_ */
