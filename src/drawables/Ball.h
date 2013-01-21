#ifndef __BRECHEN_BALL_H
#define __BRECHEN_BALL_H

#include <tsu/drawable.h>
#include <tsu/texture.h>
#include <tsu/vector.h>
#include <plx/list.h>
#include <plx/sprite.h>

class Brick;

class Ball : public Drawable
{
public:
    Ball(Texture* texture);

    virtual ~Ball() {}

    virtual void draw(int list);

    void reset()
    {
        m_isLaunched = false;
        m_velocity = Vector(0.0f, 0.0f, 0.0f);
    }

    bool getIsLaunched() const
    {
        return m_isLaunched;
    }
    
    void launch();

    float getWidth() const;
    float getHeight() const;

    const Vector& getVelocity() const
    {
        return m_velocity;
    }

    void setVelocity(const Vector& velocity)
    {
        m_velocity = velocity;
        m_velocity.normalizeSelf();
        m_velocity *= m_speed;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    void setSpeed(float speed)
    {
        m_speed = speed;
    }

    bool intersectsWith(Brick* brick) const;

private:
    bool m_isLaunched;
    float m_radius;
    float m_speed;
    Vector m_velocity;
    RefPtr<Texture>	m_texture;

	float m_u1, m_v1, m_u2, m_v2;
	float m_u3, m_v3, m_u4, m_v4;
};

#endif