#ifndef __BRECHEN_GAME_H
#define __BRECHEN_GAME_H

#include <tsu/sound.h>
#include <tsu/drawable.h>
#include "../drawables/Paddle.h"
#include "../drawables/Ball.h"
#include "../drawables/Powerup.h"
#include "Level.h"

class Powerup;

class Game : public Drawable
{
public:

    enum Difficulty
    {
        Easy = 0,
        Medium,
        Hard
    };
    
    Game();

    void checkCollisions();
    void updateControls();

    void loadLevel(int levelNumber);

    void onLostBall();

    Difficulty getDifficulty() const
    {
        return m_difficulty;
    }

    void setDifficulty(Difficulty difficulty);

    int getScore() const
    {
        return m_score;
    }

    int getLevelNumber() const
    {
        return m_levelNumber;
    }

    int getLives() const
    {
        return m_lives;
    }

    Level* getLevel() const
    {
        return m_currentLevel;
    }

    bool isPowerupActive(Powerup::Effect effect) const
    {
        return m_activePowerups[(int)effect] > 0;
    }

    virtual void draw(int list);

private:
    void createPaddle();
    void createBall();
    void spawnPowerups();
    void checkPowerups();
    void activatePowerup(Powerup* powerup);

    unsigned int m_activePowerups[Powerup::BAD_EFFECT_MAX];
    unsigned int m_powerupActiveTime;
    unsigned int m_powerdownActiveTime;
    int m_score;
    int m_extraLifeScoreMultiplier;
    int m_extraLifeScoreIncrement;
    int m_levelNumber;
    int m_lives;
    RefPtr<Level> m_currentLevel;
    RefPtr<Paddle> m_paddle;
    RefPtr<Ball> m_ball;
    RefPtr<Sound> m_wallBounce;
    Difficulty m_difficulty;
    int m_powerupChance;
    int m_powerdownChance;
    List<Powerup> m_powerups;
};

#endif