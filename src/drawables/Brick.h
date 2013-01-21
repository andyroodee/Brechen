#ifndef __BRECHEN_BRICK_H
#define __BRECHEN_BRICK_H

#include <tsu/drawable.h>

class Brick : public Drawable
{
public:
    static const int BORDER_SIZE = 1;

    Brick(int width, int height);

    virtual ~Brick() {}

    virtual void draw(int list);

    void animateDestruction(const Vector& direction, const float power);

    int getWidth() const
    {
        return m_width;
    }

    void setWidth(int width)
    {
        m_width = width;
    }

    int getHeight() const
    {
        return m_height;
    }

    void setHeight(int height)
    {
        m_height = height;
    }

    int getValue() const
    {
        return m_value;
    }

    void setValue(int value)
    {
        m_value = value;
    }

    bool getIsAlive() const
    {
        return m_alive;
    }

private:
    int m_width;
    int m_height;
    int m_value;
    int m_alive;
};

#endif