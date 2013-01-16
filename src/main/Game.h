#ifndef __BRECHEN_GAME_H
#define __BRECHEN_GAME_H

#include "../drawables/Paddle.h"
#include "../drawables/Ball.h"
#include "Level.h"

class Game
{
public:
    Game();

    void CheckCollisions();
    void UpdateControls();

    void LoadLevel(int levelNumber);

    int GetScore() const
    {
        return m_score;
    }

    int GetLevelNumber() const
    {
        return m_levelNumber;
    }

    int GetLives() const
    {
        return m_lives;
    }

    Paddle* GetPaddle() const
    {
        return m_paddle;
    }

    Ball* GetBall() const
    {
        return m_ball;
    }

    Level* GetLevel() const
    {
        return m_currentLevel;
    }

private:
    void CreatePaddle();
    void CreateBall();

    int m_score;
    int m_levelNumber;
    int m_lives;
    RefPtr<Level> m_currentLevel;
    RefPtr<Paddle> m_paddle;
    RefPtr<Ball> m_ball;
};

#endif