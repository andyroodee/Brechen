#ifndef __BRECHEN_BRICK_H
#define __BRECHEN_BRICK_H

#include <tsu/drawable.h>

class Brick : public Drawable
{
public:
    Brick(int width, int height);

    virtual ~Brick() {}

    virtual void draw(int list);

    virtual void nextFrame()
    {
        Drawable::nextFrame();
    }

    void setType(unsigned char type);

    unsigned char getType() const
    {
        return m_type;
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
    unsigned char m_type;
    int m_width;
    int m_height;
};

#endif