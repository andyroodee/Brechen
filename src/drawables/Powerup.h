#ifndef __BRECHEN_POWERUP_H
#define __BRECHEN_POWERUP_H

#include <tsu/drawable.h>
#include <tsu/texture.h>
#include <tsu/vector.h>
#include <plx/list.h>
#include <plx/sprite.h>

class Powerup : public Drawable
{
public:
    enum Effect
    {
        ExtendPaddle = 0,
        ExtraBalls,
        Powerball,
        LaserPaddle,
        GOOD_EFFECT_MAX,
        ShrinkPaddle,
        DoubleBallSpeed,
        RandomBounce,
        BAD_EFFECT_MAX
    };    

    // TODO: Make this not terrible later. It's too late in the month now :o
    static const unsigned int EXTEND_PADDLE = 0;
    static const unsigned int EXTRA_BALLS = 1;
    static const unsigned int POWER_BALL = 2;
    static const unsigned int LASER_PADDLE = 3;
    static const unsigned int SHRINK_PADDLE = 4;
    static const unsigned int DOUBLE_BALL_SPEED = 5;
    static const unsigned int RANDOM_BOUNCE = 6;

    Powerup(Effect effect, const Vector& position);

    virtual ~Powerup() {}

    float getWidth() const;
    float getHeight() const;

    Effect getEffect() const
    {
        return m_effect;
    }

    virtual void draw(int list);

private:
    void loadTexture();

    Effect m_effect;
    RefPtr<Texture>	m_texture;
    Vector m_velocity;
	float m_u1, m_v1, m_u2, m_v2;
	float m_u3, m_v3, m_u4, m_v4;
};

#endif