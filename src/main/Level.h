#ifndef __BRECHEN_LEVEL_H
#define __BRECHEN_LEVEL_H

#include "../drawables/Brick.h"
#include "../drawables/Ball.h"
#include <tsu/drawables/scene.h>

class Level 
{
public:
    // Number of bricks in a level, horizontally and vertically.
    static const int BRICK_COUNT_X = 12;
    static const int BRICK_COUNT_Y = 18;

    // Where the bricks start drawing, in screen pixel coordinates.
    static const float BRICK_START_X = 320 - (BRICK_COUNT_X * Brick::BRICK_WIDTH / 2.0f) + Brick::BRICK_WIDTH / 2.0f - (BRICK_COUNT_X - 1);
    static const float BRICK_START_Y = 100.0f + Brick::BRICK_HEIGHT/2;

    void load(int levelNumber);

    void addToScene(RefPtr<Scene> scene);

    int CheckCollision(Ball* ball);

private:
    RefPtr<Brick> m_bricks[BRICK_COUNT_X * BRICK_COUNT_Y];
};

#endif