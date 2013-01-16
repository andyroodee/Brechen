#include "Game.h"

#include <kos.h>
#include <tsu/texture.h>
#include <tsu/vector.h>

Game::Game()
{
    m_score = 0;
    m_levelNumber = 1;
    m_lives = 3;
    CreatePaddle();
    CreateBall();
    m_currentLevel = new Level();
    m_currentLevel->Load(m_levelNumber);
}

void Game::CreatePaddle()
{
    m_paddle = new Paddle();
    m_paddle->setTint(Color(1.0f, 0.0, 0.0f));
    m_paddle->setTranslate(Vector(296.0f, 420.0f, 10.0f));
    m_paddle->setSize(48, 12);
    m_paddle->setSpeed(5.0f);
}

void Game::CreateBall()
{
    RefPtr<Texture> ballTexture = new Texture("/rd/ball.png", true);
    m_ball = new Ball(ballTexture);
    m_ball->setTranslate(m_paddle->getPosition() + Vector(0.0f, -12.0f, 0.0f));
}

void Game::LoadLevel(int level)
{
    m_ball->reset();       
    m_levelNumber = level;
    m_currentLevel->Load(m_levelNumber);
}

void Game::CheckCollisions()
{
    // Ball and Paddle collision.
    // Hacky shit. Oh well.
    const Vector& ballPosition = m_ball->getPosition();
    if (ballPosition.x < 20 || ballPosition.x > 620)
    {
        // Bouncing off a side wall. Reflect about the x axis.
        Vector velocity = m_ball->getVelocity();
        velocity.x = -velocity.x;
        m_ball->setVelocity(velocity);
        return;
    }

    if (ballPosition.y < 50 || ballPosition.y > 450)
    {
        // Bouncing off the top wall. Reflect about the y axis.
        Vector velocity = m_ball->getVelocity();
        velocity.y = -velocity.y;
        m_ball->setVelocity(velocity);
        return;
    }

    const Vector& thePaddlePosition = m_paddle->getPosition();
    const int HALF_BALL_HEIGHT = 16/2;
    const int HALF_BALL_WIDTH = 16/2;
    const int HALF_PADDLE_HEIGHT = 12/2;
    const int HALF_PADDLE_WIDTH = 36/2;
    if (ballPosition.y + HALF_BALL_HEIGHT >= thePaddlePosition.y - HALF_PADDLE_HEIGHT)
    {
        // We're lined up on the Y axis. Are we also within the X bounds?
        if (ballPosition.x + HALF_BALL_WIDTH >= thePaddlePosition.x - HALF_PADDLE_WIDTH && 
            ballPosition.x - HALF_BALL_WIDTH <= thePaddlePosition.x + HALF_PADDLE_WIDTH)
        {
            Vector hyp = thePaddlePosition - ballPosition;   
            hyp.normalizeSelf();
            hyp *= m_ball->getSpeed();
            m_ball->setVelocity(-hyp);            
        }
    }

    // Check the bricks.
    m_score += m_currentLevel->CheckCollision(m_ball);
}

void Game::UpdateControls()
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
        if (paddlePosition.x < 20)
        {
            paddlePosition.x = 20;
        }
    }  

    if (t->buttons & CONT_DPAD_RIGHT)
    {
        paddlePosition.x += m_paddle->getSpeed();
        if (paddlePosition.x > 620)
        {
            paddlePosition.x = 620;
        }
    }  
        
    if (!m_ball->getIsLaunched())
    {
        m_ball->setTranslate(paddlePosition + Vector(0.0f, -12.0f, 0.0f));
    }

    m_paddle->setTranslate(paddlePosition); 

    MAPLE_FOREACH_END() 
}