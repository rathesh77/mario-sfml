#ifndef CONSTANTS_H_
#define CONSTANTS_H_


#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 16*13

#define BOUNDING_X_LEFT 0
#define BOUNDING_Y_BOTTOM TILE_DIMENSION * 10
#define BOUNDING_X_MIDDLE WINDOW_WIDTH / 2.5

#define TILE_DIMENSION 16

class Constants
{
public:
    Constants();
    virtual ~Constants();
};


#endif /* CONSTANTS_H_ */
