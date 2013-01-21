#ifndef __BRECHEN_GAME_H
#define __BRECHEN_GAME_H

#include <tsu/sound.h>
#include "../drawables/Paddle.h"
#include "../drawables/Ball.h"
#include "Level.h"

class Game
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

    Paddle* getPaddle() const
    {
        return m_paddle;
    }

    Ball* getBall() const
    {
        return m_ball;
    }

    Level* getLevel() const
    {
        return m_currentLevel;
    }

private:
    void createPaddle();
    void createBall();

    int m_score;
    int m_levelNumber;
    int m_lives;
    RefPtr<Level> m_currentLevel;
    RefPtr<Paddle> m_paddle;
    RefPtr<Ball> m_ball;
    RefPtr<Sound> m_wallBounce;
    Difficulty m_difficulty;

};

#endif