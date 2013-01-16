#ifndef __BRECHEN_UI_H
#define __BRECHEN_UI_H

#include <tsu/drawable.h>
#include "UILabel.h"

class UI : public Drawable
{
public:
    UI(Font* font, int fontSize);
    virtual ~UI();

    void UpdateScoreLabel(int score);
    void UpdateLivesLabel(int lives);
    void UpdateLevelLabel(int level);

private:
    RefPtr<UILabel> m_scoreLabel;
    RefPtr<UILabel> m_levelLabel;
    RefPtr<UILabel> m_livesLabel;
};

#endif