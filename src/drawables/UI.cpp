#include "UI.h"

UI::UI(Font* font, int fontSize)
{
    m_scoreLabel = new UILabel(font, fontSize);
    m_scoreLabel->setTranslate(Vector(80.0f, 40.0f, 10.0f));
    subAdd(m_scoreLabel);   

    m_levelLabel = new UILabel(font, fontSize);
    m_levelLabel->setTranslate(Vector(240.0f, 40.0f, 10.0f));
    subAdd(m_levelLabel); 
    
    m_livesLabel = new UILabel(font, fontSize);
    m_livesLabel->setTranslate(Vector(450.0f, 40.0f, 10.0f));
    subAdd(m_livesLabel);    
}

UI::~UI()
{
}

void UI::UpdateScoreLabel(int score)
{    
    m_scoreLabel->setText("%s%d", "Score: ", score);
}

void UI::UpdateLivesLabel(int lives)
{
    m_livesLabel->setText("%s%d", "Lives: ", lives);
}

void UI::UpdateLevelLabel(int level)
{
    m_levelLabel->setText("%s%d", "Level: ", level);
}