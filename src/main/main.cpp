#include <kos.h>
#include <tsu/texture.h>
#include <tsu/drawables/banner.h>
#include "../drawables/UI.h"
#include "../drawables/Border.h"
#include "../menus/TitleScreen.h"
#include "Level.h"
#include "SceneManager.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_NO_DCLOAD | INIT_QUIET);

bool hasColorReachedTarget(float color, float targetColor, float increment)
{
    return (fabs(color - targetColor) <= increment);    
}

float moveColorTowardsTarget(float color, float targetColor, float increment)
{
    float newColor = color;
    if (newColor < targetColor)
    {
        newColor += increment;
    }
    else
    {
        newColor -= increment;
    }
    return newColor;
}

Color adjustBGColor(const Color& currentBGColor, Color& bgTint)
{    
    static float tintIncrement = 1.0f / (3 * 60);
    Color newColor = currentBGColor;

    if (hasColorReachedTarget(newColor.r, bgTint.r, tintIncrement))
    {
        bgTint.r = rand() % 256 / 255.0f;
    }        
    newColor.r = moveColorTowardsTarget(newColor.r, bgTint.r, tintIncrement);

    if (hasColorReachedTarget(newColor.g, bgTint.g, tintIncrement))
    {
        bgTint.g = rand() % 256 / 255.0f;
    }   
    newColor.g = moveColorTowardsTarget(newColor.g, bgTint.g, tintIncrement);

    if (hasColorReachedTarget(newColor.b, bgTint.b, tintIncrement))
    {
        bgTint.b = rand() % 256 / 255.0f;
    }        
    newColor.b = moveColorTowardsTarget(newColor.b, bgTint.b, tintIncrement);

    return newColor;
}

int main(int argc, char** argv)
{        
    pvr_init_defaults();     

    srand(time(0));
    
    SceneManager sceneManager;
    sceneManager.setup();

    RefPtr<TitleScreen> titleScreen = new TitleScreen();
    titleScreen->doMenu();
    int difficulty = titleScreen->getDifficultySelection();
    
    pvr_set_bg_color(0.0f, 0.0f, 0.2f);
    
    RefPtr<Game> game = new Game();
    game->setDifficulty((Game::Difficulty)difficulty);    
    sceneManager.addDrawable(game);
    
    RefPtr<Font> font = new Font("/rd/fonts/arista.txf");
        
    RefPtr<UI> mainUI = new UI(font, 12);
    mainUI->updateScoreLabel(game->getScore());
    mainUI->updateLivesLabel(game->getLives());
    mainUI->updateLevelLabel(game->getLevelNumber());
    sceneManager.addDrawable(mainUI);   

    RefPtr<Border> border = new Border();
    sceneManager.addDrawable(border);
    
    RefPtr<Texture> bgTexture = new Texture("/rd/bg.png", true);
    bgTexture->setFilter(Texture::FilterNone);
    RefPtr<Banner> bg = new Banner(PVR_LIST_TR_POLY, bgTexture);
    bg->setTranslate(Vector(
        Border::LEFT_BORDER + bgTexture->getW() / 2, 
        Border::TOP_BORDER + bgTexture->getH() / 2, 9));
    Color bgTint(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);   

    sceneManager.addDrawable(bg);
            
    bool done = false;
    while (!done) 
    {
        bool backToTitleScreen = true;

        if (game->getLives() > 0)
        {
            backToTitleScreen = false;
            
            Color newBGColor = adjustBGColor(bg->getTint(), bgTint);
            bg->setTint(newBGColor);

            sceneManager.draw();

            int oldScore = game->getScore();
            int oldLives = game->getLives();

            game->checkCollisions();
                
            if (oldScore != game->getScore())
            {
                mainUI->updateScoreLabel(game->getScore());
            }

            if (oldLives != game->getLives())
            {
                mainUI->updateLivesLabel(game->getLives());
            }

            game->updateControls();

            if (game->getLevel()->isCompleted())
            {            
                if (game->loadLevel(game->getLevelNumber() + 1))
                {
                    mainUI->updateLevelLabel(game->getLevelNumber());
                }
                else
                {
                    backToTitleScreen = true;
                }
            }
        }
        if (backToTitleScreen)
        {            
            titleScreen = new TitleScreen();
            titleScreen->doMenu();
            int difficulty = titleScreen->getDifficultySelection();     
            game->reset();            
            game->setDifficulty((Game::Difficulty)difficulty);    
            game->loadLevel(1);
            mainUI->updateScoreLabel(game->getScore());
            mainUI->updateLivesLabel(game->getLives());
            mainUI->updateLevelLabel(game->getLevelNumber());
            pvr_set_bg_color(0.0f, 0.0f, 0.2f);
            backToTitleScreen = false;
        }
    }
    
    sceneManager.teardown();

    return 0;
}