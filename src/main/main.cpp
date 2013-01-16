#include <math.h>
#include <kos.h>
#include <oggvorbis/sndoggvorbis.h>
#include <tsu/font.h>
#include <tsu/texture.h>
#include <tsu/drawables/scene.h>
#include <plx/sprite.h>
#include <plx/context.h>
#include "../drawables/Paddle.h"
#include "../drawables/Brick.h"
#include "../drawables/Ball.h"
#include "../drawables/UI.h"
#include "Level.h"
#include "SceneManager.h"
#include "Game.h"

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_NO_DCLOAD | INIT_QUIET);

int main(int argc, char** argv)
{    
    SceneManager sceneManager;
    sceneManager.Setup();
    
    Game game;
    sceneManager.AddDrawable(game.GetPaddle());
    sceneManager.AddDrawable(game.GetBall());
    
    RefPtr<Font> font = new Font("/rd/typewriter.txf");
    font->setColor(0.1f, 0.4f, 0.9f);
        
    RefPtr<UI> mainUI = new UI(font, 16);
    mainUI->UpdateScoreLabel(game.GetScore());
    mainUI->UpdateLivesLabel(game.GetLives());
    mainUI->UpdateLevelLabel(game.GetLevelNumber());
    sceneManager.AddDrawable(mainUI);   
    
    game.GetLevel()->AddToScene(sceneManager.GetScene());
    
    bool done = false;
    while (!done) 
    {
        sceneManager.Draw();

        int oldScore = game.GetScore();
        game.CheckCollisions();
        if (oldScore != game.GetScore())
        {
            mainUI->UpdateScoreLabel(game.GetScore());
        }

        if (game.GetLevel()->IsCompleted())
        {            
            game.LoadLevel(game.GetLevelNumber() + 1);
            game.GetLevel()->AddToScene(sceneManager.GetScene());     
            mainUI->UpdateLevelLabel(game.GetLevelNumber());
        }

        game.UpdateControls();
    }
    
    sceneManager.Teardown();

    return 0;
}