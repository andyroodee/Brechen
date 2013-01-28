#include "UILabel.h"
#include <plx/list.h>

UILabel::UILabel(Font* font, int fontSize)
{
    m_font = font;
    m_fontSize = fontSize;
}

UILabel::~UILabel()
{
}

void UILabel::setText(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(m_text, MAX_LABEL_SIZE, format, args);
    va_end(args);
}

void UILabel::draw(int list) 
{
	if (list != PLX_LIST_TR_POLY)
	{
        return;
    }

	const Vector & p = getPosition();
	Color t = getColor();

	m_font->setSize(m_fontSize);
	m_font->setAlpha(t.a);
	m_font->setColor(t.r, t.g, t.b);
	m_font->smearDraw(p.x, p.y, p.z, m_text);
}