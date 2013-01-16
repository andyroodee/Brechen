#ifndef __BRECHEN_UILABEL_H
#define __BRECHEN_UILABEL_H

#include <stdarg.h>
#include <tsu/font.h>
#include <tsu/drawable.h>

class UILabel : public Drawable
{
public:
    static const int MAX_LABEL_SIZE = 64;

    UILabel(Font* font, int fontSize);
    virtual ~UILabel();

    void setText(const char* format, ...);
    
	virtual void draw(int list);

private:
	RefPtr<Font> m_font;
    char m_text[MAX_LABEL_SIZE];
    int m_fontSize;
};

#endif