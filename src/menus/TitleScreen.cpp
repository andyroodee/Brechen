#include "TitleScreen.h"
#include <tsu/anims/logxymover.h>
#include <tsu/anims/alphafader.h>
#include <tsu/triggers/death.h>

TitleScreen::TitleScreen()
{
    m_clickSound = new Sound("/rd/sounds/menuclick.wav");
    m_state = TitleScreen::AtTitleScreen;

    m_scene->setTranslate(Vector(320, 240, 9));
    setBg(0.0f, 0.0f, 0.0f);
    
    RefPtr<Texture> titleTexture = new Texture("/rd/title.png", true);
    titleTexture->setFilter(Texture::FilterNone);
    m_titleImage = new Banner(PVR_LIST_TR_POLY, titleTexture);
    m_scene->subAdd(m_titleImage);

    m_selectedColour = Color(0.95f, 0.41f, 0.0f);
    m_normalColour = Color(1.0f, 1.0f, 1.0f);

    RefPtr<Font> font = new Font("/rd/fonts/phatrave.txf");
    
    // Setup three labels and have them zoom in.
    m_startOption = new Label(font, "Push Start", 36, true, true);
    m_startOption->setTranslate(Vector(0, 600, 10));
    m_startOption->animAdd(new LogXYMover(0, 75));
    m_startOption->setTint(m_selectedColour);
    m_scene->subAdd(m_startOption);

    m_difficultyOptions[0] = new Label(font, "Easy", 36, true, true);
    m_difficultyOptions[0]->setTranslate(Vector(0, 600, 10));
    m_difficultyOptions[0]->setTint(m_normalColour);
    m_scene->subAdd(m_difficultyOptions[0]);    

    m_difficultyOptions[1] = new Label(font, "Medium", 36, true, true);
    m_difficultyOptions[1]->setTranslate(Vector(0, 600, 10));
    m_difficultyOptions[1]->setTint(m_normalColour);
    m_scene->subAdd(m_difficultyOptions[1]);

    m_difficultyOptions[2] = new Label(font, "Hard", 36, true, true);
    m_difficultyOptions[2]->setTranslate(Vector(0, 600, 10));
    m_difficultyOptions[2]->setTint(m_normalColour);
    m_scene->subAdd(m_difficultyOptions[2]);

    m_difficultySelection = 0;
}

void TitleScreen::inputEvent(const Event& evt)
{    
    if (evt.type != Event::EvtKeypress)
    {
        return;
    }

    m_clickSound->play();

    switch (evt.key)
    {
    case Event::KeyStart:
    case Event::KeySelect:
        if (m_state == TitleScreen::AtTitleScreen)
        {
            m_state = TitleScreen::AtDifficultySelect;
            // Slide out the start prompt.
            m_startOption->animAdd(new LogXYMover(0, 600));

            // Slide in the difficulty select options.
            for (int i = 0; i < NUM_OF_DIFFICULTIES; i++) 
            {          
                m_difficultyOptions[i]->animAdd(new LogXYMover(0, 100 + (i-1) * 50));     
            }
        }
        else
        {
            startExit();
        }
        break;
    case Event::KeyUp:
        if (m_state == TitleScreen::AtDifficultySelect)
        {
            m_difficultySelection--;

            if (m_difficultySelection < 0)
            {
                m_difficultySelection += NUM_OF_DIFFICULTIES;
            }
        }
        break;
    case Event::KeyDown:
        if (m_state == TitleScreen::AtDifficultySelect)
        {
            m_difficultySelection++;

            if (m_difficultySelection >= NUM_OF_DIFFICULTIES)
            {
                m_difficultySelection -= NUM_OF_DIFFICULTIES;
            }
         }
        break;
    default:
        break;
    }

    if (m_state == TitleScreen::AtDifficultySelect)
    {
        for (int i = 0; i < NUM_OF_DIFFICULTIES; i++) 
        {
            if (i == m_difficultySelection)
            {
                m_difficultyOptions[i]->setTint(m_selectedColour);
            }
            else
            {
                m_difficultyOptions[i]->setTint(m_normalColour);
            }
        }
    }
}

void TitleScreen::startExit()
{
    float fadeLength = -1.0f / 60;
    RefPtr<AlphaFader> titleFade = new AlphaFader(0.0f, fadeLength);
    titleFade->triggerAdd(new Death());
    m_titleImage->animAdd(titleFade);

    for (int i = 0; i < NUM_OF_DIFFICULTIES; i++) 
    {          
        m_difficultyOptions[i]->animAdd(new LogXYMover(0, 600));     
    }

    GenericMenu::startExit();
}