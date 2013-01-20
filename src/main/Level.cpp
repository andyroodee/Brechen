#include "Level.h"
#include <math.h>

Level::Level()
{
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
    m_brickBounce = new Sound("/rd/sounds/brickbounce.wav");
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
   
    BrickRecord brickRecord;
    while (m_brickCount < numberOfBrickRecords && fread(&brickRecord, sizeof(BrickRecord), 1, file))
    {        
        RefPtr<Brick> brick = new Brick(brickRecord.width * 8, brickRecord.height * 8);    

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

        m_bricks.insertHead(brick);
        ++m_brickCount;
    }

    fclose(file);
}

void Level::unload()
{
    m_bricks.delAll();
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
}

void Level::draw(int list)
{
    ListNode<Drawable> *t = (ListNode<Drawable>*)m_bricks.getHead();
	while (t) 
    {
		if (!(*t)->isFinished())
		{
            (*t)->draw(list);
        }
		t = t->getNext();
	}
}

int Level::checkCollision(Ball* ball)
{
    const Vector& ballPosition = ball->getPosition();
    
    int score = 0;

    ListNode<Brick>* brickNode = m_bricks.getHead();
    while (brickNode)
    {
        Brick* brick = brickNode->getData();

        if (ball->intersectsWith(brick))
        {
            m_brickBounce->play();

            const Vector& brickPosition = brick->getPosition();
            const int halfBrickHeight = brick->getHeight() / 2;
            const int halfBrickWidth = brick->getWidth() / 2;
            
            const float brickTop = brickPosition.y - halfBrickHeight;
            const float brickBottom = brickPosition.y + halfBrickHeight;
            const float brickLeft = brickPosition.x - halfBrickWidth;
            const float brickRight = brickPosition.x + halfBrickWidth;
                
            // What side of the brick did the ball hit?
            const bool onLeftSide = ballPosition.x < brickLeft;
            const bool onRightSide = ballPosition.x > brickRight;
            const bool onTopSide = ballPosition.y < brickTop;
            const bool onBottomSide = ballPosition.y > brickBottom;

            Vector newBallVelocity = ball->getVelocity();

            if (onLeftSide || onRightSide)
            {
                if (onTopSide || onBottomSide)
                {
                    // Corner.
                    newBallVelocity = -newBallVelocity;
                }
                else
                {
                    // One of the sides.
                    newBallVelocity.x = -newBallVelocity.x;
                }
            }
            else
            {
                // Top or bottom.
                newBallVelocity.y = -newBallVelocity.y;
            }

            ball->setVelocity(newBallVelocity); 
                 
            score += brick->getValue();
            brickNode = brickNode->getNext();
            m_bricks.del(brick);  
            ++m_destroyedBrickCount;      
        }
        else
        {
            brickNode = brickNode->getNext();
        }
    }

    return score;
}
