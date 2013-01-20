#ifndef __BRECHEN_PADDLE_H
#define __BRECHEN_PADDLE_H

#include <tsu/drawable.h>
#include <tsu/vector.h>
#include <tsu/texture.h>
#include <tsu/color.h>
#include <plx/list.h>
#include <plx/sprite.h>

class Paddle : public Drawable
{
public:    
    Paddle(Texture* texture);

    virtual ~Paddle() {}

    virtual void draw(int list);

    void setSize(float width, float height)
    {
        m_width = width;
        m_height = height;
    }

    float getWidth() const
    {
        return m_width;
    }

    float getHeight() const
    {
        return m_height;
    }

    float getSpeed() const
    {
        return m_speed;
    }

    void setSpeed(float speed)
    {
        m_speed = speed;
    }

private:
    int m_width;
    int m_height;
    float m_speed;
    RefPtr<Texture>	m_texture;
	float m_u1, m_v1, m_u2, m_v2;
	float m_u3, m_v3, m_u4, m_v4;
};

#endif