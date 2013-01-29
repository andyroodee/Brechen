#include "Game.h"
#include <math.h>
#include <kos.h>
#include <tsu/texture.h>
#include <tsu/vector.h>
#include "../drawables/Border.h"
#include "../drawables/Powerup.h"
#include "../drawables/LaserBeam.h"
#include <stdlib.h>

Game::Game()
{
    for (int i = 0; i < Powerup::BAD_EFFECT_MAX; i++)
    {
        m_activePowerups[i] = 0;
    }

    m_score = 0;
    m_extraLifeScoreMultiplier = 1;
    m_extraLifeScoreIncrement = 1000;
    m_levelNumber = 1;
    m_lives = 3;
    m_lastLaserFireTime = LASER_FIRE_DELAY;
    m_difficulty = Game::Easy;
    m_laserTexture = new Texture("/rd/powerups/laser.png", true);
    m_extraBallTexture = new Texture("/rd/ball.png", true);
    m_wallBounce = new Sound("/rd/sounds/wallbounce.wav");
    m_laserSound = new Sound("/rd/sounds/laser.wav");
    m_lifeSound = new Sound("/rd/sounds/life.wav");
    m_deathSound = new Sound("/rd/sounds/death.wav");
    createPaddle();
    createBall("/rd/ball2.png");
    m_currentLevel = new Level(this);
    subAdd(m_currentLevel);
    m_currentLevel->load(m_levelNumber);
}

void Game::reset()
{        
    clearExtras();
    m_score = 0;
    m_extraLifeScoreMultiplier = 1;
    m_extraLifeScoreIncrement = 1000;
    m_levelNumber = 1;
    m_lives = 3;
    m_lastLaserFireTime = LASER_FIRE_DELAY;
    m_difficulty = Game::Easy;
    m_paddle->setTranslate(Vector(296.0f, 428.0f, 10.0f));        
    m_balls[0]->reset();      
    m_balls[0]->setTranslate(m_paddle->getPosition() + Vector(0.0f, -(m_balls[0]->getHeight() / 2), 0.0f));  
}

void Game::draw(int list)
{
    subRemoveFinished();
    subDraw(list);
    drawPowerups(list);
    drawLasers(list);
}

void Game::clearExtras()
{
    for (int i = 0; i < Powerup::BAD_EFFECT_MAX; i++)
    {
        m_activePowerups[i] = 0;
        deactivatePowerup((Powerup::Effect)i);
    } 

    ListNode<Powerup>* powerup = m_powerups.getHead();
    ListNode<Powerup>* nextPowerup;
	while (powerup) 
    {
        nextPowerup = powerup->getNext();
        (*powerup)->setFinished();		
		powerup = nextPowerup;
	}

    ListNode<LaserBeam>* laser = m_lasers.getHead();
    ListNode<LaserBeam>* nextLaser;
	while (laser) 
    {
        nextLaser = laser->getNext();
        (*laser)->setFinished();		
		laser = nextLaser;
	}
}

