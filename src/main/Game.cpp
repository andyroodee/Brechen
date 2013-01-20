#include "Game.h"

#include <kos.h>
#include <tsu/texture.h>
#include <tsu/vector.h>
#include <oggvorbis/sndoggvorbis.h>
#include "../drawables/Border.h"

Game::Game()
{
    m_score = 0;
    m_levelNumber = 1;
    m_lives = 3;
    m_wallBounce = new Sound("/rd/sounds/wallbounce.wav");
    createPaddle();
    createBall();
    m_currentLevel = new Level();
    m_currentLevel->load(m_levelNumber);
}

void Game::createPaddle()
{
    RefPtr<Texture> paddleTexture = new Texture("/rd/paddle.png", true);
    paddleTexture->setFilter(Texture::FilterNone);
    m_paddle = new Paddle(paddleTexture);
    m_paddle->setTranslate(Vector(296.0f, 428.0f, 10.0f));
    m_paddle->setSize(48, 12);
    m_paddle->setSpeed(5.0f);
}

void Game::createBall()
{
    RefPtr<Texture> ballTexture = new Texture("/rd/ball.png", true);
    m_ball = new Ball(ballTexture);
    m_ball->setTranslate(m_paddle->getPosition() + Vector(0.0f, -(m_ball->getHeight() / 2), 0.0f));
}

void Game::loadLevel(int level)
{
    m_ball->reset();       
    m_levelNumber = level;
    m_currentLevel->load(m_levelNumber);
}

void Game::onLostBall()
{
    --m_lives;
    if (m_lives < 0)
    {
        // TODO: Game Over!
    }
    else
    {        
        m_ball->setTranslate(m_paddle->getPosition() + Vector(0.0f, -(m_ball->getHeight() / 2), 0.0f));
        m_ball->reset();
    }
}

void Game::checkCollisions()
{
    if (!m_ball->getIsLaunched())
    {
        return;
    }

    // Ball and Paddle collision.
    const Vector& ballPosition = m_ball->getPosition();
    int halfBallWidth = m_ball->getWidth() / 2;
    if (ballPosition.x - halfBallWidth <= Border::LEFT_BORDER || ballPosition.x + halfBallWidth >= Border::RIGHT_BORDER)
    {
        m_wallBounce->play();
        // Bouncing off a side wall. Reflect about the x axis.
        Vector velocity = m_ball->getVelocity();
        velocity.x = -velocity.x;
        m_ball->setVelocity(velocity);
        
        // Make sure it doesn't get stuck in the wall...
        if (ballPosition.x - halfBallWidth <= Border::LEFT_BORDER)
        {
            m_ball->setTranslate(ballPosition + Vector(1.0f, 0.0f, 0.0f));
        }
        else
        {
            m_ball->setTranslate(ballPosition + Vector(-1.0f, 0.0f, 0.0f));
        }
        return;
    }

    int halfBallHeight = m_ball->getHeight() / 2;
    if (ballPosition.y - halfBallHeight <= Border::TOP_BORDER)
    {
        m_wallBounce->play();
        // Bouncing off the top wall. Reflect about the y axis.
        Vector velocity = m_ball->getVelocity();
        velocity.y = -velocity.y;
        m_ball->setVelocity(velocity);

        // Make sure it doesn't get stuck in the wall...
        m_ball->setTranslate(ballPosition + Vector(0.0f, 1.0f, 0.0f));
        return;
    }
    
    if (ballPosition.y >= Border::BOTTOM_BORDER)
    {
        // Bouncing off the top wall. Reflect about the y axis.
        Vector velocity = m_ball->getVelocity();
        velocity.y = -velocity.y;
        m_ball->setVelocity(velocity);

        // Make sure it doesn't get stuck in the wall...
        m_ball->setTranslate(ballPosition + Vector(0.0f, -1.0f, 0.0f));
        return;
        //// We die!
        //onLostBall();
        //return;
    }

    const Vector& thePaddlePosition = m_paddle->getPosition();
    int halfPaddleHeight = m_paddle->getHeight() / 2;
    if (ballPosition.y + halfBallHeight >= thePaddlePosition.y - halfPaddleHeight)
    {
        int halfPaddleWidth = m_paddle->getWidth() / 2;
        // We're lined up on the Y axis. Are we also within the X bounds?
        if (ballPosition.x + halfBallWidth >= thePaddlePosition.x - halfPaddleWidth && 
            ballPosition.x - halfBallWidth <= thePaddlePosition.x + halfPaddleWidth)
        {
            m_wallBounce->play();
            Vector dir = thePaddlePosition - ballPosition;   
            dir.normalizeSelf();
            dir *= m_ball->getSpeed();
            m_ball->setVelocity(-dir);            
        }
    }

    // Check the bricks.
    m_score += m_currentLevel->checkCollision(m_ball);
}

void Game::updateControls()
{
    MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, t)
            
    if (t->buttons & CONT_A)
    {
        if (!m_ball->getIsLaunched())
        {
            m_ball->launch();
        }
    }

    Vector paddlePosition = m_paddle->getPosition();

    if (t->buttons & CONT_DPAD_LEFT)
    {
        paddlePosition.x -= m_paddle->getSpeed();
        int halfWidth = m_paddle->getWidth() / 2;
        if (paddlePosition.x - halfWidth <= Border::LEFT_BORDER)
        {
            paddlePosition.x = Border::LEFT_BORDER + halfWidth;
        }
    }  

    if (t->buttons & CONT_DPAD_RIGHT)
    {
        paddlePosition.x += m_paddle->getSpeed();
        int halfWidth = m_paddle->getWidth() / 2;
        if (paddlePosition.x + halfWidth >= Border::RIGHT_BORDER)
        {
            paddlePosition.x = Border::RIGHT_BORDER - halfWidth;
        }
    }  
        
    if (!m_ball->getIsLaunched())
    {
        m_ball->setTranslate(paddlePosition + Vector(0.0f, -(m_ball->getHeight()), 0.0f));
    }

    m_paddle->setTranslate(paddlePosition); 

    MAPLE_FOREACH_END() 
}