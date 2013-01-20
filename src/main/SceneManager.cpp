#include "SceneManager.h"
#include <kos.h>
#include <oggvorbis/sndoggvorbis.h>

SceneManager::SceneManager()
{
}

void SceneManager::setup()
{
    snd_stream_init();
    sndoggvorbis_init();

    pvr_init_defaults(); 

    pvr_set_bg_color(0.0f, 0.0f, 0.2f);

    m_scene = new Scene();
}

void SceneManager::teardown()
{
    sndoggvorbis_shutdown();
    spu_disable();
}

void SceneManager::draw()
{
    pvr_wait_ready();
    pvr_scene_begin();        

    pvr_list_begin(PVR_LIST_OP_POLY);       
    m_scene->draw(PVR_LIST_OP_POLY);
    pvr_list_finish();
        
    pvr_list_begin(PVR_LIST_TR_POLY);      
    m_scene->draw(PVR_LIST_TR_POLY);
    pvr_list_finish();

    pvr_scene_finish();

    m_scene->nextFrame();
}

void SceneManager::addDrawable(Drawable* drawable)
{
    m_scene->subAdd(drawable);
}