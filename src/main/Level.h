#ifndef __BRECHEN_LEVEL_H
#define __BRECHEN_LEVEL_H

#include "../drawables/Brick.h"
#include "../drawables/Ball.h"
#include <tsu/drawables/scene.h>
#include <tsu/sound.h>

class Game;
class LaserBeam;

class Level : public Drawable
{
public:
    
    struct BrickRecord
    {
        unsigned char xPos;
        unsigned char yPos;
        unsigned char width;
        unsigned char height;
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };

    // Where the bricks start drawing, in screen pixel coordinates.
    static const int BRICK_START_X = 40;
    static const int BRICK_START_Y = 64;

    Level(Game* parent);

    void load(int levelNumber);

    bool isCompleted() const;

    int checkCollision(Ball* ball);

    int checkCollision(LaserBeam* laser);
    
    virtual void draw(int list);

private:
    void unload();

    List<Brick> m_bricks;
    int m_brickCount;
    int m_destroyedBrickCount;
    RefPtr<Sound> m_brickBounce;
    Game* m_parentGame;
};

#endif