#include "Level.h"
#include <math.h>

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

Level::Level()
{
    m_bricks = 0;
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
}

bool Level::isCompleted() const
{
    return (m_brickCount == m_destroyedBrickCount);
}

void Level::load(int levelNumber)
{
    unload();

    char levelName[256];
    sprintf(levelName, "/cd/levels/%s%d", "level", levelNumber);

    FILE* file = fopen(levelName, "rb");
   
    int numberOfBrickRecords = 0;
    fread(&numberOfBrickRecords, sizeof(int), 1, file);
   
    m_bricks = new RefPtr<Brick>[numberOfBrickRecords];

    BrickRecord brickRecord;
    while (m_brickCount < numberOfBrickRecords && fread(&brickRecord, sizeof(BrickRecord), 1, file))
    {        
        RefPtr<Brick> brick = new Brick(brickRecord.width * 8, brickRecord.height * 8);     
        brick->setType('1');

        // GPU is not happy to recieve 0 / 255 for a color value!
        if (brickRecord.a < 1) brickRecord.a = 1;
        if (brickRecord.r < 1) brickRecord.r = 1;
        if (brickRecord.g < 1) brickRecord.g = 1;
        if (brickRecord.b < 1) brickRecord.b = 1;

        brick->setTint(Color(brickRecord.a / 255.0f, brickRecord.r / 255.0f,
            brickRecord.g / 255.0f, brickRecord.b / 255.0f));

        brick->setTranslate(Vector(
            BRICK_START_X + brickRecord.xPos * 8 + brickRecord.width * 4,
            BRICK_START_Y + brickRecord.yPos * 8 + brickRecord.height * 4,
            10.0f));

        m_bricks[m_brickCount++] = brick;
    }

    fclose(file);
}

void Level::unload()
{
    if (m_bricks)
    {
        delete [] m_bricks;
    }
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
}

int Level::CheckCollision(Ball* ball)
{
    const Vector& ballPosition = ball->getPosition();
    const int HALF_BALL_HEIGHT = 16/2;
    const int HALF_BALL_WIDTH = 16/2;

    int score = 0;

    for (int i = 0; i < m_brickCount; ++i)
    {
        if (!m_bricks[i])
        {
            continue;
        }
        RefPtr<Brick> brick = m_bricks[i];
        if (brick->getType() != '0')
        {
            const Vector& brickPosition = brick->getPosition();

            if (fabs(ballPosition.y - brickPosition.y) <= brick->getHeight() && 
                fabs(ballPosition.x - brickPosition.x) <= brick->getWidth())
            {
                Vector hyp = brickPosition - ballPosition;
                hyp.normalizeSelf();
                hyp *= ball->getSpeed();
                //ball->setVelocity(-hyp); 

                brick->setType('0');
                brick->getParent()->subRemove(brick);                    
                score += 20;
                ++m_destroyedBrickCount;                
            }
        }
    }

    return score;
}

void Level::addToScene(RefPtr<Scene> scene)
{
    for (int i = 0; i < m_brickCount; ++i)
    {
        if (m_bricks[i])            
        {
            scene->subAdd(m_bricks[i]);
        }
    }
}