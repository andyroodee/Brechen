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

    virtual void nextFrame()
    {
        Drawable::nextFrame();
    }

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

private:
    int m_width;
    int m_height;
};

#endif