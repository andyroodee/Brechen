#ifndef __BRECHEN_PADDLE_H
#define __BRECHEN_PADDLE_H

#include <tsu/drawable.h>
#include <tsu/vector3.h>
#include <tsu/color.h>
#include <plx/list.h>
#include <plx/sprite.h>

class Paddle : public Drawable
{
public:
    Paddle();

    virtual ~Paddle() {}

    virtual void draw(int list);

    virtual void nextFrame()
    {
        Drawable::nextFrame();
    }

    void setSize(float w, float h);

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
};

#endif