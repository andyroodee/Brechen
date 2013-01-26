#include "UI.h"

UI::UI(Font* font, int fontSize)
{
    m_scoreLabel = new UILabel(font, fontSize);    
    m_scoreLabel->setTint(Color(0.1f, 0.9f, 0.4f));
    m_scoreLabel->setTranslate(Vector(60.0f, 40.0f, 10.0f));
    subAdd(m_scoreLabel);   

    m_levelLabel = new UILabel(font, fontSize);
    m_levelLabel->setTint(Color(0.1f, 0.9f, 0.4f));
    m_levelLabel->setTranslate(Vector(260.0f, 40.0f, 10.0f));
    subAdd(m_levelLabel); 
    
    m_livesLabel = new UILabel(font, fontSize);
    m_livesLabel->setTint(Color(0.1f, 0.9f, 0.4f));
    m_livesLabel->setTranslate(Vector(450.0f, 40.0f, 10.0f));
    subAdd(m_livesLabel);    
}

UI::~UI()
{
}

void UI::updateScoreLabel(int score)
{    
    m_scoreLabel->setText("%s%d", "Score: ", score);
}

void UI::updateLivesLabel(int lives)
{
    m_livesLabel->setText("%s%d", "Lives: ", lives);
}

void UI::updateLevelLabel(int level)
{
    m_levelLabel->setText("%s%d", "Level: ", level);
}