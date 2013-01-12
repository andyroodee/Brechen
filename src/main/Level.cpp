#include "Level.h"

void Level::load(int levelNumber)
{
    char levelName[256];
    sprintf(levelName, "/cd/data/%s%d.txt", "level", levelNumber);

    FILE* file = fopen(levelName, "r");
    
    unsigned char brickValues[BRICK_COUNT_X * BRICK_COUNT_Y];
    fread(brickValues, sizeof(unsigned char), BRICK_COUNT_X * BRICK_COUNT_Y, file);
    
    fclose(file);

    // Fill out the actual Brick data
    for (int i = 0; i < BRICK_COUNT_Y; ++i)
    {
        for (int j = 0; j < BRICK_COUNT_X; ++j)
        {
            int index = i * BRICK_COUNT_X + j;
            m_bricks[index] = new Brick();
            m_bricks[index]->setType(brickValues[index]);
            m_bricks[index]->setTranslate(Vector(
                BRICK_START_X + j * (Brick::BRICK_WIDTH+1), 
                BRICK_START_Y + i * (Brick::BRICK_HEIGHT+1), 
                10.0f));
        }
    }
}

int Level::CheckCollision(Ball* ball)
{
    const Vector& ballPosition = ball->getPosition();
    const int HALF_BALL_HEIGHT = 16/2;
    const int HALF_BALL_WIDTH = 16/2;

    int score = 0;

    for (int i = 0; i < BRICK_COUNT_Y * BRICK_COUNT_X; ++i)
    {
        RefPtr<Brick> brick = m_bricks[i];
        if (brick->getType() != '0')
        {
            const Vector& brickPosition = brick->getPosition();

            if (ballPosition.y - HALF_BALL_HEIGHT <= brickPosition.y + Brick::BRICK_HEIGHT/2 &&
                ballPosition.y + HALF_BALL_HEIGHT >= brickPosition.y - Brick::BRICK_HEIGHT/2)
            {
                // We're lined up on the Y axis. Are we also within the X bounds?
                if (ballPosition.x + HALF_BALL_WIDTH >= brickPosition.x - Brick::BRICK_WIDTH/2 && 
                    ballPosition.x - HALF_BALL_WIDTH <= brickPosition.x + Brick::BRICK_WIDTH/2)
                {
                    //ball->setVelocity(-ball->getVelocity());
                    //Vector velocity = ball->getVelocity();
                    //velocity.x = -velocity.x;
                    //ball->setVelocity(velocity);

                    Vector hyp = brickPosition - ballPosition;
                    hyp.normalizeSelf();
                    hyp *= ball->getSpeed();
                    ball->setVelocity(-hyp); 

                    brick->setType('0');
                    brick->getParent()->subRemove(brick);
                    score += 20;
                }
            }
        }
    }

    return score;
}

void Level::addToScene(RefPtr<Scene> scene)
{
    for (int i = 0; i < BRICK_COUNT_Y * BRICK_COUNT_X; ++i)
    {
        scene->subAdd(m_bricks[i]);
    }
}