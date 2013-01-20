#ifndef __BRECHEN_UI_H
#define __BRECHEN_UI_H

#include <tsu/drawable.h>
#include "UILabel.h"

class UI : public Drawable
{
public:
    UI(Font* font, int fontSize);
    virtual ~UI();

    void updateScoreLabel(int score);
    void updateLivesLabel(int lives);
    void updateLevelLabel(int level);

private:
    RefPtr<UILabel> m_scoreLabel;
    RefPtr<UILabel> m_levelLabel;
    RefPtr<UILabel> m_livesLabel;
};

#endif