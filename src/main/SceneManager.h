#ifndef __BRECHEN_SCENEMANAGER_H
#define __BRECHEN_SCENEMANAGER_H

#include <tsu/drawables/scene.h>

class SceneManager
{
public:
    SceneManager();

    void Setup();
    void Teardown();
    void Draw();
    void AddDrawable(Drawable* drawable);

private:
    RefPtr<Scene> m_scene;
};

#endif