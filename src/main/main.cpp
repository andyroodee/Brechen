#include <kos.h>
#include <tsu/texture.h>
#include <tsu/drawables/banner.h>
#include "../drawables/Paddle.h"
#include "../drawables/Brick.h"
#include "../drawables/Ball.h"
#include "../drawables/UI.h"
#include "../drawables/Border.h"
#include "../drawables/Powerup.h"
#include "../menus/TitleScreen.h"
#include "Level.h"
#include "SceneManager.h"
#include "Game.h"
#include <time.h>
#include <stdlib.h>

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_NO_DCLOAD | INIT_QUIET);

int main(int argc, char** argv)
{        
    pvr_init_defaults(); 

    srand(time(0));

    RefPtr<TitleScreen> titleScreen = new TitleScreen();
    titleScreen->doMenu();
    int difficulty = titleScreen->getDifficultySelection();
    
    SceneManager sceneManager;
    sceneManager.setup();
    
    RefPtr<Game> game = new Game();
    game->setDifficulty((Game::Difficulty)difficulty);
    sceneManager.addDrawable(game);
    
    RefPtr<Font> font = new Font("/rd/typewriter.txf");
    font->setColor(0.1f, 0.4f, 0.9f);
        
    RefPtr<UI> mainUI = new UI(font, 16);
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

    sceneManager.addDrawable(bg);
            
    bool done = false;
    while (!done) 
    {
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

        if (game->getLevel()->isCompleted())
        {            
            game->loadLevel(game->getLevelNumber() + 1);  
            mainUI->updateLevelLabel(game->getLevelNumber());
        }

        game->updateControls();
    }
    
    sceneManager.teardown();

    return 0;
}