void Game::drawPowerups(int list)
{
    if (list != PVR_LIST_TR_POLY)
    {
        return;
    }

    ListNode<Powerup>* t = m_powerups.getHead();
    ListNode<Powerup>* n;
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

void Game::drawLasers(int list)
{
    if (list != PVR_LIST_TR_POLY)
    {
        return;
    }

    ListNode<LaserBeam>* t = m_lasers.getHead();
    ListNode<LaserBeam>* n;
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

void Game::createPaddle()
{
    RefPtr<Texture> paddleTexture = new Texture("/rd/paddle.png", true);
    paddleTexture->setFilter(Texture::FilterNone);
    m_paddle = new Paddle(paddleTexture);
    m_paddle->setTranslate(Vector(296.0f, 428.0f, 10.0f));
    m_paddle->setSize(48, 12);
    m_paddle->setSpeed(5.0f);
    subAdd(m_paddle);
}

void Game::createBall(const char* textureName)
{
    RefPtr<Texture> ballTexture = new Texture(textureName, true);
    m_balls[0] = new Ball(ballTexture);
    m_balls[0]->setTranslate(m_paddle->getPosition() + Vector(0.0f, -(m_balls[0]->getHeight() / 2), 0.0f));
    subAdd(m_balls[0]);
}

bool Game::loadLevel(int level)
{        
    clearExtras();
    m_lastLaserFireTime = LASER_FIRE_DELAY;
    m_balls[0]->reset();   
    switch (m_difficulty)
    {
    case Easy:
        m_balls[0]->setSpeed(4.0f);
        break;
    case Medium:
        m_balls[0]->setSpeed(5.0f);
        break;
    case Hard:
        m_balls[0]->setSpeed(6.0f);
        break;
    default:
        m_balls[0]->setSpeed(4.0f);
        break;
    }
    m_levelNumber = level;
    return m_currentLevel->load(m_levelNumber);
}

void Game::onLostBall()
{
    m_lives--;
    m_deathSound->play();
    if (m_lives >= 0)
    {
        m_balls[0]->setTranslate(m_paddle->getPosition() + Vector(0.0f, -(m_balls[0]->getHeight() / 2), 0.0f));
        m_balls[0]->reset();
    }
}

void Game::setDifficulty(Difficulty difficulty)
{
    m_difficulty = difficulty;
    switch (m_difficulty)
    {
    case Easy:
        m_paddle->setSpeed(6.0f);
        m_balls[0]->setSpeed(4.0f);
        m_extraLifeScoreIncrement = 1000;
        m_powerupChance = 15;
        m_powerdownChance = 5;
        m_powerupActiveTime = 5 * 60;
        m_powerdownActiveTime = 3 * 60;
        break;
    case Medium:
        m_paddle->setSpeed(5.0f);
        m_balls[0]->setSpeed(5.0f);
        m_extraLifeScoreIncrement = 2000;
        m_powerupChance = 10;
        m_powerdownChance = 10;
        m_powerupActiveTime = 4 * 60;
        m_powerdownActiveTime = 4 * 60;
        break;
    case Hard:
        m_paddle->setSpeed(4.0f);
        m_balls[0]->setSpeed(6.0f);
        m_extraLifeScoreIncrement = 4000;
        m_powerupChance = 5;
        m_powerdownChance = 15;
        m_powerupActiveTime = 3 * 60;
        m_powerdownActiveTime = 5 * 60;
        break;
    default:
        break;
    }
}

void Game::checkCollisions()
{
    if (!m_balls[0]->getIsLaunched())
    {
        return;
    }
        
    for (int i = 0; i < Powerup::BAD_EFFECT_MAX; i++)
    {
        if (m_activePowerups[i] > 0)
        {
            m_activePowerups[i]--;
            if (m_activePowerups[i] == 0)
            {
                deactivatePowerup((Powerup::Effect)i);
            }
        }
    }

    // Ball and Paddle collision.
    for (int i = 0; i < MAX_BALLS; i++)
    {
        if (!m_balls[i] || m_balls[i]->isFinished())
        {
            continue;
        }
        const Vector& ballPosition = m_balls[i]->getPosition();
        int halfBallWidth = m_balls[i]->getWidth() / 2;
        if (ballPosition.x - halfBallWidth <= Border::LEFT_BORDER || ballPosition.x + halfBallWidth >= Border::RIGHT_BORDER)
        {
            m_wallBounce->play();
            if (isPowerupActive(Powerup::RandomBounce))
            {
                m_balls[i]->doRandomBounce();
            }
            else
            {
                // Bouncing off a side wall. Reflect about the x axis.
                Vector velocity = m_balls[i]->getVelocity();
                velocity.x = -velocity.x;
                m_balls[i]->setVelocity(velocity);
            }
        
            // Make sure it doesn't get stuck in the wall...
            if (ballPosition.x - halfBallWidth <= Border::LEFT_BORDER)
            {
                m_balls[i]->setTranslate(Vector(Border::LEFT_BORDER + halfBallWidth, ballPosition.y, ballPosition.z));
            }
            else if (ballPosition.x + halfBallWidth >= Border::RIGHT_BORDER)
            {
                m_balls[i]->setTranslate(Vector(Border::RIGHT_BORDER - halfBallWidth, ballPosition.y, ballPosition.z));
            }
            
            continue;
        }

        int halfBallHeight = m_balls[i]->getHeight() / 2;
        if (ballPosition.y - halfBallHeight <= Border::TOP_BORDER)
        {
            m_wallBounce->play();
            if (isPowerupActive(Powerup::RandomBounce))
            {
                m_balls[i]->doRandomBounce();
                Vector velocity = m_balls[i]->getVelocity();
                if (velocity.y <= 0.0f)
                {
                    velocity.y = -velocity.y;
                    m_balls[i]->setVelocity(velocity);
                }
            }
            else
            {
                // Bouncing off the top wall. Reflect about the y axis.
                Vector velocity = m_balls[i]->getVelocity();
                velocity.y = -velocity.y;
                m_balls[i]->setVelocity(velocity);
            }

            // Make sure it doesn't get stuck in the wall...
            m_balls[i]->setTranslate(Vector(ballPosition.x, Border::TOP_BORDER + halfBallHeight, ballPosition.z));
            
            continue;
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
                m_balls[i]->setVelocity(-dir);    
            }
        }
    }

    // Handle the extra balls getting passed the paddle
    for (int i = 1; i < MAX_BALLS; i++)
    {
        if (m_balls[i] && !m_balls[i]->isFinished())
        {
            if (m_balls[i]->getPosition().y >= Border::BOTTOM_BORDER)
            {
                m_balls[i]->setFinished();
            }
        }
    }

    if (m_balls[0]->getPosition().y >= Border::BOTTOM_BORDER)
    {
        onLostBall();
        return;
    }

    // Check the bricks.
    int oldScore = m_score;
    for (int i = 0; i < MAX_BALLS; i++)
    {
        if (!m_balls[i] || m_balls[i]->isFinished())
        {
            continue;
        }
        m_score += m_currentLevel->checkCollision(m_balls[i]);
        
        // See if we should spawn a new powerup
        if (oldScore != m_score && !m_currentLevel->isCompleted())
        {
            spawnPowerups(m_balls[i]->getPosition());
        }
    }

    m_score += checkLaserBeamCollisions();

    // Check for powerup and paddle collisions
    checkPowerups();    

    if (m_score >= m_extraLifeScoreMultiplier * m_extraLifeScoreIncrement)
    {        
        m_lifeSound->play();
        m_lives++;
        m_extraLifeScoreMultiplier++;
    }
}

int Game::checkLaserBeamCollisions()
{
    int score = 0;

    ListNode<LaserBeam>* t = m_lasers.getHead();
    ListNode<LaserBeam>* n;
	while (t) 
    {
        n = t->getNext();
		if (!(*t)->isFinished())
		{
            LaserBeam* laser = (LaserBeam*)t->getData();
            int laserScore = m_currentLevel->checkCollision(laser);
            if (laserScore > 0)
            {
                laser->setFinished();                
            }
        }        
		t = n;
	}

    return score;
}

void Game::checkPowerups()
{
    const Vector& thePaddlePosition = m_paddle->getPosition();
    int halfPaddleHeight = m_paddle->getHeight() / 2;

    ListNode<Powerup>* t = m_powerups.getHead();
    ListNode<Powerup>* n;
	while (t) 
    {
        n = t->getNext();
		if (!(*t)->isFinished())
		{
            Powerup* powerup = (Powerup*)t->getData();
            Vector powerupPosition = powerup->getPosition();
            int halfPowerupHeight = powerup->getHeight() / 2;
            int halfPowerupWidth = powerup->getWidth() / 2;
            if (powerupPosition.y + halfPowerupHeight >= thePaddlePosition.y - halfPaddleHeight)
            {
                int halfPaddleWidth = m_paddle->getWidth() / 2;
                if (powerupPosition.x + halfPowerupWidth >= thePaddlePosition.x - halfPaddleWidth && 
                    powerupPosition.x - halfPowerupWidth <= thePaddlePosition.x + halfPaddleWidth)
                {
                    activatePowerup(powerup);
                }
            }
        }
		t = n;
	}
}

void Game::activatePowerup(Powerup* powerup)
{
    Powerup::Effect effect = powerup->getEffect();
        
    int bonus = 1;
    // Don't want to activate if we're already activated.
    if (!isPowerupActive(effect))
    {        
        // Set up any powerup-specific stuff in here.
        switch (effect)
        {
        case Powerup::ExtendPaddle:
            m_paddle->setScale(Vector(2.0f, 1.0f, 1.0f));
            break;
        case Powerup::ExtraBalls:
            for (int i = 1; i < MAX_BALLS; i++)
            {
                m_balls[i] = new Ball(m_extraBallTexture);
                m_balls[i]->setTranslate(m_balls[0]->getPosition());
                m_balls[i]->setSpeed(m_balls[0]->getSpeed());
                // Apply a random bounce, but make sure Y is moving up so
                // that the bonus balls don't immediately streak towards the
                // exit.
                m_balls[i]->doRandomBounce(true);
                subAdd(m_balls[i]);
            }
            // Little hack to make extra balls last a while longer than other powerups.
            // Ideally I'd have specified individual powerup activation times in an array or as part of a structure,
            // but I'm out of time.
            bonus = 10;
            break;
        case Powerup::Powerball:
            break;
        case Powerup::LaserPaddle:        
            break;   
        case Powerup::ShrinkPaddle:
            m_paddle->setScale(Vector(0.5f, 1.0f, 1.0f));
            break;
        case Powerup::DoubleBallSpeed:            
            m_previousBallSpeed = m_balls[0]->getSpeed();
            for (int i = 0; i < MAX_BALLS; i++)
            {
                if (!m_balls[i] || m_balls[i]->isFinished())
                {
                    continue;
                }
                m_balls[i]->setSpeed(m_previousBallSpeed*1.5f);
                m_balls[i]->updateVelocity();
            }
            break;
        case Powerup::RandomBounce: 
            break;
        default:
            break;
        }
    }

    if ((int)effect < (int)Powerup::GOOD_EFFECT_MAX)
    {
        m_activePowerups[(int)effect] = m_powerupActiveTime * bonus;
    }
    else
    {
        m_activePowerups[(int)effect] = m_powerdownActiveTime;
    }

    powerup->setFinished();
}

void Game::deactivatePowerup(Powerup::Effect effect)
{
    // Undo whatever horrible thing you did for the powerup to work in here.
    switch (effect)
    {
    case Powerup::ExtendPaddle:
        m_paddle->setScale(Vector(1.0f, 1.0f, 1.0f));
        break;
    case Powerup::ShrinkPaddle:
        m_paddle->setScale(Vector(1.0f, 1.0f, 1.0f));
        break;
    case Powerup::DoubleBallSpeed:
        for (int i = 0; i < MAX_BALLS; i++)
        {
            if (!m_balls[i] || m_balls[i]->isFinished())
            {
                continue;
            }
            m_balls[i]->setSpeed(m_previousBallSpeed);  
            m_balls[i]->updateVelocity();
        }
        break;
    case Powerup::ExtraBalls:
        for (int i = 1; i < MAX_BALLS; i++)
        {
            if (m_balls[i])
            {
                m_balls[i]->setFinished();
            }
        }
        break;
    default:
        break;
    }
}

void Game::spawnPowerups(const Vector& position)
{
    int powerupThreshold = rand() % 100;
    if (powerupThreshold < m_powerupChance)
    {
        // Release a powerup
        Powerup::Effect effect = (Powerup::Effect)(rand() % Powerup::GOOD_EFFECT_MAX);
        if (isPowerupActive(effect))
        {
            return;
        }
        RefPtr<Powerup> powerup = new Powerup(effect, position);
        m_powerups.insertHead(powerup);
        return;
    }

    int powerdownThreshold = rand() % 100;
    if (powerdownThreshold < m_powerdownChance)
    {
        // Release a powerdown
        Powerup::Effect effect = (Powerup::Effect)(rand() % (Powerup::BAD_EFFECT_MAX - (Powerup::GOOD_EFFECT_MAX + 1)) + (Powerup::GOOD_EFFECT_MAX + 1));
        if (isPowerupActive(effect))
        {
            return;
        }
        RefPtr<Powerup> powerup = new Powerup(effect, position);
        m_powerups.insertHead(powerup);
        return;
    }    
}

void Game::updateControls()
{
    maple_device_t* cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
    cont_state_t* state = (cont_state_t *)maple_dev_status(cont);
          
    if (state->buttons & CONT_A)
    {
        if (!m_balls[0]->getIsLaunched())
        {
            m_balls[0]->launch();
        }
        if (isPowerupActive(Powerup::LaserPaddle))
        {
            if (m_lastLaserFireTime >= LASER_FIRE_DELAY)
            {
                m_lastLaserFireTime = 0;
                m_laserSound->play();
                RefPtr<LaserBeam> laser = new LaserBeam(m_laserTexture);
                laser->setTranslate(m_paddle->getPosition());
                m_lasers.insertHead(laser);
            }
        }
    }
              
    if (isPowerupActive(Powerup::LaserPaddle))
    {
        m_lastLaserFireTime++;
    }

    Vector paddlePosition = m_paddle->getPosition();

    if (fabs(state->joyx) > 1.0f)
    {
        paddlePosition.x += (state->joyx / 128.0f) * m_paddle->getSpeed();
        int halfWidth = m_paddle->getWidth() / 2;
        if (paddlePosition.x + halfWidth >= Border::RIGHT_BORDER)
        {
            paddlePosition.x = Border::RIGHT_BORDER - halfWidth;
        }
        if (paddlePosition.x - halfWidth <= Border::LEFT_BORDER)
        {
            paddlePosition.x = Border::LEFT_BORDER + halfWidth;
        }
    }
    else if (state->buttons & CONT_DPAD_LEFT)
    {
        paddlePosition.x -= m_paddle->getSpeed();
        int halfWidth = m_paddle->getWidth() / 2;
        if (paddlePosition.x - halfWidth <= Border::LEFT_BORDER)
        {
            paddlePosition.x = Border::LEFT_BORDER + halfWidth;
        }
    }  

    else if (state->buttons & CONT_DPAD_RIGHT)
    {
        paddlePosition.x += m_paddle->getSpeed();
        int halfWidth = m_paddle->getWidth() / 2;
        if (paddlePosition.x + halfWidth >= Border::RIGHT_BORDER)
        {
            paddlePosition.x = Border::RIGHT_BORDER - halfWidth;
        }
    }      
        
    if (!m_balls[0]->getIsLaunched())
    {
        m_balls[0]->setTranslate(paddlePosition + Vector(0.0f, -(m_balls[0]->getHeight()), 0.0f));
    }

    m_paddle->setTranslate(paddlePosition); 
}