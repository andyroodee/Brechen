#ifndef __BRECHEN_TITLESCREEN_H
#define __BRECHEN_TITLESCREEN_H

#include <tsu/genmenu.h>
#include <tsu/drawables/label.h>
#include <tsu/texture.h>
#include <tsu/drawables/banner.h>
#include <tsu/sound.h>

class TitleScreen : public GenericMenu, public RefCnt
{
public:
    TitleScreen();
    virtual ~TitleScreen() {}

    virtual void inputEvent(const Event & evt);    
    virtual void startExit();

    enum MenuState
    {
        AtTitleScreen,
        AtDifficultySelect
    };

    int getDifficultySelection() const
    {
        return m_difficultySelection;
    }

private:    
    static const int NUM_OF_DIFFICULTIES = 3;
    RefPtr<Banner> m_titleImage;
    RefPtr<Label> m_startOption;
    RefPtr<Label> m_difficultyOptions[NUM_OF_DIFFICULTIES];
    int m_difficultySelection;
    Color m_selectedColour;
    Color m_normalColour;
    MenuState m_state;
    RefPtr<Sound> m_clickSound;
};

#endif