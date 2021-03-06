#include "Level.h"
#include "Game.h"
#include <math.h>
#include "../drawables/LaserBeam.h"

Level::Level(Game* parentGame)
{
    m_parentGame = parentGame;
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
    m_brickBounce = new Sound("/rd/sounds/brickbounce.wav");
}

bool Level::isCompleted() const
{
    return (m_destroyedBrickCount >= m_brickCount);
}

bool Level::load(int levelNumber)
{
    unload();

    char levelName[256];
    sprintf(levelName, "/cd/levels/%s%d", "level", levelNumber);

    FILE* file = fopen(levelName, "rb");

    if (!file)
    {
        return false;
    }
   
    fread(&m_brickCount, sizeof(int), 1, file);
   
    BrickRecord brickRecord;
    while (fread(&brickRecord, sizeof(BrickRecord), 1, file))
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
    }

    fclose(file);

    return true;
}

void Level::unload()
{
    m_bricks.delAll();
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
}

void Level::draw(int list)
{
    if (list != PVR_LIST_TR_POLY)
    {
        return;
    }

    ListNode<Brick>* t = m_bricks.getHead();
    ListNode<Brick>* n;
	while (t) 
    {
        n = t->getNext();
		if (!(*t)->isFinished())
		{
            (*t)->draw(list);
            (*t)->nextFrame();
        }
        else
        {
            if (t)
            {
                t->remove();
			    delete t; 
            }
        }
		t = n;
	}
}

int Level::checkCollision(LaserBeam* laser)
{
    if (!laser)
    {
        return 0;
    }
    int score = 0;

    ListNode<Brick>* brickNode = m_bricks.getHead();
    while (brickNode)
    {
        Brick* brick = brickNode->getData();

        if (brick && brick->getIsAlive() && laser->isHitting(brick))
        {
            // TODO: Play a special laser destruction sound.
            m_brickBounce->play();
                       
            // TODO: Animate in an interesting way
            brick->animateDestruction(Vector(0.0f, -1.0f, 0.0f), 5.0f);   
            
            score += brick->getValue();
            brickNode = brickNode->getNext();
            ++m_destroyedBrickCount;      
        }
        else
        {
            brickNode = brickNode->getNext();
        }
    }

    return score;
}

int Level::checkCollision(Ball* ball)
{
    if (!ball)
    {
        return 0;
    }

    const Vector& ballPosition = ball->getPosition();
    
    int score = 0;
    bool hasBounced = false;

    ListNode<Brick>* brickNode = m_bricks.getHead();
    while (brickNode)
    {
        Brick* brick = brickNode->getData();

        if (brick && brick->getIsAlive() && ball->intersectsWith(brick))
        {
            if (!hasBounced)
            {
                m_brickBounce->play();
            }

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
            
            if (m_parentGame->isPowerupActive(Powerup::Powerball))
            {                
                brick->animateDestruction(ball->getVelocity(), ball->getSpeed() * 9);
            }
            else
            {
                brick->animateDestruction(ball->getVelocity(), ball->getSpeed() * 3);   
                if (!hasBounced)
                {
                    if (m_parentGame->isPowerupActive(Powerup::RandomBounce))
                    {
                        ball->doRandomBounce();
                    }
                    else
                    {
                        ball->setVelocity(newBallVelocity);     
                    }                    
                }
            }           

            hasBounced = true;
            score += brick->getValue();
            brickNode = brickNode->getNext();
            ++m_destroyedBrickCount;      
        }
        else
        {
            brickNode = brickNode->getNext();
        }
    }

    return score;
}
