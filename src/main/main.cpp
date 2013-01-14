#include <math.h>
#include <kos.h>
#include <oggvorbis/sndoggvorbis.h>
#include <tsu/font.h>
#include <tsu/texture.h>
#include <tsu/drawables/scene.h>
#include <tsu/drawables/label.h>
#include <plx/sprite.h>
#include <plx/context.h>
#include "../drawables/Paddle.h"
#include "../drawables/Brick.h"
#include "../drawables/Ball.h"
#include "Level.h"

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

KOS_INIT_FLAGS(INIT_DEFAULT | INIT_NO_DCLOAD | INIT_QUIET);

struct GameData
{
    int score;
    int level;
    int lives;
};

GameData gameData;

void CheckCollisions(Ball* ball, Paddle* paddle, Level& level)
{
    // Ball and Paddle collision.
    // Hacky shit. Oh well.
    const Vector& ballPosition = ball->getPosition();
    if (ballPosition.x < 20 || ballPosition.x > 620)
    {
        // Bouncing off a side wall. Reflect about the x axis.
        Vector velocity = ball->getVelocity();
        velocity.x = -velocity.x;
        ball->setVelocity(velocity);
        return;
    }

    if (ballPosition.y < 50 || ballPosition.y > 450)
    {
        // Bouncing off the top wall. Reflect about the y axis.
        Vector velocity = ball->getVelocity();
        velocity.y = -velocity.y;
        ball->setVelocity(velocity);
        return;
    }

    const Vector& thePaddlePosition = paddle->getPosition();
    const int HALF_BALL_HEIGHT = 16/2;
    const int HALF_BALL_WIDTH = 16/2;
    const int HALF_PADDLE_HEIGHT = 12/2;
    const int HALF_PADDLE_WIDTH = 36/2;
    if (ballPosition.y + HALF_BALL_HEIGHT >= thePaddlePosition.y - HALF_PADDLE_HEIGHT)
    {
        // We're lined up on the Y axis. Are we also within the X bounds?
        if (ballPosition.x + HALF_BALL_WIDTH >= thePaddlePosition.x - HALF_PADDLE_WIDTH && 
            ballPosition.x - HALF_BALL_WIDTH <= thePaddlePosition.x + HALF_PADDLE_WIDTH)
        {
            Vector hyp = thePaddlePosition - ballPosition;   
            hyp.normalizeSelf();
            hyp *= ball->getSpeed();
            ball->setVelocity(-hyp);            
        }
    }

    // Check the bricks.
    gameData.score += level.CheckCollision(ball);
}

void nextLevel()
{    
}

int main(int argc, char** argv)
{
    //snd_stream_init();
    //sndoggvorbis_init();

   // sndoggvorbis_start("/cd/data/scheme.ogg", 0);

    pvr_init_defaults();    

    RefPtr<Scene> sc = new Scene();
    RefPtr<Font> fnt = new Font("/rd/typewriter.txf");
    fnt->setColor(0.1f, 0.4f, 0.9f);
        
    gameData.score = 0;
    char scoreString[256];
    sprintf(scoreString, "%s%d", "Score: ", gameData.score);
    RefPtr<Label> scoreLabel = new Label(fnt, scoreString, 16, false, false);
    scoreLabel->setTranslate(Vector(80.0f, 40.0f, 10.0f));
    sc->subAdd(scoreLabel);    
    
    gameData.level = 1;
    char levelString[256];
    sprintf(levelString, "%s%d", "Level: ", gameData.level);
    RefPtr<Label> levelLabel = new Label(fnt, levelString, 16, false, false);
    levelLabel->setTranslate(Vector(240.0f, 40.0f, 10.0f));
    sc->subAdd(levelLabel);    
    
    gameData.lives = 3;
    char livesString[256];
    sprintf(livesString, "%s%d", "Lives: ", gameData.lives);
    RefPtr<Label> livesLabel = new Label(fnt, livesString, 16, false, false);
    livesLabel->setTranslate(Vector(450.0f, 40.0f, 10.0f));
    sc->subAdd(livesLabel);        

    pvr_set_bg_color(0.0f, 0.0f, 0.2f);
    Vector scorePosition = Vector(30.0f, 30.0f, 10.0f);

    // Make a paddle
    RefPtr<Paddle> paddle = new Paddle();
    Vector paddlePosition = Vector(296.0f, 420.0f, 10.0f);
    paddle->setTint(Color(1.0f, 0.0, 0.0f));
    paddle->setTranslate(paddlePosition);
    paddle->setSize(48, 12);
    paddle->setSpeed(5.0f);
    sc->subAdd(paddle);

    RefPtr<Texture> ballTexture = new Texture("/rd/ball.png", true);
    RefPtr<Ball> ball = new Ball(ballTexture);
    ball->setTranslate(paddlePosition + Vector(0.0f, -16.0f, 0.0f));
    sc->subAdd(ball);

    Level level;
    level.load(gameData.level);
    level.addToScene(sc);

    bool done = false;
    while (!done) 
    {
        pvr_wait_ready();
        pvr_scene_begin();        

        pvr_list_begin(PVR_LIST_OP_POLY);       
        sc->draw(PVR_LIST_OP_POLY);
        pvr_list_finish();
        
        pvr_list_begin(PVR_LIST_TR_POLY);      
        sc->draw(PVR_LIST_TR_POLY);
        pvr_list_finish();

        pvr_scene_finish();

        sc->nextFrame();

        int oldScore = gameData.score;
        CheckCollisions(ball, paddle, level);
        if (oldScore != gameData.score)
        {
            sprintf(scoreString, "%s%d", "Score: ", gameData.score);
            scoreLabel->setText(scoreString);
        }

        if (level.isCompleted())
        {
            ++gameData.level;
            level.load(gameData.level);
            level.addToScene(sc);
            ball->reset();
            sprintf(levelString, "%s%d", "Level: ", gameData.level);
            levelLabel->setText(levelString);
        }

        MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, t)
            
        if (t->buttons & CONT_A)
        {
            if (!ball->getIsLaunched())
            {
                ball->launch();
            }
        }

        if (t->buttons & CONT_DPAD_LEFT)
        {
            paddlePosition.x -= paddle->getSpeed();
            if (paddlePosition.x < 20)
            {
                paddlePosition.x = 20;
            }
        }  

        if (t->buttons & CONT_DPAD_RIGHT)
        {
            paddlePosition.x += paddle->getSpeed();
            if (paddlePosition.x > 620)
            {
                paddlePosition.x = 620;
            }
        }  
        
        if (!ball->getIsLaunched())
        {
            ball->setTranslate(paddlePosition + Vector(0.0f, -12.0f, 0.0f));
        }

        MAPLE_FOREACH_END()
                        
        paddle->setTranslate(paddlePosition);        
    }
    
    //sndoggvorbis_shutdown();

    //spu_disable();

    return 0;
}