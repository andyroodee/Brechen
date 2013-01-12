#include "Ball.h"

Ball::Ball(Texture* texture)
{
    m_isLaunched = false;
    m_texture = texture;
    m_radius = 2.0f;
    m_speed = 3.0f;
    m_velocity = Vector(0.0f, 0.0f, 0.0f);
    m_u1 = m_v2 = 0.0f;
	m_u2 = m_v4 = 0.0f;
	m_u3 = m_v1 = 1.0f;
	m_u4 = m_v3 = 1.0f;
}

void Ball::launch()
{
    if (!m_isLaunched)
    {
        m_isLaunched = true;
        // Start moving!
        m_velocity = Vector(0.0f, -2.0f, 0.0f);
    }
}

void Ball::draw(int list)
{
    if (list != PVR_LIST_TR_POLY || !m_texture)
		return;

	m_texture->sendHdr(list);
    	
	float w = m_texture->getW();
	float h = m_texture->getH();

	const Vector& sv = getScale();
	w *= sv.x;
	h *= sv.y;

    translate(m_velocity);

	const Vector& tv = getPosition();
    
	plx_vertex_t vert;
	if (list == PLX_LIST_TR_POLY) {
		vert.argb = getColor();
	} else {
		Color t = getColor(); t.a = 1.0f;
		vert.argb = t;
	}
	vert.oargb = 0;

	vert.flags = PLX_VERT;
	vert.x = tv.x-w/2;
	vert.y = tv.y+h/2;
	vert.z = tv.z;
	vert.u = m_u1;
	vert.v = m_v1;
	plx_prim(&vert, sizeof(vert));

	vert.y = tv.y-h/2;
	vert.u = m_u2;
	vert.v = m_v2;
	plx_prim(&vert, sizeof(vert));

	vert.x = tv.x+w/2;
	vert.y = tv.y+h/2;
	vert.u = m_u3;
	vert.v = m_v3;
	plx_prim(&vert, sizeof(vert));

	vert.flags = PLX_VERT_EOS;
	vert.y = tv.y-h/2;
	vert.u = m_u4;
	vert.v = m_v4;
	plx_prim(&vert, sizeof(vert));

	Drawable::draw(list);
}