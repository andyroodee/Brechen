#ifndef __BRECHEN_BRICK_H
#define __BRECHEN_BRICK_H

#include <tsu/drawable.h>

class Brick : public Drawable
{
public:
    static const int BRICK_WIDTH = 36;
    static const int BRICK_HEIGHT = 12;

    Brick();

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

private:
    unsigned char m_type;
};

#endif