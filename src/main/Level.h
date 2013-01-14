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
    static const int BRICK_START_X = 32;
    static const int BRICK_START_Y = 64;

    Level();

    void load(int levelNumber);

    bool isCompleted() const;

    void addToScene(RefPtr<Scene> scene);

    int CheckCollision(Ball* ball);

private:
    void unload();

    int m_brickCount;
    int m_destroyedBrickCount;
    RefPtr<Brick> m_bricks[BRICK_COUNT_X * BRICK_COUNT_Y];
};

#endif