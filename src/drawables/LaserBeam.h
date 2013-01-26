#ifndef __BRECHEN_LASERBEAM_H
#define __BRECHEN_LASERBEAM_H

#include <tsu/drawable.h>
#include <tsu/texture.h>
#include <tsu/vector.h>
#include <plx/list.h>
#include <plx/sprite.h>

class Brick;

class LaserBeam : public Drawable
{
public:
    LaserBeam(Texture* texture);

    virtual ~LaserBeam() {}

    virtual void draw(int list);
    
    bool isHitting(Brick* brick);

private:
    Vector m_velocity;
    RefPtr<Texture>	m_texture;
	float m_u1, m_v1, m_u2, m_v2;
	float m_u3, m_v3, m_u4, m_v4;
};

#endif