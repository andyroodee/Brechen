#ifndef __BRECHEN_BORDER_H
#define __BRECHEN_BORDER_H

#include <tsu/drawable.h>

class Border : public Drawable
{
public:
    static const int LEFT_BORDER = 40;
    static const int RIGHT_BORDER = 600;
    static const int TOP_BORDER = 64;
    static const int BOTTOM_BORDER = 480;
    static const int BORDER_SIZE = 8;

    Border();

    virtual ~Border() {}

    virtual void draw(int list);

private:
    void drawLeftBorder();
    void drawRightBorder();
    void drawTopBorder();
    void drawQuad(Vector coords[4], Color colours[4]);

    Vector leftBorderCoords[4];
    Vector rightBorderCoords[4];
    Vector topBorderCoords[4];

    Color leftBorderColours[4];
    Color rightBorderColours[4];
    Color topBorderColours[4];
};

#endif