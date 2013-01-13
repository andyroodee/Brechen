#include "Level.h"

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

void Level::load(int levelNumber)
{
    char levelName[256];
    sprintf(levelName, "/cd/data/%s%d.txt", "level", levelNumber);

    FILE* file = fopen(levelName, "rb");
    
   // unsigned char brickValues[BRICK_COUNT_X * BRICK_COUNT_Y];
    //fread(brickValues, sizeof(unsigned char), BRICK_COUNT_X * BRICK_COUNT_Y, file);
    // Each brick record is 8 bytes
   
    int index = 0;
    BrickRecord brickRecord;
    while (fread(&brickRecord, sizeof(BrickRecord), 1, file))
    {        
        RefPtr<Brick> brick = new Brick(brickRecord.width * 8, brickRecord.height * 8);     
        brick->setType('1');
        brick->setTint(Color(brickRecord.a / 255.0f, brickRecord.r / 255.0f,
            brickRecord.g / 255.0f, brickRecord.b / 255.0f));
        brick->setTranslate(Vector(
            BRICK_START_X + brickRecord.xPos * 8 + 1,
            BRICK_START_Y + brickRecord.yPos * 8 + 1,
            10.0f));

        m_bricks[index] = brick;
        index++;
    }
    
    fclose(file);

    /*const int DEFAULT_BRICK_WIDTH = 36;
    const int DEFAULT_BRICK_HEIGHT = 12;

    // Fill out the actual Brick data
    for (int i = 0; i < BRICK_COUNT_Y; ++i)
    {
        for (int j = 0; j < BRICK_COUNT_X; ++j)
        {
            int index = i * BRICK_COUNT_X + j;
            m_bricks[index] = new Brick(DEFAULT_BRICK_WIDTH, DEFAULT_BRICK_HEIGHT);
            m_bricks[index]->setType(brickValues[index]);
            m_bricks[index]->setTranslate(Vector(
                BRICK_START_X + j * (DEFAULT_BRICK_WIDTH+1), 
                BRICK_START_Y + i * (DEFAULT_BRICK_HEIGHT+1), 
                10.0f));
        }
    }*/
}

int Level::CheckCollision(Ball* ball)
{
    const Vector& ballPosition = ball->getPosition();
    const int HALF_BALL_HEIGHT = 16/2;
    const int HALF_BALL_WIDTH = 16/2;

    int score = 0;

    for (int i = 0; i < BRICK_COUNT_Y * BRICK_COUNT_X; ++i)
    {
        if (!m_bricks[i])
        {
            continue;
        }
        RefPtr<Brick> brick = m_bricks[i];
        if (brick->getType() != '0')
        {
            const Vector& brickPosition = brick->getPosition();
            int halfBrickHeight = brick->getHeight() / 2;
            int halfBrickWidth = brick->getWidth() / 2;

            if (ballPosition.y - HALF_BALL_HEIGHT <= brickPosition.y + halfBrickHeight &&
                ballPosition.y + HALF_BALL_HEIGHT >= brickPosition.y - halfBrickHeight)
            {
                // We're lined up on the Y axis. Are we also within the X bounds?
                if (ballPosition.x + HALF_BALL_WIDTH >= brickPosition.x - halfBrickHeight && 
                    ballPosition.x - HALF_BALL_WIDTH <= brickPosition.x + halfBrickHeight)
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
        if (m_bricks[i])            
        {
            scene->subAdd(m_bricks[i]);
        }
    }
}