#include "Level.h"
#include <math.h>

Level::Level()
{
    m_brickCount = 0;
    m_destroyedBrickCount = 0;
}

bool Level::IsCompleted() const
{
    return (m_brickCount == m_destroyedBrickCount);
}

void Level::Load(int levelNumber)
{
    Unload();

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

void Level::Unload()
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

int Level::CheckCollision(Ball* ball)
{
    const Vector& ballPosition = ball->getPosition();

    int score = 0;
    
    ListNode<Brick>* brickNode = m_bricks.getHead();
	while (brickNode) 
    {
        Brick* brick = brickNode->getData();

        const Vector& brickPosition = brick->getPosition();

        // TODO: Take ball size into account?!
        if (fabs(ballPosition.y - brickPosition.y) <= brick->getHeight() && 
            fabs(ballPosition.x - brickPosition.x) <= brick->getWidth())
        {
            Vector hyp = brickPosition - ballPosition;
            hyp.normalizeSelf();
            hyp *= ball->getSpeed();
            //ball->setVelocity(-hyp); 

            brickNode = brickNode->getNext();
            m_bricks.del(brick);   
            score += 20;
            ++m_destroyedBrickCount;         
            continue;
        }

		brickNode = brickNode->getNext();
	}

    return score;
}